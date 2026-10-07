#include "Spi_Internal.h"
#include "Spi_Runtime.h"
#include "Spi_map.h"

/**
 * @brief Bảng cấu hình runtime thông tin quản lý danh sách Id và tác vụ callback ứng với các nghiệp vụ
 * @details được module Spi_Internal sử dụng để ánh xạ tới loại callback cụ thể và nguồn ngắt
 *          xử lý phù hợp
 */
static Spi_AsyncSeqCfgType s_NotiHandlerRuntime[SPI_SEQ_MAX] = {
    [SPI_SEQ_UPDATE_LED_STATUS] = {
        .JobAsyncArray = {
            [SPI_JOB_SEND_LED_CMD] = {
                .flag = Spi_IRQ_SOURCE_MAX,
                .JobNoti = NULL_PTR}},
        .SeqNoti = NULL_PTR}};
// Biến lưu trạng thái runtime của Job hiện tại (để biết đang ở Channel nào, byte thứ mấy)
typedef struct
{
    Spi_DataSizeType datasize;           /*kiểu dữ liệu đọc/ghi 8 hoặc 16 bit*/
    Spi_JobId_Type CurrentActiveJobId;   /*Id của Job đang xử lý trên stack*/
    uint8_t ChannelIndex;                /*Id của channel trong danh sách job đang xử lý runtime */
    uint16_t ByteIndex;                  /*vị trí của byte hiện tại mà Job đang thực hiện nhận/gửi*/
    uint16_t TotalBytesInCurrentChannel; /*tổng số byte truyền/nhận của channel được Job xử lý*/
    uint16 *ActiveTxPtr;                 /*con trỏ tới buffer chứa dữ liệu truyền*/
    uint16 *ActiveRxPtr;                 /*con trỏ tới buffer sẽ đọc về dữ liệu*/
    uint8 Length;                        /*kích thước của buffer gửi/nhận*/
    Spi_GroupId_Type HwId;
} Spi_JobRuntimeAsyncType;

static Spi_JobRuntimeAsyncType JobRuntime;

Std_ReturnType Spi_RegisterSeqNoti(const Spi_AsyncSeqCfgType *AsyncSeqCfgPtr, Spi_JobId_Type JobId, Spi_SequenceId_Type SeqId)
{

    /*truy xuất các thông tin cấu hình callback */
    Spi_IrqSourceType flag = AsyncSeqCfgPtr->JobAsyncArray[JobId].flag;
    Spi_notificationType SeqNoti = AsyncSeqCfgPtr->SeqNoti;
    Spi_notificationType JobNoti = AsyncSeqCfgPtr->JobAsyncArray[JobId].JobNoti;
    if (SeqId < SPI_SEQ_MAX && JobId < SPI_JOB_MAX && AsyncSeqCfgPtr != NULL_PTR)
    {
        /*đăng ký callback cho sequence Id tương ứng*/
        s_NotiHandlerRuntime[SeqId].SeqNoti = SeqNoti;
        s_NotiHandlerRuntime[SeqId].JobAsyncArray[JobId].JobNoti = JobNoti;
        s_NotiHandlerRuntime[SeqId].JobAsyncArray[JobId].flag = flag;
        return E_OK;
    }
    return E_NOT_OK;
}

