#include "Spi_Hw.h"
#include "Spi_Runtime.h"
#include "Spi_Internal.h"

static void Spi_Hw_IrqHandler(SPI_TypeDef *HwUnit)
{
    if (HwUnit == NULL_PTR)
    {
        return;
    }

    // 1. Truy xuất thông tin ngữ cảnh phần cứng SPI
    Spi_HwUnitRuntimeType_s *ContextSavedPtr = Spi_Runtime_GetHwUnit();
    if (ContextSavedPtr == NULL_PTR)
    {
        return;
    }
    Spi_JobType_e currentJobId = ContextSavedPtr->ActiveJobId;
    Spi_JobRuntimeType_s *JobRuntime = Spi_Runtime_GetJob(currentJobId);
    const Spi_JobConfigType_s *JobConfig = ContextSavedPtr->JobCfg;

    if (JobConfig == NULL_PTR)
    {
        SPI_I2S_ITConfig(HwUnit, SPI_I2S_IT_TXE | SPI_I2S_IT_RXNE, DISABLE);
        return; // Lỗi an toàn
    }

    if (Spi_Map_GetHwInstance(JobConfig->HwId) != HwUnit)
    {
        return;
    }

    if (JobRuntime == NULL_PTR)
    {
        SPI_I2S_ITConfig(HwUnit, SPI_I2S_IT_TXE | SPI_I2S_IT_RXNE, DISABLE);
        return; // Lỗi an toàn
    }

    // 2. Kiểm tra xem Job đã hoàn thành tất cả các Channel chưa
    uint8 TotalCh = JobConfig->ActiveTotalChIncurrentJob;
    uint8 ActiveChIndex = JobRuntime->ChannelIndex;
    if (ActiveChIndex >= TotalCh)
    {
        SPI_I2S_ITConfig(HwUnit, SPI_I2S_IT_TXE | SPI_I2S_IT_RXNE, DISABLE);
        Spi_InternalFinishJob(currentJobId, ContextSavedPtr->ActiveSeqId);
        return;
    }

    // 3. Lấy thông tin Channel hiện tại
    Spi_ChannelType_e chId = JobConfig->ChannelList[ActiveChIndex];
    Spi_ChannelRuntimeType_s *chPtr = Spi_Runtime_GetChannel(chId);
    if (chPtr == NULL_PTR)
    {
        SPI_I2S_ITConfig(HwUnit, SPI_I2S_IT_TXE | SPI_I2S_IT_RXNE, DISABLE);
        return;
    }

    // 4. Nếu đây là byte đầu tiên của Channel, thực hiện cấu hình nguồn Buffer (Chỉ chạy 1 lần duy nhất mỗi đầu channel)
    if (ContextSavedPtr->ByteIndex == 0)
    {
        if (chPtr->BufferType == SPI_BUFFER_TYPE_EB)
        {
            JobRuntime->TotalBytesInCurrentChannel = chPtr->DefaultLength;
            // Đảm bảo con trỏ EB đã được cấu hình từ trước qua Spi_SetupEB
        }
        else
        {
            Spi_IbChannelType_s *currentIbPool = Spi_Runtime_GetIbPool(chId);
            if (currentIbPool == NULL_PTR)
            {
                SPI_I2S_ITConfig(HwUnit, SPI_I2S_IT_TXE | SPI_I2S_IT_RXNE, DISABLE);
                return;
            }
            chPtr->ActiveTxPtr = currentIbPool->TxBuffer;
            chPtr->ActiveRxPtr = currentIbPool->RxBuffer;
            JobRuntime->TotalBytesInCurrentChannel = currentIbPool->Length;
        }
    }

    /*Truy xuất tham số phục vụ mô tả cách thức truyền/nhận */
    uint8 byteIndex = ContextSavedPtr->ByteIndex;
    uint16 length = JobRuntime->TotalBytesInCurrentChannel;
    Spi_DirectionType direction = ContextSavedPtr->direction;
    Spi_DataSizeType datasize = ContextSavedPtr->datasize;

    // 5. Xử lý ngắt nhận dữ liệu (RXNE) trước để hứng dữ liệu đến
    if (SPI_I2S_GetITStatus(HwUnit, SPI_I2S_IT_RXNE) != RESET)
    {
        uint16 rxData = SPI_I2S_ReceiveData(HwUnit);
        if (chPtr->ActiveRxPtr != NULL_PTR)
        {
            if (datasize == SPI_MR_DATASIZE_16B)
            {
                ((uint16_t *)chPtr->ActiveRxPtr)[byteIndex] = rxData;
            }
            else
            {
                ((uint8_t *)chPtr->ActiveRxPtr)[byteIndex] = (uint8_t)rxData;
            }
        }
    }

    // 6. Xử lý ngắt truyền dữ liệu đi (TXE)
    if (SPI_I2S_GetITStatus(HwUnit, SPI_I2S_IT_TXE) != RESET)
    {
        if (byteIndex < length)
        {
            uint16 txVal = 0xFFFFU;
            if (direction != SPI_MR_2LINES_RX_ONLY && chPtr->ActiveTxPtr != NULL_PTR)
            {
                if (datasize == SPI_MR_DATASIZE_16B)
                {
                    txVal = ((uint16_t *)chPtr->ActiveTxPtr)[byteIndex];
                }
                else
                {
                    txVal = ((uint8_t *)chPtr->ActiveTxPtr)[byteIndex];
                }
            }

            // Gửi dữ liệu qua phần cứng SPL
            if (datasize == SPI_MR_DATASIZE_16B)
            {
                SPI_I2S_SendData(HwUnit, txVal);
            }
            else
            {
                SPI_I2S_SendData(HwUnit, (uint16_t)txVal);
            }

            // Tăng chỉ số byte
            ContextSavedPtr->ByteIndex++;

            // Kiểm tra xem đã hết dữ liệu của Channel hiện tại chưa
            if (ContextSavedPtr->ByteIndex >= length)
            {
                ContextSavedPtr->ByteIndex = 0;
                JobRuntime->ChannelIndex++; // Chuyển sang channel tiếp theo cho ngắt kế tiếp
            }
        }
        else
        {
            // Tạm khóa ngắt TXE nếu channel này đã xong nhưng chưa kịp chuyển đổi
            SPI_I2S_ITConfig(HwUnit, SPI_I2S_IT_TXE, DISABLE);
        }
    }
}

