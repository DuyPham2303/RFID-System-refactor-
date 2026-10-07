#include "Spi.h"
#include "Spi_hW.h"
#include "Mcu_Irq.h"

// Biến lưu trạng thái runtime của Job hiện tại (để biết đang ở Channel và job nào, đang xử lý buffer nào)
typedef struct
{
    Spi_JobId_Type CurrentActiveJobId;    /*xác định Id của Job đang xử lý trên stack*/
    Spi_ChannelId_Type CurrentActiveChId; /*xác định Id của channel trong danh sách job đang xử lý runtime */
    uint16 *ActiveTxPtr;                  /*con trỏ tới buffer chứa dữ liệu truyền*/
    uint16 *ActiveRxPtr;                  /*con trỏ tới buffer sẽ đọc về dữ liệu*/
    uint8 Length;                         /*kích thước của buffer gửi/nhận*/
} Spi_JobRuntimeSyncType;

/*danh sách quản lý cấu hình thông số Spi của từng channel*/
static const Spi_ConfigTypes_s *Spi_ConfigPtr_s;

/*biến quản lý trạng thái runtime của Job*/
static Spi_JobRuntimeSyncType Job_SyncRuntime;

Std_ReturnType Spi_Init(const Spi_ConfigTypes_s *ConfigPtr)
{
    if (ConfigPtr == NULL_PTR)
    {
        return E_NOT_OK;
    }

    /*duyệt qua từng cấu hình trong danh sách*/
    for (uint8 currentChannel = 0; currentChannel < SPI_MAX_CHANNEL; currentChannel++)
    {
        /*Khởi tạo đối tượng lưu trữ cấu hình xuống phần cứng*/
        SPI_InitTypeDef Spi_InitCfg_s;

        /*đọc cấu hình Spi của từng phần tử*/
        const Spi_ParamConfigType *ChannelParam = &ConfigPtr->ChannelParamCfgPtr[currentChannel];

        /*truy xuất địa chỉ cứng của Spi*/
        Spi_GroupId_Type HwId = ChannelParam->HwId;
        SPI_TypeDef *Spix = Spi_Map_GetHwInstance(HwId);
        /*Kiểm tra địa chỉ hợp lệ*/
        if (Spix == NULL_PTR)
        {
            return E_NOT_OK;
        }

        const Mcu_NvicConfigType *NvicConfig_s = NULL_PTR;
        const Spi_AsyncSeqCfgType *AsyncSeqCfgPtr_s = NULL_PTR;

        /*cấu hình NVIC*/
        if (ChannelParam->NvicCfgPtr != NULL_PTR && ChannelParam->AsyncNotiPtr != NULL_PTR)
        {
            /*đọc cấu hình NVIC và ánh xạ xuống thanh ghi phần cứng*/
            NvicConfig_s = ChannelParam->NvicCfgPtr;
            Mcu_IrqInit(NvicConfig_s);

            /*truy cập cấu hình đăng ký callback Api*/
            AsyncSeqCfgPtr_s = ChannelParam->AsyncNotiPtr;

            Spi_JobId_Type JobIdFound;
            Spi_SequenceId_Type SeqIdFound;
            Spi_GetJobAndSeqId(currentChannel, &JobIdFound, &SeqIdFound);

            /*đắng ký callback và cờ ngắt cho channel*/
            Spi_RegisterSeqNoti(AsyncSeqCfgPtr_s, JobIdFound, SeqIdFound);
        }
        /*đọc từng thông cấu hình */
        Spi_InitCfg_s.SPI_Mode = Spi_Map_GetMode(ChannelParam->Mode);
        Spi_InitCfg_s.SPI_NSS = Spi_Map_GetNss(ChannelParam->Nss);
        Spi_InitCfg_s.SPI_FirstBit = Spi_Map_GetFirstBit(ChannelParam->FirstBit);
        Spi_InitCfg_s.SPI_Direction = Spi_Map_GetDir(ChannelParam->Dir);
        Spi_InitCfg_s.SPI_DataSize = Spi_Map_GetDataSize(ChannelParam->DataSize);
        Spi_InitCfg_s.SPI_CPOL = Spi_Map_GetCpol(ChannelParam->CPOL);
        Spi_InitCfg_s.SPI_CPHA = Spi_Map_GetCpha(ChannelParam->CPHA);
        Spi_InitCfg_s.SPI_BaudRatePrescaler = Spi_Map_GetBaudRate(ChannelParam->BaudRatePrescaler);

        /*ánh xạ cấu hình lưu trữ xuống thanh ghi cứng*/
        SPI_Init(Spix, &Spi_InitCfg_s);
        /*kích hoạt phần cứng SPI*/
        SPI_Cmd(Spix, ENABLE);

        /*cập nhật trạng thái của driver Spi*/
        Spi_Runtime_SetStatus(HwId, SPI_IDLE);
    }

    /*copy cấu hình lưu trữ sang biến tĩnh sử dụng cục bộ*/
    Spi_ConfigPtr_s = ConfigPtr;
    return E_OK;
}

