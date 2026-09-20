#include "Spi.h"
#include "Spi_Internal.h"
#include "Spi_map.h"
#include "Mcu_Irq.h"

#define MAX_WIDTH_BYTE_TRANSFER 100

/*danh sách quản lý cấu hình thông số Spi của từng channel*/
static const Spi_ConfigType *Spi_ConfigPtr_s;

/*API nội bộ để xử lý logic low level giao tiếp SPI */
static void Spi_Transmit(void *pTxData, Spi_ChannelId_Type channel, uint16_t Size);
static void Spi_Receive(void *pRxData, Spi_ChannelId_Type channel, uint16_t Size);
static void Spi_TxRx(uint16 *pTxData, uint16 *pRxData, Spi_ChannelId_Type channel, uint16_t Size);

Std_ReturnType Spi_Init(const Spi_ConfigType *ConfigPtr)
{
    if (ConfigPtr == NULL_PTR)
    {
        return E_NOT_OK;
    }

    /*duyệt qua từng cấu hình trong danh sách*/
    for (uint8 index = 0; index < SPI_GROUP_MAX; index++)
    {
        /*Khởi tạo đối tượng lưu trữ cấu hình xuống phần cứng*/
        SPI_InitTypeDef Spi_InitCfg_s;
        /*đọc cấu hình Spi của từng phần tử*/
        const Spi_ConfigType *ChannelConfig = &ConfigPtr[index];

        /*kiểm tra cấu hình NVIC*/
        if (ChannelConfig->IrqCfgPtr != NULL_PTR)
        {
            /*truy cập cấu hình NVIC*/
            const Mcu_IrqConfigType *IrqConfig_s = ChannelConfig->IrqCfgPtr;

            /*truyền con trỏ cấu hình xuống tầng thanh ghi*/
            Mcu_IrqInit(IrqConfig_s);
        }

        /*truy xuất địa chỉ cứng của Spi*/
        SPI_TypeDef *Spix = Spi_Map_GetHwInstance(ChannelConfig->HwId);
        /*Kiểm tra địa chỉ hợp lệ*/
        if (Spix == NULL_PTR)
        {
            return E_NOT_OK;
        }
        /*đọc từng thông cấu hình */
        Spi_InitCfg_s.SPI_Mode = Spi_Map_GetMode(ChannelConfig->Mode);
        Spi_InitCfg_s.SPI_NSS = Spi_Map_GetNss(ChannelConfig->Nss);
        Spi_InitCfg_s.SPI_FirstBit = Spi_Map_GetFirstBit(ChannelConfig->FirstBit);
        Spi_InitCfg_s.SPI_Direction = Spi_Map_GetDir(ChannelConfig->Dir);
        Spi_InitCfg_s.SPI_DataSize = Spi_Map_GetDataSize(ChannelConfig->DataSize);
        Spi_InitCfg_s.SPI_CPOL = Spi_Map_GetCpol(ChannelConfig->CPOL);
        Spi_InitCfg_s.SPI_CPHA = Spi_Map_GetCpha(ChannelConfig->CPHA);
        Spi_InitCfg_s.SPI_BaudRatePrescaler = Spi_Map_GetBaudRate(ChannelConfig->BaudRatePrescaler);

        /*ánh xạ cấu hình lưu trữ xuống thanh ghi cứng*/
        SPI_Init(Spix, &Spi_InitCfg_s);
        /*kích hoạt phần cứng SPI*/
        SPI_Cmd(Spix, ENABLE);

        /*kích hoạt ngắt và cấu hình cờ ngắt*/

        if (ChannelConfig->IrqCfgPtr->cmd)
        {
            SPI_I2S_ITConfig(Spix, SPI_I2S_IT_TXE, ENABLE);
        }

        /*cập nhật channel status runtime*/
        Spi_Channels_Status[index].Status = SPI_IDLE;
    }

    /*copy cấu hình lưu trữ sang biến tĩnh sử dụng cục bộ*/
    Spi_ConfigPtr_s = ConfigPtr;
    return E_OK;
}
Spi_StatusType Spi_GetStatus(void)
{
    Spi_StatusType st;
    for (uint8 index = 0; index < SPI_CHANNEL_MAX; index++)
    {
        st = Spi_Channels_Status[index].Status;
        if (st == SPI_UNINIT)
        {
            break;
        }
    }
    return st;
}
Std_ReturnType Spi_SetupEB(
    Spi_ChannelId_Type Id,
    uint16 *TxBuffer,
    uint16 *RxBuffer,
    uint16 Length)
{
    if ((TxBuffer == NULL_PTR && RxBuffer == NULL_PTR) ||
        Length > MAX_WIDTH_BYTE_TRANSFER)
    {
        return E_NOT_OK;
    }
    Spi_Channels_data[Id].dataWidthByte = Length;
    Spi_Channels_data[Id].TxBuffer = TxBuffer;
    Spi_Channels_data[Id].RxBuffer = RxBuffer;
    return E_OK;
}