void Spi_InternalStartJob(Spi_JobId_Type JobId, Spi_SequenceId_Type SeqId)
{

    // Khởi tạo trạng thái ban đầu cho runtime memory quản lý cho 1 Job tại 1 thời điểm
    JobRuntime.CurrentActiveJobId = JobId; // gán Job đầu tiên
    JobRuntime.ChannelIndex = 0;           // khởi tạo vị trí của channel đầu tiên sẽ xử lý
    JobRuntime.ByteIndex = 0;              // reset vị trí byte ban đầu của buffer
    JobRuntime.ActiveTxPtr = NULL_PTR;     // mặc định chưa có dữ liệu truyền
    JobRuntime.ActiveRxPtr = NULL_PTR;     // mặc định chưa có dữ liệu nhận
    JobRuntime.Length = 0;

    /*truy xuất driver hardware Id tương ứng*/
    Spi_GroupId_Type HwId = Spi_Jobs[JobId].HwId;
    JobRuntime.HwId = HwId;

    /*truy xuất địa chỉ thực tế của Driver Spi và cờ ngắt cẩn chờ*/
    SPI_TypeDef *Spix = Spi_Map_GetHwInstance(HwId);
    Spi_IrqSourceType source = s_NotiHandlerRuntime[SeqId].JobAsyncArray[JobId].flag;
    uint16 It_flag = Spi_MapIrqSourceToSplFlag(source);

    /*kéo CS xuống Low --> cho phép truyền/nhận*/
    Dio_WriteChannel(Spi_Jobs[JobId].CsPinId, STD_LOW);

    /*xử lý lấy dữ liệu từ nguồn EB hay IB*/
    Spi_ChannelId_Type ChId = Spi_Jobs[JobId].ChannelList[JobRuntime.ChannelIndex];
    Spi_BufferType Buftype = Spi_ChannelsRuntime[ChId].BufferType;

    if (Buftype == SPI_BUFFER_TYPE_EB)
    {
        JobRuntime.ActiveTxPtr = Spi_ChannelsRuntime[ChId].TxBufferPtr;
        JobRuntime.ActiveRxPtr = Spi_ChannelsRuntime[ChId].TxBufferPtr;
        JobRuntime.Length = Spi_ChannelsRuntime[ChId].DefaultLength;
    }
    else
    {
        JobRuntime.ActiveTxPtr = Spi_IbPool[ChId].TxBuffer;
        JobRuntime.ActiveRxPtr = Spi_IbPool[ChId].RxBuffer;
        JobRuntime.Length = Spi_IbPool[ChId].Length;
    }
    /*lấy byte đầu tiên của channel đầu tiên*/
    uint16 Txbyte = JobRuntime.ActiveTxPtr[JobRuntime.ByteIndex];

    /*đẩy vào thanh ghi phần cứng*/
    SPI_I2S_SendData(Spix, Txbyte);
    /*kích họa ngắt hoặc DMA*/
    SPI_I2S_ITConfig(Spix, It_flag, ENABLE);

    /*kiểm tra có dùng DMA*/
    // todo...
}

void Spi_InternalFinishJob(Spi_JobId_Type JobId, Spi_SequenceId_Type SeqId)
{

    /*nhà chân CS để hoàn tất Job*/
    Dio_WriteChannel(Spi_Jobs[JobId].CsPinId, STD_HIGH);

    /*truy xuất Job callback dựa trên Id*/
    Spi_notificationType currentJobNoti = s_NotiHandlerRuntime[SeqId].JobAsyncArray[JobId].JobNoti;

    /*kiểm tra và gọi callback đã đăng ký*/
    if (currentJobNoti != NULL_PTR)
    {
        currentJobNoti(); // gọi tác vụ của job Id trong danh sách để thực thi
    }

    /*kiểm tra danh sách sequence còn job nào hay không*/
    if (JobId < Spi_Sequences[SeqId].ActiveJobCount)
    {
        /*tiếp tục gọi API xử lý Job kế tiếp */
        JobId++;
        Spi_notificationType nextJobNoti = s_NotiHandlerRuntime[SeqId].JobAsyncArray[JobId].JobNoti;
        Spi_InternalStartJob(JobId, SeqId);
    }
    /*nếu không còn Job nào -> hoàn tất toàn bộ Sequence -> trả về driver trạng thái sẵn sàng*/
    else
    {
        Spi_GroupId_Type HwId = Spi_Jobs[JobId].HwId;
        // Spi_HWUnitStatus[HwId].Status = SPI_IDLE;

        // Gọi Sequence Notification callback nếu có
        if (s_NotiHandlerRuntime[SeqId].SeqNoti != NULL_PTR)
        {
            s_NotiHandlerRuntime[SeqId].SeqNoti();
        }
    }
}

