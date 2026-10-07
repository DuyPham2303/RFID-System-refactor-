#include "Spi_Internal.h"
#include "Spi_Runtime.h"
#include "Spi_Hw.h"

/**
 * @brief Hàm nội bộ phụ trợ xác định trạng thái bận Tx/Rx dựa theo Direction
 */
static void Spi_GetDirectionBusy(Spi_DirectionType direction, boolean *Txbusy, boolean *Rxbusy)
{
    *Txbusy = FALSE;
    *Rxbusy = FALSE;

    if (direction == SPI_MR_1LINE_TX || direction == SPI_MR_2LINES_FD)
    {
        *Txbusy = TRUE;
    }
    if (direction == SPI_MR_1LINE_RX || direction == SPI_MR_2LINES_FD || direction == SPI_MR_2LINES_RX_ONLY)
    {
        *Rxbusy = TRUE;
    }
}

/**
 * @brief Hàm nội bộ phụ trợ cấu hình nguồn buffer cho Channel (Hỗ trợ cả EB và IB)
 */
static Std_ReturnType Spi_InternalSetupChannelBuffer(
    Spi_ChannelType_e ChId,
    Spi_ChannelRuntimeType_s *channelPtr,
    uint16 *totalBytes)
{
    if (channelPtr->BufferType == SPI_BUFFER_TYPE_EB)
    {
        *totalBytes = channelPtr->DefaultLength;
    }
    else
    {
        Spi_IbChannelType_s *CurrentIbPool = Spi_Runtime_GetIbPool(ChId);
        if (CurrentIbPool == NULL_PTR)
        {
            return E_NOT_OK;
        }
        channelPtr->ActiveTxPtr = CurrentIbPool->TxBuffer;
        channelPtr->ActiveRxPtr = CurrentIbPool->RxBuffer;
        *totalBytes = CurrentIbPool->Length;
    }
    return E_OK;
}

