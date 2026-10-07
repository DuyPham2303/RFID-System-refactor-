#include "Spi.h"
#include "Mcu_Irq.h"
#include "Spi_Internal.h"
#include "Spi_Runtime.h"

/*danh sách quản lý cấu hình thông số Spi của từng bộ SPi*/
static const Spi_ConfigType_s *s_ConfigPtr;

Std_ReturnType Spi_Init(const Spi_ConfigType_s *ConfigPtr)
{
    if (ConfigPtr == NULL_PTR)
    {
        return E_NOT_OK;
    }

    Spi_Runtime_Init(ConfigPtr);

    /*duyệt qua từng cấu hình trong danh sách*/
    for (uint8 Hwindex = 0; Hwindex < ConfigPtr->DeviceCount; Hwindex++)
    {
        /*Khởi tạo đối tượng lưu trữ cấu hình xuống phần cứng*/
        SPI_InitTypeDef Spi_HwInitGroup;

        /*đọc cấu hình Spi của từng phần tử*/
        const Spi_ExternalDeviceConfigType_s *CurrentDeviceCfgPtr = &ConfigPtr->DeviceConfigPtr[Hwindex];

        /*truy xuất địa chỉ cứng của Spi*/
        Spi_HwUnitType_e HwId = CurrentDeviceCfgPtr->HwId;
        SPI_TypeDef *Spix = Spi_Map_GetHwInstance(HwId);

        const Mcu_NvicConfigType_s *NvicConfig_s = CurrentDeviceCfgPtr->NvicCfgPtr;

        /*cấu hình NVIC*/
        if (NvicConfig_s != NULL_PTR)
        {
            /*ánh xạ xuống thanh ghi phần cứng*/
            Mcu_IrqInit(NvicConfig_s);
        }
        /*đọc từng thông cấu hình */
        Spi_HwInitGroup.SPI_Mode = Spi_Map_GetMode(CurrentDeviceCfgPtr->Mode);
        Spi_HwInitGroup.SPI_NSS = Spi_Map_GetNss(CurrentDeviceCfgPtr->Nss);
        Spi_HwInitGroup.SPI_FirstBit = Spi_Map_GetFirstBit(CurrentDeviceCfgPtr->FirstBit);
        Spi_HwInitGroup.SPI_Direction = Spi_Map_GetDir(CurrentDeviceCfgPtr->Dir);
        Spi_HwInitGroup.SPI_DataSize = Spi_Map_GetDataSize(CurrentDeviceCfgPtr->DataSize);
        Spi_HwInitGroup.SPI_CPOL = Spi_Map_GetCpol(CurrentDeviceCfgPtr->CPOL);
        Spi_HwInitGroup.SPI_CPHA = Spi_Map_GetCpha(CurrentDeviceCfgPtr->CPHA);
        Spi_HwInitGroup.SPI_BaudRatePrescaler = Spi_Map_GetBaudRate(CurrentDeviceCfgPtr->BaudRatePrescaler);

        /*ánh xạ cấu hình lưu trữ xuống thanh ghi cứng*/
        SPI_Init(Spix, &Spi_HwInitGroup);
        /*kích hoạt phần cứng SPI*/
        SPI_Cmd(Spix, ENABLE);

        /*cập nhật trạng thái của driver Spi*/
        Spi_Runtime_SetHwGroupStatus(HwId, SPI_IDLE);
    }
    /*copy cấu hình lưu trữ sang biến tĩnh sử dụng cục bộ*/
    s_ConfigPtr = ConfigPtr;
    return E_OK;
}

Std_ReturnType Spi_WriteIB(Spi_ChannelType_e Channel, const uint16 *DataBuffer, uint8 Length)
{
    Spi_BufferType bufType = s_ConfigPtr->ChannelConfigPtr[Channel].BufferType;
    // Kiểm tra tính hợp lệ của tham số
    if (bufType != SPI_BUFFER_TYPE_IB)
    {
        return E_NOT_OK;
    }
    return Spi_Runtime_Writebuffer_IbChannel(Channel, DataBuffer, Length);
}

Std_ReturnType Spi_ReadIB(Spi_ChannelType_e Channel, uint16 *DataBuffer, uint8 Length)
{
    // Kiểm tra tính hợp lệ
    Spi_BufferType bufType = s_ConfigPtr->ChannelConfigPtr[Channel].BufferType;
    // Kiểm tra tính hợp lệ của tham số
    if (bufType != SPI_BUFFER_TYPE_IB)
    {
        return E_NOT_OK;
    }

    // đọc dữ liệu từ Internal Buffer của Driver ra biến ở App sau khi quá trình truyền nhận hoàn tất
    return Spi_Runtime_ReadBufer_IbChannel(Channel, DataBuffer);
}

Std_ReturnType Spi_SetupEB(Spi_ChannelType_e Channel, uint16 *TxBuffer, uint16 *RxBuffer, uint8 Length)
{
    Spi_BufferType bufType = s_ConfigPtr->ChannelConfigPtr[Channel].BufferType;
    if (bufType != SPI_BUFFER_TYPE_EB)
    {
        return E_NOT_OK;
    }

    // Thiết lập địa chỉ quản lý buffer của stack runtime
    return Spi_Runtime_SetChannelBuffers(Channel, TxBuffer, RxBuffer, Length);
}

static Std_ReturnType Spi_TransmitCore(Spi_SequenceType_e SeqId, Spi_ModeType Mode)
{
    Spi_SequenceRuntimeType_s *currentSeqPtr = Spi_Runtime_GetSequence(SeqId);
    if (currentSeqPtr == NULL_PTR)
    {
        return E_NOT_OK;
    }

    // 1. Kiểm tra Sequence có đang bận không
    if (currentSeqPtr->Status == SPI_BUSY)
    {
        return E_NOT_OK;
    }

    // 2. Lấy Job đầu tiên của Sequence
    Spi_JobType_e JobId = s_ConfigPtr->SequenceConfigPtr[SeqId].JobList[0];
    Spi_HwUnitType_e HwId = s_ConfigPtr->JobConfigPtr[JobId].HwId;

    boolean Txbusy, Rxbusy;

    // Khuyến nghị đặt trong Critical Section nếu hệ thống đa nhiệm/ngắt phức tạp
    // SchM_Enter_Spi();

    // 3. Kiểm tra Bus phần cứng
    Spi_Runtime_GetBusBusy(HwId, &Txbusy, &Rxbusy);
    if (Txbusy == TRUE || Rxbusy == TRUE)
    {
        // SchM_Exit_Spi();
        return E_NOT_OK;
    }

    // 4. Thiết lập thông số runtime cho nghiệp vụ
    currentSeqPtr->Status = SPI_BUSY;
    currentSeqPtr->Mode = Mode;
    currentSeqPtr->CurrentJobIndex = 0;
    currentSeqPtr->CurrentSequenceId = SeqId;

    // SchM_Exit_Spi();

    // 5. Kích hoạt Job đầu tiên
    return Spi_InternalStartJob(JobId, SeqId);
}

Std_ReturnType Spi_SyncTransmit(Spi_SequenceType_e SeqId)
{
    return Spi_TransmitCore(SeqId, SPI_POLLING_MODE);
}

Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType_e SeqId)
{
    return Spi_TransmitCore(SeqId, SPI_INTERRUPT_MODE);
}