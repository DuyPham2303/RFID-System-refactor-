#include "Spi_Internal.h"
#include "Spi_Runtime.h"
#include "Spi_Hw.h"
#define DELAY_NSS_PERIOD 1U
/**
 * @brief Hàm delay ước lượng theo microsecond (us).
 * @note  Cần hiệu chỉnh hệ số vòng lặp (LOOP_CYCLES_PER_US) dựa theo tần số xung nhịp CPU thực tế (HCLK).
 * @param us Số microsecond cần trễ.
 */
static void Spi_Hw_DelayUs(uint32 us)
{
    /*
     * Hệ số này phụ thuộc vào tốc độ Clock của MCU (ví dụ: STM32F103 chạy 72MHz).
     * Một vòng lặp for trống thường mất khoảng vài chu kỳ máy (instructions).
     * Bạn có thể tinh chỉnh con số này sau khi đo thực tế trên Logic Analyzer.
     */
    volatile uint32 count;

    while (us > 0U)
    {
        /*
         * Sử dụng giá trị ước lượng cho MCU chạy 72MHz.
         * Từ khóa volatile ép compiler phải thực hiện đủ số vòng lặp,
         * không được tự ý tối ưu bỏ qua đoạn code này.
         */
        for (count = 12U; count > 0U; count--)
        {
            __NOP(); // No Operation - tiêu tốn 1 chu kỳ máy
        }
        us--;
    }
}

/**
 * @brief Xác định trạng thái bận của bus Tx và Rx theo hướng truyền của Device.
 * @param direction Hướng truyền/nhận được cấu hình cho SPI Device.
 * @param Txbusy Con trỏ nhận trạng thái bận của chiều truyền.
 * @param Rxbusy Con trỏ nhận trạng thái bận của chiều nhận.
 *
 * @details
 * 1. Khởi tạo cả hai trạng thái bus là rảnh.
 * 2. Đánh dấu Tx bận nếu cấu hình có truyền dữ liệu.
 * 3. Đánh dấu Rx bận nếu cấu hình có nhận dữ liệu.
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
 * @brief Chuẩn bị buffer runtime và xác định số phần tử cần truyền cho Channel.
 * @param ChId ID của Channel cần chuẩn bị.
 * @param channelPtr Runtime của Channel.
 * @param totalBytes Con trỏ nhận tổng số phần tử cần truyền/nhận.
 * @return E_OK nếu lấy được thông tin buffer; ngược lại trả E_NOT_OK.
 *
 * @details
 * 1. Với EB, lấy độ dài đã cấu hình trong runtime Channel.
 * 2. Với IB:
 *    2.1. Lấy vùng đệm nội bộ theo ChId.
 *    2.2. Nếu không có vùng đệm, dừng và trả E_NOT_OK.
 *    2.3. Gán buffer Tx/Rx nội bộ vào runtime Channel và lấy độ dài.
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

/**
 * @brief Khởi tạo và thực thi một Job thuộc Sequence.
 * @param JobId ID của Job cần chạy.
 * @param SeqId ID của Sequence chứa Job.
 * @return E_OK nếu Job được khởi chạy/hoàn tất thành công; E_NOT_OK nếu không
 *         lấy được runtime hoặc Hardware Unit chưa rảnh.
 *
 * @details
 * 1. Lấy runtime Job và Sequence; từ chối nếu không hợp lệ.
 * 2. Lấy Hardware Unit của Job và xác nhận phần cứng đang ở trạng thái SPI_IDLE.
 * 3. Đọc Direction và DataSize của Device tương ứng với Hardware Unit.
 * 4. Khởi tạo trạng thái thực thi:
 *    4.1. Cập nhật cờ Tx/Rx bận theo Direction.
 *    4.2. Reset chỉ số Channel, ghi nhận Job đang chạy và đánh dấu Job bận.
 *    4.3. Kéo CS xuống LOW, lưu CS đang dùng và đánh dấu Hardware Unit bận.
 * 5. Nếu Sequence chạy ở chế độ polling:
 *    5.1. Lần lượt lấy từng Channel trong Job.
 *    5.2. Chuẩn bị buffer và độ dài của Channel.
 *    5.3. Gọi truyền/nhận đồng bộ; sau cùng chuyển sang xử lý kết thúc Job.
 * 6. Nếu Sequence chạy ở chế độ interrupt:
 *    6.1. Lấy runtime context và lưu thông tin Job/Sequence, Direction,
 *         DataSize cùng chỉ số dữ liệu ban đầu để ISR tiếp tục xử lý.
 *    6.2. Xác nhận Channel đầu tiên hợp lệ.
 *    6.3. Bật ngắt TXE và RXNE trên Hardware Unit để bắt đầu truyền nhận.
 */