Std_ReturnType Spi_SyncTransmit(Spi_SequenceId_Type SeqId)
{
    if (SeqId > SPI_SEQ_MAX)
    {
        return E_NOT_OK;
    }
    /*truy cập nghiệp vụ (danh sách tác vụ + số lượng) qua ID ánh xạ*/
    uint8 JobTotal = Spi_Sequences[SeqId].ActiveJobCount;

    /*duyệt qua danh sách các tác vụ của nghiệp vụ hiện tại đã truy cập*/
    for (uint8 Job = 0; Job < JobTotal; Job++)
    {
        /*truy cập tác vụ (danh sách data channel + số lượng) qua ID ánh xạ*/
        uint8 ChannelTotal = Spi_Jobs[Job].ActiveChannelCount;

        /*duyệt qua và xử lý lần lượt từng Id chanvnel của job hiện tại*/
        for (uint8 channel = 0; channel < ChannelTotal; channel++)
        {
            /*lọc ra Id channel quản lý bởi Job hiện tại*/
            Spi_ChannelId_Type IdMapJob = Spi_Jobs[Job].ChannelList[channel];

            /*cập nhật status xử lý của channel tương ứng*/
            Spi_Channels_Status[IdMapJob].Status = SPI_BUSY;

            /*đọc ra dữ liệu thực tế cần xử lý*/
            uint16 maxlen = Spi_Channels_data[channel].dataWidthByte;
            uint16 *txbuf = Spi_Channels_data[channel].TxBuffer;
            uint16 *rxbuf = Spi_Channels_data[channel].RxBuffer;
            /*xác định hướng truyền*/
            Spi_DirectionType dir = Spi_ConfigPtr_s[channel].Dir;
            /*phân luồng xử lý ứng với cấu hình hướng truyền (direction type)*/
            switch (dir)
            {
            case SPI_MR_2LINES_FD:
                /*hàm truyền tx/rx*/
                Spi_TxRx(txbuf, rxbuf, channel, maxlen);
                break;
            case SPI_MR_1LINE_TX:
                /*hàm truyền*/
                Spi_Transmit(txbuf, channel, maxlen);
                break;
            case SPI_MR_1LINE_RX:
                /*hàm nhận*/
                Spi_Receive(rxbuf, channel, maxlen);
                break;
            case SPI_MR_2LINES_RX_ONLY:
                /*not supported yet*/
                break;
            }
            /*cập nhật channel status runtime*/
            Spi_Channels_Status[IdMapJob].Status = SPI_IDLE;
        }
    }
    return E_OK;
}
Std_ReturnType Spi_AsyncTransmit(Spi_SequenceId_Type SeqId)
{
    if (SeqId > SPI_SEQ_MAX)
    {
        return E_NOT_OK;
    }
    return E_OK;
}
Std_ReturnType Spi_ReadIB(Spi_ChannelId_Type Channel, uint8 *DataBuffer)
{
    if (Channel > SPI_CHANNEL_MAX)
    {
        return E_NOT_OK;
    }
    return E_OK;
}
Std_ReturnType Spi_WriteIB(Spi_ChannelId_Type Channel, const uint8 *DataBuffer)
{
    if (Channel > SPI_CHANNEL_MAX)
    {
        return E_NOT_OK;
    }
    return E_OK;
}
//////////////////////////////*PRIVATE API*///////////////////////////

