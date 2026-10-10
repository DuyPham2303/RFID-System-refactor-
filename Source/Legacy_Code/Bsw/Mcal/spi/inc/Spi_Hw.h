/**
 * @file Spi_Hw.h
 * @brief API truyền/nhận mức phần cứng của SPI MCAL.
 * @details
 * 1. Các API nhận Hardware Unit logic hoặc peripheral SPL tùy thao tác.
 * 2. Spi_Hw_Sync_TransmitReceive() thực hiện trao đổi dữ liệu đồng bộ.
 * 3. Spi_Hw_StartFirstByteInterrupt() nạp phần tử đầu tiên để khởi động
 *    truyền interrupt; việc xử lý tiếp theo do ISR thực hiện.
 */

#ifndef SPI_HW_H
#define SPI_HW_H

#include "Spi_Types.h"
#include "Spi_map.h"
/**
 * @brief Truyền/nhận đồng bộ trên Hardware Unit đã cấu hình.
 * @param TxPtr Buffer nguồn; có thể NULL tùy Direction.
 * @param RxPtr Buffer nhận; có thể NULL nếu bỏ qua dữ liệu nhận.
 * @param length Số phần tử dữ liệu cần xử lý.
 * @param HwId ID logic của Hardware Unit.
 * @param direction Chế độ full-duplex, receive-only hoặc một đường Tx/Rx.
 * @param datasize Độ rộng phần tử dữ liệu 8-bit hoặc 16-bit.
 * @details
 * 1. Chọn cách phát/nhận theo Direction.
 * 2. Chờ các cờ TXE/RXNE cần thiết, đọc/ghi dữ liệu theo DataSize.
 * 3. Chờ bus rảnh khi chế độ truyền yêu cầu hoàn tất giao dịch.
 */
void Spi_Hw_Sync_TransmitReceive(uint16 *TxPtr,
                                 uint16 *RxPtr,
                                 uint8 length,
                                 Spi_HwUnitType_e HwId,
                                 Spi_DirectionType direction,
                                 Spi_DataSizeType datasize);

/**
 * @brief Nạp phần tử đầu tiên để bắt đầu một giao dịch SPI dùng interrupt.
 * @param Spix Con trỏ peripheral SPI SPL đã được ánh xạ.
 * @param TxPtr Buffer nguồn; có thể NULL để phát dữ liệu dummy.
 * @param RxPtr Buffer nhận; hiện không được truy cập bởi hàm này.
 * @param direction Hướng truyền/nhận của giao dịch.
 * @param datasize Độ rộng dữ liệu 8-bit hoặc 16-bit.
 * @details Với các hướng cần phát, hàm chọn dữ liệu đầu tiên hoặc dummy và
 *          ghi vào thanh ghi dữ liệu. Hướng 1-line RX cấu hình chiều nhận.
 */
void Spi_Hw_StartFirstByteInterrupt(
    SPI_TypeDef *Spix,
    const uint16 *TxPtr,
    uint16 *RxPtr,
    Spi_DirectionType direction,
    Spi_DataSizeType datasize);
#endif /* SPI_HW_H */