#include "Spi_Hw.h"
#include "Spi_map.h"
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

void Spi_Hw_Sync_TransmitReceive(const Spi_Hw_dataConfigType *HwDataCfgPtr)
{
    uint16 index = 0;
    uint16 dummyData = 0xFFFFU; // Giá trị Dummy byte khi cần phát để lấy clock về (Rx-Only)

    // Ép kiểu con trỏ nguồn/đích tạm thời dựa trên DataSize để tăng tốc độ truy xuất mảng
    const uint8 *pTx8 = (const uint8 *)HwDataCfgPtr->pTxData;
    const uint16 *pTx16 = (const uint16 *)HwDataCfgPtr->pTxData;
    uint8 *pRx8 = (uint8 *)HwDataCfgPtr->pRxData;
    uint16 *pRx16 = (uint16 *)HwDataCfgPtr->pRxData;

    SPI_TypeDef *Spix = Spi_Map_GetHwInstance(HwDataCfgPtr->HwId);
    Spi_DirectionType direction = HwDataCfgPtr->direction;
    Spi_DataSizeType SizeType = HwDataCfgPtr->SizeType;
    uint8 length = HwDataCfgPtr->length;

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
            uint16 txVal = (SizeType == SPI_MR_DATASIZE_16B) ? pTx16[index] : pTx8[index];

            /*đặt data vào bộ truyền để gửi đi*/
            Spi_WriteData(Spix, SizeType, txVal);
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
            uint16_t rxVal = Spi_ReadData(Spix, SizeType);

            // Ép kiểu đọc về phù hợp với cấu hình
            if (HwDataCfgPtr->pRxData != NULL)
            {
                if (SizeType == SPI_MR_DATASIZE_16B)
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
            Spi_WriteData(Spix, SizeType, dummyData);

            // Bước 2: Chờ nhận dữ liệu từ bộ đệm Rx
            Spi_WaitRxReady(Spix);

            // Bước 3 : đọc ra data từ thanh ghi có thể là 8 / 16 bit
            uint16 rxVal = Spi_ReadData(Spix, SizeType);

            // Bước 3 : ép lại kiểu phù hợp với cấu hình
            if (HwDataCfgPtr->pRxData != NULL)
            {
                if (SizeType == SPI_DataSize_16b)
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
            uint16 txVal = (HwDataCfgPtr->pTxData != NULL) ? ((SizeType == SPI_DataSize_16b) ? pTx16[index] : pTx8[index]) : dummyData;
            Spi_WriteData(Spix, SizeType, txVal);

            // Bước 2 : Master đọc dữ liệu từ slave có thể là dữ liệu thực hoặc dummy read (bỏ qua nếu không cần xử lý)
            Spi_WaitRxReady(Spix);
            uint16 rxVal = Spi_ReadData(Spix, SizeType);

            // Bước 3 : ép lại kiểu phù hợp với cấu hình
            if (HwDataCfgPtr->pRxData != NULL)
            {
                if (SizeType == SPI_DataSize_16b)
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