void SPI1_IRQHandler(void)
{
    Spi_Hw_IrqHandler(SPI1);
}

void SPI2_IRQHandler(void)
{
    Spi_Hw_IrqHandler(SPI2);
}

/* ==========================================================
 * PRIVATE API HỖ TRỢ TRUYỀN NHẬN THẤP CẤP (INLINE FUNCTIONS)
 * ========================================================== */

/**
 * @brief Đợi đến khi bộ đệm truyền trống (TXE flag).
 */
static inline void Spi_WaitTxEmpty(SPI_TypeDef *HwUnit)
{
    while (SPI_I2S_GetFlagStatus(HwUnit, SPI_I2S_FLAG_TXE) == RESET)
        ;
}

/**
 * @brief Đợi đến khi có dữ liệu trong bộ đệm nhận (RXNE flag).
 */
static inline void Spi_WaitRxReady(SPI_TypeDef *HwUnit)
{
    while (SPI_I2S_GetFlagStatus(HwUnit, SPI_I2S_FLAG_RXNE) == RESET)
        ;
}

/**
 * @brief Đợi đến khi bus SPI hoàn tất truyền và không còn bận (BSY flag clear).
 */
static inline void Spi_WaitBusyClear(SPI_TypeDef *HwUnit)
{
    while (SPI_I2S_GetFlagStatus(HwUnit, SPI_I2S_FLAG_BSY) == SET)
        ;
}

/**
 * @brief Đọc dữ liệu thô từ thanh ghi DR theo kích thước 8-bit hoặc 16-bit.
 */
static inline uint16 Spi_ReadData(SPI_TypeDef *HwUnit, uint8 dataSize)
{
    (void)dataSize; // Có thể dùng để rẽ nhánh nếu cần phân biệt 8/16-bit ở tầng đọc thô
    return SPI_I2S_ReceiveData(HwUnit);
}

/**
 * @brief Ghi dữ liệu thô vào thanh ghi DR.
 */