static inline void Spi_WaitTxEmpty(SPI_TypeDef *HwUnit)
{
    while (SPI_I2S_GetFlagStatus(HwUnit, SPI_I2S_FLAG_TXE) == RESET)
        ;
}
static inline void Spi_WaitRxReady(SPI_TypeDef *HwUnit)
{
    while (SPI_I2S_GetFlagStatus(HwUnit, SPI_I2S_FLAG_RXNE) == RESET)
        ;
}
static inline void Spi_WaitBusyClear(SPI_TypeDef *HwUnit)
{
    while (SPI_I2S_GetFlagStatus(HwUnit, SPI_I2S_FLAG_BSY) == SET)
        ;
}

static void Spi_Transmit(void *pTxData, Spi_ChannelId_Type channel, uint16_t Size)
{

    SPI_TypeDef *Spix = Spi_Map_GetHwInstance(Spi_ConfigPtr_s[channel].HwId);

    if (Spi_ConfigPtr_s[channel].DataSize == SPI_DataSize_16b)
    {
        uint16_t *pTxData16b = (uint16_t *)pTxData;
        uint8 index = 0;
        while (index < Size)
        {
            Spi_WaitTxEmpty(Spix);
            SPI_I2S_SendData(Spix, pTxData16b[index]);
            index++;
        }
    }
    else
    {
        uint8_t *pTxData8b = (uint8_t *)pTxData;
        uint8 index = 0;
        while (index < Size)
        {
            Spi_WaitTxEmpty(Spix);
            SPI_I2S_SendData(Spix, pTxData8b[index]);
            index++;
        }
    }
    Spi_WaitBusyClear(Spix);
}
static void Spi_Receive(void *pRxData, Spi_ChannelId_Type channel, uint16_t Size)
{
    SPI_TypeDef *Spix = Spi_Map_GetHwInstance(Spi_ConfigPtr_s[channel].HwId);

    if (Spi_ConfigPtr_s[channel].DataSize == SPI_DataSize_16b)
    {
        uint16_t *pRxData16b = (uint16_t *)pRxData;
        uint8 index = 0;
        while (index < Size)
        {
            Spi_WaitRxReady(Spix);
            pRxData16b[index] = SPI_I2S_ReceiveData(Spix);
            index++;
        }
    }
    else
    {
        uint8_t *pRxData8b = (uint8_t *)pRxData;
        uint8 index = 0;
        while (index < Size)
        {
            Spi_WaitRxReady(Spix);
            pRxData8b[index] = (uint8_t)SPI_I2S_ReceiveData(Spix);
            index++;
        }
    }
}
static void Spi_TxRx(uint16 *pTxData, uint16 *pRxData, Spi_ChannelId_Type channel, uint16_t Size)
{
    SPI_TypeDef *Spix = Spi_Map_GetHwInstance(Spi_ConfigPtr_s[channel].HwId);
    // SPI_NSSInternalSoftwareConfig(Spix,SPI_NSSInternalSoft_Reset);

    uint8 index = 0;
    while (index < Size)
    {
        Spi_WaitTxEmpty(Spix);
        SPI_I2S_SendData(Spix, pTxData[index]);
        Spi_WaitRxReady(Spix);
        pRxData[index] = SPI_I2S_ReceiveData(Spix);
        index++;
    }
    Spi_WaitBusyClear(Spix);
    // SPI_NSSInternalSoftwareConfig(Spix,SPI_NSSInternalSoft_Set);
}