Std_ReturnType Spi_InternalStartJob(
    Spi_JobType_e JobId,
    Spi_SequenceType_e SeqId)
{
    /* 1. Lấy runtime Job và Sequence để cập nhật trạng thái thực thi. */
    Spi_JobRuntimeType_s *JobRuntime = Spi_Runtime_GetJob(JobId);
    Spi_SequenceRuntimeType_s *currentSeqPtr = Spi_Runtime_GetSequence(SeqId);

    if (JobRuntime == NULL_PTR || currentSeqPtr == NULL_PTR)
    {
        return E_NOT_OK;
    }

    /* 2. Xác nhận Hardware Unit của Job đang rảnh trước khi sử dụng. */
    Spi_HwUnitType_e HwId = g_Spi_ConfigSet.JobConfigPtr[JobId].HwId;
    if (Spi_Runtime_GetHwGroupStatus(HwId) != SPI_IDLE)
    {
        return E_NOT_OK;
    }

    Spi_DirectionType direction;
    Spi_DataSizeType datasize;

    /* 3. Lấy Direction và DataSize của Device ánh xạ tới Hardware Unit. */
    for (uint8 index = 0; index < g_Spi_ConfigSet.DeviceCount; index++)
    {
        if (HwId == g_Spi_ConfigSet.DeviceConfigPtr[index].HwId)
        {
            direction = g_Spi_ConfigSet.DeviceConfigPtr[index].Dir;
            datasize = g_Spi_ConfigSet.DeviceConfigPtr[index].DataSize;
        }
    }
    boolean Txbusy, Rxbusy;

    /* 4. Chuẩn bị trạng thái ban đầu của Job và bus. */
    /* 4.1. Xác định các chiều Tx/Rx mà Job sẽ sử dụng. */
    Spi_GetDirectionBusy(direction, &Txbusy, &Rxbusy);

    /* 4.2. Đặt Job về Channel đầu tiên và đánh dấu Job/bus bận. */
    JobRuntime->ChannelIndex = 0;
    JobRuntime->CurrentActiveJobId = JobId;
    JobRuntime->IsBusy = TRUE;
    Spi_Runtime_SetBusBusy(HwId, Txbusy, Rxbusy);

    /* 4.3. Chọn thiết bị ngoại vi bằng cách kéo CS xuống LOW. */
    Dio_ChannelType currentCsPin = g_Spi_ConfigSet.JobConfigPtr[JobId].CsPinId;
    Dio_WriteChannel(currentCsPin, STD_LOW);
    Spi_Hw_DelayUs(DELAY_NSS_PERIOD);
    currentSeqPtr->ActiveCsPin = currentCsPin;

    /* 4.4. Đánh dấu Hardware Unit đang được Job này sử dụng. */
    Spi_Runtime_SetHwGroupStatus(HwId, SPI_BUSY);

    uint8 channelTotal = g_Spi_ConfigSet.JobConfigPtr[JobId].ActiveTotalChIncurrentJob;
    if (currentSeqPtr->Mode == SPI_POLLING_MODE)
    {
        /* 5. Chế độ polling: xử lý tuần tự và chờ truyền/nhận hoàn tất. */
        for (uint8 currentCh = 0; currentCh < channelTotal; currentCh++)
        {
            JobRuntime->ChannelIndex = currentCh;
            Spi_ChannelType_e ChId = g_Spi_ConfigSet.JobConfigPtr[JobId].ChannelList[currentCh];
            Spi_ChannelRuntimeType_s *currentChannelPtr = Spi_Runtime_GetChannel(ChId);

            /* 5.1. Chuẩn bị buffer và độ dài cho Channel hiện tại. */
            if (Spi_InternalSetupChannelBuffer(ChId, currentChannelPtr, &JobRuntime->TotalBytesInCurrentChannel) != E_OK)
            {
                return E_NOT_OK;
            }

            /* 5.2. Truyền/nhận đồng bộ toàn bộ dữ liệu của Channel. */
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
        /* 6. Chế độ interrupt: lưu trạng thái để ISR tiếp tục xử lý bất đồng bộ. */
        Spi_HwUnitRuntimeType_s *ContextSavedPtr = Spi_Runtime_GetHwUnit();
        if (ContextSavedPtr == NULL_PTR)
        {
            return E_NOT_OK;
        }

        /* 6.1. Lưu cấu hình và vị trí ban đầu mà ISR cần để xử lý Job. */
        ContextSavedPtr->JobCfg = &g_Spi_ConfigSet.JobConfigPtr[JobId];
        ContextSavedPtr->ActiveSeqId = SeqId;
        ContextSavedPtr->ActiveJobId = JobId;
        ContextSavedPtr->direction = direction;
        ContextSavedPtr->datasize = datasize;
        ContextSavedPtr->ByteIndex = 0;

        /* 6.2. Kiểm tra runtime của Channel đầu tiên trước khi bật ngắt. */
        Spi_ChannelType_e ChId = g_Spi_ConfigSet.JobConfigPtr[JobId].ChannelList[0];
        Spi_ChannelRuntimeType_s *currentChannelPtr = Spi_Runtime_GetChannel(ChId);
        if (currentChannelPtr == NULL_PTR)
        {
            return E_NOT_OK;
        }
        SPI_TypeDef *Spix = Spi_Map_GetHwInstance(HwId);

        /* 6.3. Bật ngắt TXE/RXNE để ISR xử lý lần lượt dữ liệu Job. */
        SPI_I2S_ITConfig(Spix, SPI_I2S_IT_TXE | SPI_I2S_IT_RXNE, ENABLE);
        return E_OK;
    }
}

/**
 * @brief Hoàn tất Job hiện tại và tiếp tục hoặc kết thúc Sequence chứa Job.
 * @param JobId ID của Job vừa hoàn tất.
 * @param SeqId ID của Sequence chứa Job.
 * @return E_OK nếu hoàn tất hoặc khởi chạy Job kế tiếp thành công;
 *         E_NOT_OK nếu không lấy được runtime cần thiết.
 *
 * @details
 * 1. Lấy số Job của Sequence cùng runtime Job/Sequence; dừng nếu runtime không hợp lệ.
 * 2. Đánh dấu Job hiện tại không còn bận và tăng chỉ số Job của Sequence.
 * 3. Nếu Sequence còn Job:
 *    3.1. Lấy Job kế tiếp cùng CS và Hardware Unit của Job đó.
 *    3.2. Nhả CS hiện tại nếu Job kế tiếp dùng CS khác.
 *    3.3. Đặt Hardware Unit vừa dùng về SPI_IDLE trước khi bắt đầu Job kế tiếp,
 *         kể cả khi Job kế tiếp dùng lại cùng Hardware Unit.
 *    3.4. Xóa cờ bận Tx/Rx của Hardware Unit vừa dùng và khởi chạy Job kế tiếp.
 * 4. Nếu đây là Job cuối:
 *    4.1. Reset chỉ số Job và đặt Sequence về SPI_IDLE.
 *    4.2. Nhả CS cuối cùng và đặt Hardware Unit/bus về trạng thái rảnh.
 *    4.3. Gọi callback của Sequence nếu callback đã được đăng ký.
 */
Std_ReturnType Spi_InternalFinishJob(
    Spi_JobType_e JobId,
    Spi_SequenceType_e SeqId)
{
    /* 1. Đọc số Job được cấu hình trong Sequence và lấy các runtime tương ứng. */
    uint8 SequenceTotal = g_Spi_ConfigSet.SequenceConfigPtr[SeqId].TotalJobIncurrentSequence;

    Spi_JobRuntimeType_s *JobRuntime = Spi_Runtime_GetJob(JobId);
    Spi_SequenceRuntimeType_s *currentSeqPtr = Spi_Runtime_GetSequence(SeqId);

    if (JobRuntime == NULL_PTR || currentSeqPtr == NULL_PTR)
    {
        return E_NOT_OK;
    }
    /* 2. Hoàn tất trạng thái Job hiện tại và tiến tới vị trí kế tiếp. */
    Spi_HwUnitType_e HwId = g_Spi_ConfigSet.JobConfigPtr[JobId].HwId;

    JobRuntime->IsBusy = FALSE;

    uint8 nextJobIndex = ++currentSeqPtr->CurrentJobIndex;

    if (nextJobIndex < SequenceTotal)
    {
        /* 3. Sequence còn Job cần chạy: chuẩn bị chuyển sang Job tiếp theo. */
        Spi_JobType_e nextJobId = g_Spi_ConfigSet.SequenceConfigPtr[SeqId].JobList[nextJobIndex];

        /* 3.1. Lấy chân CS được cấu hình cho Job tiếp theo. */
        Dio_ChannelType nextCsPin = g_Spi_ConfigSet.JobConfigPtr[nextJobId].CsPinId;

        /* 3.2. Nhả CS cũ nếu thiết bị kế tiếp sử dụng một chân CS khác. */
        if (nextCsPin != currentSeqPtr->ActiveCsPin)
        {
            Dio_WriteChannel(currentSeqPtr->ActiveCsPin, STD_HIGH);
            Spi_Hw_DelayUs(DELAY_NSS_PERIOD);
        }

        /* 3.3. Giải phóng Hardware Unit hiện tại trước khi bắt đầu Job kế tiếp.
         * Cần thiết cả khi hai Job liên tiếp dùng cùng Hardware Unit vì
         * Spi_InternalStartJob chỉ nhận Hardware Unit ở trạng thái SPI_IDLE.
         */
        Spi_Runtime_SetHwGroupStatus(HwId, SPI_IDLE);
        Spi_Runtime_SetBusBusy(HwId, FALSE, FALSE);
        /* 3.4. Khởi chạy Job kế tiếp; StartJob sẽ đánh dấu phần cứng bận lại. */
        return Spi_InternalStartJob(nextJobId, SeqId);
    }
    else
    {
        /* 4. Sequence đã hoàn tất: giải phóng tài nguyên và thông báo kết quả. */
        /* 4.1. Reset vị trí Job và chuyển Sequence về SPI_IDLE. */
        currentSeqPtr->CurrentJobIndex = 0;
        currentSeqPtr->Status = SPI_IDLE;

        /* 4.2. Nhả CS cuối cùng và đặt Hardware Unit cùng bus về trạng thái rảnh. */
        Dio_WriteChannel(currentSeqPtr->ActiveCsPin, STD_HIGH);
        Spi_Hw_DelayUs(DELAY_NSS_PERIOD);

        Spi_Runtime_SetHwGroupStatus(HwId, SPI_IDLE);
        Spi_Runtime_SetBusBusy(HwId, FALSE, FALSE);

        /* 4.3. Thông báo hoàn tất Sequence nếu có callback được cấu hình. */
        Spi_notificationType currentSeqNoti = g_Spi_ConfigSet.SequenceConfigPtr[SeqId].SeqNoti;
        if (currentSeqNoti != NULL_PTR)
        {
            currentSeqNoti();
        }
        return E_OK;
    }
}