Std_ReturnType Spi_InternalStartJob(
    Spi_JobType_e JobId,
    Spi_SequenceType_e SeqId)
{
    /*truy xuất runtime job và sequence */
    Spi_JobRuntimeType_s *JobRuntime = Spi_Runtime_GetJob(JobId);
    Spi_SequenceRuntimeType_s *currentSeqPtr = Spi_Runtime_GetSequence(SeqId);

    if (JobRuntime == NULL_PTR || currentSeqPtr == NULL_PTR)
    {
        return E_NOT_OK;
    }

    // truy xuất thông tin cấu hình cần thiết
    Spi_HwUnitType_e HwId = g_Spi_ConfigSet.JobConfigPtr[JobId].HwId;
    if (Spi_Runtime_GetHwGroupStatus(HwId) != SPI_IDLE)
    {
        return E_NOT_OK;
    }
    // truy xuất thông tin cấu hình cần thiết
    Spi_DirectionType direction = g_Spi_ConfigSet.DeviceConfigPtr[HwId].Dir;
    Spi_DataSizeType datasize = g_Spi_ConfigSet.DeviceConfigPtr[HwId].DataSize;
    boolean Txbusy, Rxbusy;

    // 1. Xác định cờ bận theo hướng truyền
    Spi_GetDirectionBusy(direction, &Txbusy, &Rxbusy);

    // 2. Thiết lập trạng thái ban đầu cho Job & Bus
    JobRuntime->ChannelIndex = 0;
    JobRuntime->CurrentActiveJobId = JobId;
    JobRuntime->IsBusy = TRUE;
    Spi_Runtime_SetBusBusy(HwId, Txbusy, Rxbusy);

    // 3. Điều khiển chân Chip Select (CS)
    Dio_ChannelType currentCsPin = g_Spi_ConfigSet.JobConfigPtr[JobId].CsPinId;
    Dio_WriteChannel(currentCsPin, STD_LOW);
    currentSeqPtr->ActiveCsPin = currentCsPin;

    // 4. thông báo phần cứng đang bận
    Spi_Runtime_SetHwGroupStatus(HwId, SPI_BUSY);

    uint8 channelTotal = g_Spi_ConfigSet.JobConfigPtr[JobId].ActiveTotalChIncurrentJob;
    if (currentSeqPtr->Mode == SPI_POLLING_MODE)
    {
        // --- XỬ LÝ CHẾ ĐỘ POLLING (BLOCKING) ---
        for (uint8 currentCh = 0; currentCh < channelTotal; currentCh++)
        {
            JobRuntime->ChannelIndex = currentCh;
            Spi_ChannelType_e ChId = g_Spi_ConfigSet.JobConfigPtr[JobId].ChannelList[currentCh];
            Spi_ChannelRuntimeType_s *currentChannelPtr = Spi_Runtime_GetChannel(ChId);

            /*xác định nguồn buffer*/
            if (Spi_InternalSetupChannelBuffer(ChId, currentChannelPtr, &JobRuntime->TotalBytesInCurrentChannel) != E_OK)
            {
                return E_NOT_OK;
            }

            // Xử lý truyền/nhận lần lượt các channel data
            Spi_Hw_Sync_TransmitReceive(currentChannelPtr->ActiveTxPtr,
                                        currentChannelPtr->ActiveRxPtr,
                                        JobRuntime->TotalBytesInCurrentChannel,
                                        HwId,
                                        direction,
                                        datasize);
        }
        return Spi_InternalFinishJob(JobId, SeqId);
    }
    else
    {

        // --- XỬ LÝ CHẾ ĐỘ INTERRUPT (NON-BLOCKING) ---

        // truy xuất con trỏ quản lý ngữ cảnh ISR
        Spi_HwUnitRuntimeType_s *ContextSavedPtr = Spi_Runtime_GetHwUnit();
        if (ContextSavedPtr == NULL_PTR)
        {
            return E_NOT_OK;
        }

        // Lưu ngữ cảnh phục vụ cho ISR
        ContextSavedPtr->JobCfg = &g_Spi_ConfigSet.JobConfigPtr[JobId];
        ContextSavedPtr->ActiveSeqId = SeqId;
        ContextSavedPtr->ActiveJobId = JobId;
        ContextSavedPtr->direction = direction;
        ContextSavedPtr->datasize = datasize;
        ContextSavedPtr->ByteIndex = 0;

        Spi_ChannelType_e ChId = g_Spi_ConfigSet.JobConfigPtr[JobId].ChannelList[0];
        Spi_ChannelRuntimeType_s *currentChannelPtr = Spi_Runtime_GetChannel(ChId);
        if (currentChannelPtr == NULL_PTR)
        {
            return E_NOT_OK;
        }
        /*xác định nguồn buffer*/
        if (Spi_InternalSetupChannelBuffer(ChId, currentChannelPtr, &JobRuntime->TotalBytesInCurrentChannel) != E_OK)
        {
            return E_NOT_OK;
        }

        SPI_TypeDef *Spix = Spi_Map_GetHwInstance(HwId);
        // tiến hành nạp byte đầu tiên để phán cờ bát của bộ đệm trước khi kích hoạt ngắt kiểm tra bộ đệm
        Spi_Hw_StartFirstByteInterrupt(Spix,
                                       currentChannelPtr->ActiveTxPtr,
                                       currentChannelPtr->ActiveRxPtr,
                                       direction,
                                       datasize);
        // cập nhật đã xử lý nạp byte đầu hoàn thành, và giao lại cho ISR tiếp tục xử lý
        ContextSavedPtr->ByteIndex++;

        // mặc định kích hoạt ngắt 2 chiều
        SPI_I2S_ITConfig(Spix, SPI_I2S_IT_TXE | SPI_I2S_IT_RXNE, ENABLE);
        return E_OK;
    }
    return Spi_InternalFinishJob(JobId, SeqId);
}