Std_ReturnType Spi_GetJobAndSeqId(Spi_ChannelId_Type currentChannel, Spi_JobId_Type *foundJobId, Spi_SequenceId_Type *foundSeqId)
{
    /*duyệt qua từng sequence*/
    for (uint8 currentSeqId; currentSeqId < SPI_SEQ_MAX; currentSeqId++)
    {
        uint8 Jobs = Spi_Sequences[currentSeqId].ActiveJobCount;
        /*duyet qua danh sách Job của sequence hiện tại*/
        for (uint8 currentJobId = 0; currentJobId < Jobs; currentJobId++)
        {
            /*trỏ tới từng job trong danh sách*/
            Spi_JobId_Type JobId = Spi_Sequences[currentSeqId].JobList[currentJobId];
            uint8 Channels = Spi_Jobs[JobId].ActiveChannelCount; // đếm số lượng channel của job

            for (uint8 ChId = 0; ChId < Channels; ChId++)
            {
                Spi_ChannelId_Type channelFound = Spi_Jobs[JobId].ChannelList[ChId];
                if (currentChannel == channelFound)
                {
                    *foundJobId = currentJobId;
                    *foundSeqId = currentSeqId;
                    return E_OK;
                }
            }
        }
    }
    return E_NOT_OK;
}

void Spi_InitInternalBuffers(void)
{
    for (uint8 chIdx = 0; chIdx < SPI_MAX_IB_CHANNELS; chIdx++)
    {
        // Reset độ dài dữ liệu ban đầu về 0
        Spi_IbPool[chIdx].Length = 0;

        // Xóa sạch dữ liệu trong buffer Tx và Rx
        for (uint16 i = 0; i < SPI_IB_MAX_LENGTH; i++)
        {
            Spi_IbPool[chIdx].TxBuffer[i] = 0x00;
            Spi_IbPool[chIdx].RxBuffer[i] = 0x00;
        }
    }
}

void SPI1_IRQHandler(void)
{

    // 1. Xử lý ngắt truyền trống (TXE): Gửi byte tiếp theo đi
    if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE) != RESET)
    {
        // Logic kiểm tra còn byte để gửi hay không (tương tự đoạn trước ta đã bàn)...
        // Nếu còn, gọi SPI_I2S_SendData();
        // Nếu hết, tạm thời disable ngắt TXE của channel đó.
        SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, DISABLE);
    }

    // 2. Xử lý ngắt nhận dữ liệu (RXNE): Đọc dữ liệu từ ngoại vi về buffer tương ứng
    if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) != RESET)
    {
        uint8 rxData = SPI_I2S_ReceiveData(SPI1);

        // Lưu vào active buffer (có thể là s_bufReadIB hoặc RxBuffer của EB)
        if (JobRuntime.ActiveRxPtr != NULL_PTR)
        {
            // JobRuntime.ActiveRxPtr[JobRuntime.RxByteIndex++] = rxData;
        }
    }

    // Bước 1 : kiểm tra và xử lý data hiện tại (từng byte)

    /*Đọc/ghi data : rẽ nhành và xử lý theo datasize và direction*/

    /*cập nhật vị trí byte hiện tại -> tăng biến đếm trong job runtime*/

    // Bước 2 : kiểm tra luồng channel data đã hoàn tất

    /*so sánh vị trí byte với kích thước buffer của channel*/

    /*tiép tục đọc/ghi byte kế tiếp từ DR cho đến khi hết channel buffer*/

    /*kiểm tra còn channel nào cần xử lý không*/

    /*Nếu còn driver chuyển sang channel kế và reset cấu hình Job runtime*/

    /*lặp lại quy trình đọc ghi và duy trì ngắt*/

    // Bước 3 : Hoàn tát Job hiện tại

    /*nhả chân CS*/

    /*kiểm tra trong sequence còn Job nào chưa làm*/

    /*nếu còn Job gọi lại InternalStartJob()*/

    /*Nếu hết job
    -> gọi InternalFinishJob()
        -> tắt cờ ngắt
            -> gọi callback thông báo hoàn thành
                -> báo cho App
                    -> set SPI_IDLE*/
}
