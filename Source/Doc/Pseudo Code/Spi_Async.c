/* ==========================================================
 * 1. TẦNG ỨNG DỤNG (APPLICATION LAYER)
 * ========================================================== */
void App_Task_Example(void)
{
    // Bước 1: Khởi tạo driver (thường gọi lúc boot hệ thống)
    Spi_Init(&g_Spi_ConfigSet);

    // Bước 2: Nếu kênh dùng External Buffer (EB), cấu hình con trỏ buffer
    Spi_SetupEB(SPI_CH_RFID, myTxBuf, myRxBuf, 64);

    // Bước 3: Kích hoạt truyền bất đồng bộ (Sequence chứa Job đọc RFID)
    Std_ReturnType result = Spi_AsyncTransmit(SPI_SEQ_READ_RFID);
    if (result == E_OK)
    {
        // Giao dịch đã được bắt đầu ngầm, App có thể đi làm việc khác
    }
}

/* ==========================================================
 * 2. TẦNG PUBLIC API (Spi.c)
 * ========================================================== */
Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType_e Sequence)
{
    // Kiểm tra driver có đang rảnh không
    if (s_HwUnitRuntimeStatus != SPI_IDLE)
    {
        return E_NOT_OK; // Driver đang bận
    }

    // Đánh dấu driver chuyển sang trạng thái bận
    s_HwUnitRuntimeStatus = SPI_BUSY;

    // Lưu lại Sequence đang chạy
    CurrentActiveSequence = Sequence;
    CurrentJobIndex = 0;

    // Lấy Job đầu tiên thuộc Sequence này
    Spi_JobType_e firstJob = Spi_Sequences[Sequence].JobList[CurrentJobIndex];

    // Gọi hàm nội bộ để bắt đầu thực thi Job đầu tiên
    Spi_InternalStartJob(firstJob);

    return E_OK; // Trả về ngay lập tức (Non-blocking)
}

/* ==========================================================
 * 3. TẦNG NỘI BỘ & ĐIỀU PHỐI (Spi_Internal.c)
 * ========================================================== */

// Hàm bắt đầu thực thi một Job
void Spi_InternalStartJob(Spi_JobType_e Job)
{
    // 1. Kéo chân Chip Select (CS) của Job này xuống mức thấp (Active Low)
    GPIO_WritePin(Spi_Jobs[Job].CsPinId, GPIO_PIN_RESET);

    // 2. Cấu hình phần cứng SPI hoặc bật ngắt/DMA dựa vào Channel thuộc Job này
    // (Xác định lấy dữ liệu từ IB hay EB tùy theo cấu hình tĩnh của Channel)

    // 3. Kích hoạt ngắt phần cứng (Ví dụ: bật TXE interrupt để truyền byte đầu tiên)
    SPI_EnableInterrupt(SPI_PERIPHERAL_1, SPI_IT_TXE);
}

// Trình phục vụ ngắt phần cứng (Hardware Interrupt Service Routine)
void SPI1_IRQHandler(void)
{
    // Khi phần cứng truyền xong một byte / khung dữ liệu
    if (SPI_GetITStatus(SPI_PERIPHERAL_1, SPI_IT_TXE) != RESET)
    {
        // Truyền các byte tiếp theo hoặc nhận dữ liệu...

        // GIẢ SỬ: Job hiện tại đã truyền xong toàn bộ các Channel bên trong nó
        if (IsCurrentJobCompleted() == TRUE)
        {
            // Tắt ngắt tạm thời để xử lý hậu kỳ Job
            SPI_DisableInterrupt(SPI_PERIPHERAL_1, SPI_IT_TXE);

            // Gọi hàm kết thúc Job nội bộ
            Spi_InternalFinishJob(ActiveJobId);
        }
    }
}

// Hàm hoàn tất nội bộ một Job
void Spi_InternalFinishJob(Spi_JobType_e Job)
{
    // 1. Nhả chân Chip Select (CS) lên mức cao (Deactive)
    GPIO_WritePin(Spi_Jobs[Job].CsPinId, GPIO_PIN_SET);

    // 2. Gọi Job Notification callback nếu người dùng có đăng ký
    if (Spi_JobCallbacks[Job] != NULL_PTR)
    {
        Spi_JobCallbacks[Job]();
    }

    // 3. Kiểm tra xem Sequence hiện tại còn Job nào tiếp theo không?
    CurrentJobIndex++;
    if (CurrentJobIndex < Spi_Sequences[CurrentActiveSequence].ActiveJobCount)
    {
        // Vẫn còn Job -> Lấy Job tiếp theo và chạy tiếp
        Spi_JobType_e nextJob = Spi_Sequences[CurrentActiveSequence].JobList[CurrentJobIndex];
        Spi_InternalStartJob(nextJob);
    }
    else
    {
        // Đã hết Job -> Hoàn tất toàn bộ Sequence
        s_HwUnitRuntimeStatus = SPI_IDLE; // Trả driver về trạng thái sẵn sàng

        // Gọi Sequence Notification callback nếu có
        if (Spi_SequenceCallbacks[CurrentActiveSequence] != NULL_PTR)
        {
            Spi_SequenceCallbacks[CurrentActiveSequence]();
        }
    }
}