Std_ReturnType Spi_WriteIB(Spi_ChannelId_Type Channel, const uint16 *DataBuffer, uint8 Length)
{
    // Kiểm tra tính hợp lệ của tham số
    if ((Channel >= SPI_MAX_IB_CHANNELS) || (DataBuffer == NULL_PTR) || (Length > SPI_IB_MAX_LENGTH))
    {
        return E_NOT_OK;
    }

    // Copy dữ liệu từ App vào Internal Buffer do Driver quản lý
    for (uint8 i = 0; i < Length; i++)
    {
        Spi_IbPool[Channel].TxBuffer[i] = DataBuffer[i];
    }

    // Lưu lại độ dài dữ liệu thực tế sẽ truyền
    Spi_IbPool[Channel].Length = Length;

    return E_OK;
}

Std_ReturnType Spi_ReadIB(Spi_ChannelId_Type Channel, uint16 *DataBuffer, uint8 Length)
{
    // Kiểm tra tính hợp lệ
    if ((Channel >= SPI_MAX_IB_CHANNELS) || (DataBuffer == NULL_PTR) || (Length > SPI_IB_MAX_LENGTH))
    {
        return E_NOT_OK;
    }

    // Copy dữ liệu từ Internal Buffer của Driver ra biến ở App sau khi quá trình truyền nhận hoàn tất
    for (uint8 i = 0; i < Length; i++)
    {
        DataBuffer[i] = Spi_IbPool[Channel].RxBuffer[i];
    }

    return E_OK;
}

Std_ReturnType Spi_SetupEB(Spi_ChannelId_Type ChId, uint16 *TxBuffer, uint16 *RxBuffer, uint8 Length)
{
    if (ChId >= SPI_MAX_CHANNEL || Length > MAX_WIDTH_BYTE)
    {
        return E_NOT_OK;
    }

    // Cập nhật con trỏ và độ dài vào vùng nhớ RAM Runtime của Channel
    Spi_ChannelsRuntime[ChId].BufferType = SPI_BUFFER_TYPE_EB;
    Spi_ChannelsRuntime[ChId].TxBufferPtr = TxBuffer;
    Spi_ChannelsRuntime[ChId].RxBufferPtr = RxBuffer;
    Spi_ChannelsRuntime[ChId].DefaultLength = Length;

    Spi_GroupId_Type HwId = Spi_ConfigPtr_s->ChannelParamCfgPtr[ChId].HwId;
    Spi_Runtime_SetStatus(HwId, SPI_BUSY);
    return E_OK;
}