Std_ReturnType Spi_InternalFinishJob(
    Spi_JobType_e JobId,
    Spi_SequenceType_e SeqId)
{
    // Truy xuất thông tin cấu hình tĩnh của Sequence
    uint8 SequenceTotal = g_Spi_ConfigSet.SequenceConfigPtr[SeqId].TotalJobIncurrentSequence;

    /*truy xuất runtime job và sequence */
    Spi_JobRuntimeType_s *JobRuntime = Spi_Runtime_GetJob(JobId);
    Spi_SequenceRuntimeType_s *currentSeqPtr = Spi_Runtime_GetSequence(SeqId);

    if (JobRuntime == NULL_PTR || currentSeqPtr == NULL_PTR)
    {
        return E_NOT_OK;
    }
    // truy xuất Id phần cứng hiện tại
    Spi_HwUnitType_e HwId = g_Spi_ConfigSet.JobConfigPtr[JobId].HwId;

    // Hoàn tất công việc của Job này
    JobRuntime->IsBusy = FALSE;

    // Chuyển sang Job tiếp theo trong danh sách
    uint8 nextJobIndex = ++currentSeqPtr->CurrentJobIndex;

    // Kiểm tra xem đã vượt quá tổng số Job của Sequence này chưa
    if (nextJobIndex < SequenceTotal)
    {
        // --- TRƯỜNG HỢP VẪN CÒN JOB CHƯA CHẠY ---

        // Lấy JobId tiếp theo từ mảng JobList của Sequence
        Spi_JobType_e nextJobId = g_Spi_ConfigSet.SequenceConfigPtr[SeqId].JobList[nextJobIndex];

        // truy xuất Chân Cs và Id phần cứng kế tiếp
        Dio_ChannelType nextCsPin = g_Spi_ConfigSet.JobConfigPtr[nextJobId].CsPinId;
        Spi_HwUnitType_e nextHwUnit = g_Spi_ConfigSet.JobConfigPtr[nextJobId].HwId;

        // nhả chân CS cũ lên HIGH trước khi sang job mới.
        if (nextCsPin != currentSeqPtr->ActiveCsPin)
        {
            Dio_WriteChannel(currentSeqPtr->ActiveCsPin, STD_HIGH);
        }

        if (HwId != nextHwUnit)
        {
            Spi_Runtime_SetHwGroupStatus(HwId, SPI_IDLE);
        }

        Spi_Runtime_SetBusBusy(HwId, FALSE, FALSE);
        // Gọi lại InternalStartJob để chạy Job tiếp theo trong cùng Sequence đó
        return Spi_InternalStartJob(nextJobId, SeqId);
    }
    else
    {
        // --- TRƯỜNG HỢP ĐÃ CHẠY HẾT TẤT CẢ JOB TRONG SEQUENCE ---

        // 1. Reset lại chỉ số cho lần gọi Sequence sau này (nếu cần)
        currentSeqPtr->CurrentJobIndex = 0;

        // 2. Cập nhật trạng thái Sequence thành hoàn tất (IDLE)
        currentSeqPtr->Status = SPI_IDLE;

        // 3. Nhả chân CS cuối cùng đang bị giữ mức LOW chính thức kết thúc phiên truyền của Sequence
        Dio_WriteChannel(currentSeqPtr->ActiveCsPin, STD_HIGH);

        // 4. Cập nhật trạng thái phần cứng SPI tổ trở về IDLE
        Spi_Runtime_SetHwGroupStatus(HwId, SPI_IDLE);
        Spi_Runtime_SetBusBusy(HwId, FALSE, FALSE);

        Spi_notificationType currentSeqNoti = g_Spi_ConfigSet.SequenceConfigPtr[SeqId].SeqNoti;
        // 5. Kích hoạt Callback bất đồng bộ (nếu có đăng ký) hoặc báo hiệu hoàn thành đồng bộ
        if (currentSeqNoti != NULL_PTR)
        {
            currentSeqNoti();
        }
        return E_OK;
    }
}