static inline Spi_WriteData(SPI_TypeDef *HwUnit, uint8 dataSize, uint16 data)
{
    (void)dataSize;
    SPI_I2S_SendData(HwUnit, data);
}

void Spi_Hw_Sync_TransmitReceive(uint16 *TxPtr,
                                 uint16 *RxPtr,
                                 uint8 length,
                                 Spi_HwUnitType_e HwId,
                                 Spi_DirectionType direction,
                                 Spi_DataSizeType datasize)
{
    uint16 index = 0;
    uint16 dummyData = 0xFFFFU; // Giá trị Dummy byte khi cần phát để lấy clock về (Rx-Only)

    // Ép kiểu con trỏ nguồn/đích tạm thời dựa trên DataSize để tăng tốc độ truy xuất mảng
    const uint8 *pTx8 = (const uint8 *)TxPtr;
    const uint16 *pTx16 = (const uint16 *)TxPtr;
    uint8 *pRx8 = (uint8 *)RxPtr;
    uint16 *pRx16 = (uint16 *)RxPtr;

    SPI_TypeDef *Spix = Spi_Map_GetHwInstance(HwId);

    /* --- PHÂN LOẠI XỬ LÝ THEO DIRECTION --- */
    switch (direction)
    {
    case SPI_MR_1LINE_TX: // Chỉ truyền (Transmit Only)
    {
        // Bật chiều phát trước khi gửi
        SPI_BiDirectionalLineConfig(Spix, SPI_Direction_Tx);

        // chờ truyền xong toàn bộ dữ liệu từ tx buffer
        while (index < length)
        {
            Spi_WaitTxEmpty(Spix); // đợi bộ truyền trống

            /*lưu trữ lại dữ liệu 16 hoặc 8 bit*/
            uint16 txVal = (datasize == SPI_MR_DATASIZE_16B) ? pTx16[index] : pTx8[index];

            /*đặt data vào bộ truyền để gửi đi*/
            Spi_WriteData(Spix, datasize, txVal);
            index++; /*di chuyển đến chỉ số data kế tiếp*/
        }
        // Đợi truyền hết byte cuối cùng và không còn bận truyền
        Spi_WaitTxEmpty(Spix);
        Spi_WaitBusyClear(Spix);
        break;
    }
    case SPI_MR_1LINE_RX:
    {
        // Chuyển sang chiều nhận trước khi đọc
        SPI_BiDirectionalLineConfig(Spix, SPI_Direction_Rx);

        while (index < length)
        {
            // chờ bộ nhận đầy (có dữ liệu trên thanh ghi)
            Spi_WaitRxReady(Spix);

            // đọc từ thanh ghi vào RAM
            uint16_t rxVal = Spi_ReadData(Spix, datasize);

            // Ép kiểu đọc về phù hợp với cấu hình
            if (RxPtr != NULL)
            {
                if (datasize == SPI_MR_DATASIZE_16B)
                {
                    pRx16[index] = rxVal;
                }
                else
                {
                    pRx8[index] = (uint8_t)rxVal;
                }
            }
            index++;
        }
        break;
    }
    case SPI_MR_2LINES_RX_ONLY: // Chỉ nhận (Receive Only - Master tự sinh Clock)
    {
        // chờ đọc xong toàn bộ dữ liệu vào rx buffer
        while (index < length)
        {
            // Bước 1: Master phải phát Dummy Byte để Slave có xung nhịp (SCK) đẩy dữ liệu ra
            Spi_WaitTxEmpty(Spix);
            Spi_WriteData(Spix, datasize, dummyData);

            // Bước 2: Chờ nhận dữ liệu từ bộ đệm Rx
            Spi_WaitRxReady(Spix);

            // Bước 3 : đọc ra data từ thanh ghi có thể là 8 / 16 bit
            uint16 rxVal = Spi_ReadData(Spix, datasize);

            // Bước 3 : ép lại kiểu phù hợp với cấu hình
            if (RxPtr != NULL)
            {
                if (datasize == SPI_DataSize_16b)
                {
                    pRx16[index] = rxVal;
                }
                else
                {
                    pRx8[index] = (uint8)rxVal;
                }
            }
            index++; // nhảy đến vị trí data tiếp theo
        }
        break;
    }
    case SPI_MR_2LINES_FD: // Song công toàn phần (Full-Duplex -> master only)
    {
        /*chờ đến khi dọc/ghi hoàn tất cả tx và rx buffer*/
        while (index < length)
        {
            // bước 1 : Master chọn phát dữ liệu thực hoặc dummy byte cho slave
            Spi_WaitTxEmpty(Spix);
            uint16 txVal = (TxPtr != NULL) ? ((datasize == SPI_DataSize_16b) ? pTx16[index] : pTx8[index]) : dummyData;
            Spi_WriteData(Spix, datasize, txVal);

            // Bước 2 : Master đọc dữ liệu từ slave có thể là dữ liệu thực hoặc dummy read (bỏ qua nếu không cần xử lý)
            Spi_WaitRxReady(Spix);
            uint16 rxVal = Spi_ReadData(Spix, datasize);

            // Bước 3 : ép lại kiểu phù hợp với cấu hình
            if (RxPtr != NULL)
            {
                if (datasize == SPI_DataSize_16b)
                {
                    pRx16[index] = rxVal;
                }
                else
                {
                    pRx8[index] = (uint8)rxVal;
                }
            }
            index++;
        }
        Spi_WaitBusyClear(Spix);
        break;
    }
    default:
        // Xử lý lỗi cấu hình nếu cần
        break;
    }
}
void Spi_Hw_StartFirstByteInterrupt(
    SPI_TypeDef *Spix,
    const uint16 *TxPtr,
    uint16 *RxPtr,
    Spi_DirectionType direction,
    Spi_DataSizeType datasize)
{
    uint16 firstTxVal = 0xFFFFU; // Giá trị mặc định cho Dummy / Full-Duplex khi TxPtr == NULL
    uint16 dummyData = 0xFFFFU;

    // 1. Xác định giá trị phát đầu tiên dựa trên Direction và TxPtr
    switch (direction)
    {
    case SPI_MR_1LINE_TX:
    case SPI_MR_2LINES_FD:
    {
        if (TxPtr != NULL_PTR)
        {
            // Ép kiểu lấy giá trị phần tử đầu tiên (index = 0) dựa vào DataSize
            if (datasize == SPI_MR_DATASIZE_16B) // Hoặc tên macro datasize 16-bit tương ứng trong project của bạn
            {
                firstTxVal = TxPtr[0];
            }
            else
            {
                // Nếu là 8-bit, ép kiểu con trỏ để lấy đúng byte đầu
                firstTxVal = (uint16)(((const uint8 *)TxPtr)[0]);
            }
        }
        break;
    }
    case SPI_MR_2LINES_RX_ONLY:
    {
        // Master nhận hoàn toàn: Cần phát dummy data để tạo clock cho Slave đẩy dữ liệu ra
        firstTxVal = dummyData;
        break;
    }
    case SPI_MR_1LINE_RX:
    {
        // Trường hợp 1 dây chuyên nhận thuần túy, có thể cần cấu hình hướng dòng trước
        SPI_BiDirectionalLineConfig(Spix, SPI_Direction_Rx);
        // Đối với dòng RX 1-line, thường không cần ghi mồi dữ liệu ngay nếu chưa đổi chiều,
        // hoặc tùy thuộc vào phần cứng cụ thể của bạn.
        return;
    }
    default:
        break;
    }

    // 2. Cấu hình độ dài dữ liệu (8-bit hoặc 16-bit) trên thanh ghi SPI của SPL
    if (datasize == SPI_MR_DATASIZE_16B)
    {
        SPI_DataSizeConfig(Spix, SPI_DataSize_16b);
    }
    else
    {
        SPI_DataSizeConfig(Spix, SPI_DataSize_8b);
    }

    // 3. Thực hiện "mồi" byte đầu tiên vào thanh ghi dữ liệu (SPI_DR) qua SPL
    // Hành động ghi này sẽ kích hoạt phần cứng đẩy dữ liệu đi và sinh ra ngắt sau đó.
    if (datasize == SPI_MR_DATASIZE_16B)
    {
        SPI_I2S_SendData(Spix, firstTxVal);
    }
    else
    {
        SPI_I2S_SendData(Spix, (uint16_t)firstTxVal);
    }
}