Std_ReturnType Spi_SyncTransmit(Spi_SequenceId_Type SeqId)
{
    /*Kiểm tra bus có đang busy*/
    uint8 Jobtotal = Spi_Sequences[SeqId].ActiveJobCount;
    for (uint8 index = 0; index < Jobtotal; index++)
    {
        Spi_JobId_Type JobId = Spi_Sequences[SeqId].JobList[index];
        Spi_GroupId_Type HwId = Spi_Jobs[JobId].HwId;
        if (Spi_Runtime_GetStatus(HwId) == SPI_BUSY)
        {
            return E_NOT_OK;
        }
        Spi_Runtime_SetBusBusy(HwId, TRUE, TRUE);
    }

    /*duyệt qua danh sách các tác vụ của nghiệp vụ hiện tại đã truy cập*/
    for (uint8 JobIndex = 0; JobIndex < Jobtotal; JobIndex++)
    {
        /*truy cập danh sách job */
        Spi_JobId_Type JobId = Spi_Sequences[SeqId].JobList[JobIndex];

        /*truy cập tác vụ (danh sách data channel + số lượng) qua ID ánh xạ*/
        uint8 ChTotal = Spi_Jobs[JobId].ActiveChannelCount;

        /*Gửi tín hiệu bắt đầu phiên giao tiếp Spi*/
        Dio_WriteChannel(Spi_Jobs[JobId].CsPinId, STD_LOW);

        Job_SyncRuntime.CurrentActiveJobId = JobId; // gán Job đầu tiên
        /*duyệt qua và xử lý danh sách channel của job hiện tại*/
        for (uint8 ChIndex = 0; ChIndex < ChTotal; ChIndex++)
        {
            /*lọc ra Id channel quản lý bởi Job hiện tại*/
            Spi_ChannelId_Type ChId = Spi_Jobs[JobId].ChannelList[ChIndex];

            // Khởi tạo trạng thái ban đầu cho runtime memory quản lý cho 1 Job tại 1 thời điểm
            Job_SyncRuntime.CurrentActiveChId = ChId; // khởi tạo vị trí của channel đầu tiên sẽ xử lý
            Job_SyncRuntime.ActiveTxPtr = NULL_PTR;   // mặc định chưa có dữ liệu truyền
            Job_SyncRuntime.ActiveRxPtr = NULL_PTR;   // mặc định chưa có dữ liệu nhận
            Job_SyncRuntime.Length = 0;

            // Lựa chọn phương thức truy cập dữ liệu EB hay IB
            if (Spi_ChannelsRuntime[ChId].BufferType == SPI_BUFFER_TYPE_EB)
            {
                Job_SyncRuntime.ActiveTxPtr = Spi_ChannelsRuntime[ChId].TxBufferPtr;
                Job_SyncRuntime.ActiveRxPtr = Spi_ChannelsRuntime[ChId].TxBufferPtr;
                Job_SyncRuntime.Length = Spi_ChannelsRuntime[ChId].DefaultLength;
            }
            else
            {
                Job_SyncRuntime.ActiveTxPtr = Spi_IbPool[ChId].TxBuffer;
                Job_SyncRuntime.ActiveRxPtr = Spi_IbPool[ChId].RxBuffer;
                Job_SyncRuntime.Length = Spi_IbPool[ChId].Length;
            }

            Spi_ParamConfigType *channelcfg = &Spi_ConfigPtr_s->ChannelParamCfgPtr[ChId];
            // khởi tạo cấu trúc runtime cho phiên giao tiếp Spi trên bus
            Spi_Hw_dataConfigType HwDataCfgPtr = {
                .direction = Spi_ConfigPtr_s->ChannelParamCfgPtr[ChId].Dir,
                .HwId = Spi_ConfigPtr_s->ChannelParamCfgPtr[ChId].HwId,
                .SizeType = Spi_ConfigPtr_s->ChannelParamCfgPtr[ChId].DataSize,
                .length = Job_SyncRuntime.Length,
                .pRxData = Job_SyncRuntime.ActiveRxPtr,
                .pTxData = Job_SyncRuntime.ActiveRxPtr};

            // xử lý data transaction
            Spi_Hw_Sync_TransmitReceive(&HwDataCfgPtr);
        }
        /*Kéo chân Cs của Job xuống Low*/
        Dio_WriteChannel(Spi_Jobs[JobId].CsPinId, STD_HIGH);

        /*Gửi tín hiệu kết thúc phiên giao tiếp Spi*/
        Dio_WriteChannel(Spi_Jobs[JobId].CsPinId, STD_HIGH);
    }
    return E_OK;
}

Std_ReturnType Spi_AsyncTransmit(Spi_SequenceId_Type SeqId)
{
    /*
    //truy xuất trạng thái của Job đầu tiên trong danh sách và Sequence hiện tại quản lý
    Spi_JobId_Type firstjobId = Spi_Sequences[SeqId].JobList[0];
    Spi_GroupId_Type HwId = Spi_Jobs[firstjobId].HwId;

    // kiểm tra driver Spi của Job có đang bận không
    if (SeqId > SPI_SEQ_MAX || Spi_HWUnitStatus[HwId].Status == SPI_BUSY)
    {
        return E_NOT_OK;
    }

    // gọi hàm nội bộ để xử lý lần lượt từng job
    Spi_InternalStartJob(firstjobId, SeqId);

    // trả về trạng thái cho App tiếp tục xử lý mà không cần chờ
    Spi_HWUnitStatus[HwId].Status = SPI_BUSY;
    return E_OK;
    */
}
