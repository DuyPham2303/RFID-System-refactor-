/**
 * @file        Spi.h
 * @brief       API công khai của SPI MCAL Driver.
 * @details
 * Header khai báo các API ứng dụng dùng để khởi tạo driver, chuẩn bị buffer
 * và yêu cầu truyền một Sequence. Cấu hình Channel, Job, Sequence và phần cứng
 * được cung cấp qua Spi_ConfigType_s.
 *
 * 1. Spi_Init() khởi tạo driver với bộ cấu hình.
 * 2. Ứng dụng ghi/đọc dữ liệu qua API IB hoặc gán buffer qua API EB.
 * 3. Spi_SyncTransmit() chạy theo kiểu polling; Spi_AsyncTransmit() chạy
 *    theo kiểu interrupt.
 * 4. Các API còn lại cung cấp thao tác hủy và giải phóng driver.
 *
 * Trong kiến trúc AUTOSAR đầy đủ, tầng ứng dụng thường truy cập SPI thông qua
 * IoHwAb/RTE thay vì gọi trực tiếp các API MCAL.
 *
 * @version     1.0.0
 * @date        2026
 * @author      Pham Cao Duy
 */
#ifndef __SPI_H
#define __SPI_H
#include "Spi_Cfg.h"
/**
 * @brief Khởi tạo SPI driver và các Hardware Unit được cấu hình.
 * @param[in] ConfigPtr Con trỏ tới cấu hình gồm Device, Channel, Job và
 *                      Sequence.
 * @return E_OK nếu khởi tạo thành công; E_NOT_OK nếu không thể khởi tạo.
 *
 * @details
 * 1. Phải gọi hàm này trước các API truyền/nhận dữ liệu.
 * 2. Cấu hình phải còn hợp lệ trong thời gian driver sử dụng.
 */
Std_ReturnType Spi_Init(const Spi_ConfigType_s *ConfigPtr);

/**
 * @brief Giải phóng trạng thái khởi tạo của SPI driver.
 * @details Sau khi hủy khởi tạo, cần gọi Spi_Init() trước khi yêu cầu truyền
 *          dữ liệu trở lại.
 */
void Spi_DeInit(void);

/**
 * @brief Gán buffer ngoài (EB) cho một Channel.
 * @param[in] Channel ID logic của Channel cần thiết lập.
 * @param[in] TxBuffer Buffer dữ liệu nguồn; có thể được dùng làm buffer Tx.
 * @param[out] RxBuffer Buffer đích nhận dữ liệu.
 * @param[in] Length Số phần tử dữ liệu cần truyền/nhận.
 * @return E_OK nếu thiết lập buffer thành công; E_NOT_OK nếu Channel không
 *         được cấu hình dùng EB hoặc runtime từ chối thiết lập.
 *
 * @details
 * 1. Các buffer do bên gọi quản lý, vì vậy phải còn hợp lệ trong toàn bộ
 *    thời gian giao dịch sử dụng Channel.
 * 2. Kích thước phần tử thực tế phụ thuộc cấu hình DataSize 8-bit/16-bit.
 */
Std_ReturnType Spi_SetupEB(
    Spi_ChannelType_e Channel,
    uint16 *TxBuffer,
    uint16 *RxBuffer,
    uint8 Length);

/**
 * @brief Ghi dữ liệu truyền vào Internal Buffer (IB) của Channel.
 * @param[in] Channel ID logic của Channel cần ghi.
 * @param[in] DataBuffer Buffer nguồn do bên gọi cung cấp.
 * @param[in] Length Số phần tử cần sao chép vào IB.
 * @return E_OK nếu ghi thành công; E_NOT_OK nếu Channel không dùng IB hoặc
 *         Length vượt sức chứa IB.
 *
 * @details Driver sao chép dữ liệu vào vùng nhớ nội bộ; bên gọi không cần giữ
 *          DataBuffer còn sống sau khi hàm trả về. DataBuffer phải là con trỏ
 *          hợp lệ khi Length khác 0.
 */
Std_ReturnType Spi_WriteIB(
    Spi_ChannelType_e Channel,
    const uint16 *DataBuffer,
    uint8 Length);

/**
 * @brief Sao chép dữ liệu nhận từ Internal Buffer (IB) ra buffer bên gọi.
 * @param[in] Channel ID logic của Channel cần đọc.
 * @param[out] DataBuffer Buffer đích do bên gọi cấp phát.
 * @param[in] Length Tham số độ dài của API; implementation hiện sao chép theo
 *                   độ dài IB đang lưu trong runtime.
 * @return E_OK nếu đọc thành công; E_NOT_OK nếu Channel không dùng IB hoặc
 *         tham số không hợp lệ.
 *
 * @details Buffer đích cần đủ chỗ chứa độ dài dữ liệu đã lưu cho Channel.
 */
Std_ReturnType Spi_ReadIB(
    Spi_ChannelType_e Channel,
    uint16 *DataBuffer,
    uint8 Length);

/**
 * @brief Yêu cầu truyền một Sequence theo chế độ đồng bộ (polling).
 * @param[in] Sequence ID logic của Sequence cần chạy.
 * @return E_OK nếu yêu cầu và quá trình truyền thành công; E_NOT_OK nếu không
 *         thể bắt đầu hoặc xử lý Sequence.
 *
 * @details
 * 1. Các Job trong Sequence được thực thi theo thứ tự cấu hình.
 * 2. Hàm chờ quá trình truyền/nhận của từng Job hoàn tất trước khi trả về.
 */
Std_ReturnType Spi_SyncTransmit(Spi_SequenceType_e Sequence);

/**
 * @brief Bắt đầu truyền một Sequence theo chế độ bất đồng bộ (interrupt).
 * @param[in] Sequence ID logic của Sequence cần chạy.
 * @return E_OK nếu giao dịch được khởi chạy; E_NOT_OK nếu không thể bắt đầu.
 *
 * @details Hàm trả về sau khi khởi tạo giao dịch. Việc tiếp tục truyền/nhận
 *          được xử lý qua ngắt; callback Sequence được gọi khi Sequence kết
 *          thúc nếu callback đã được cấu hình.
 */
Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType_e Sequence);

/**
 * @brief Yêu cầu hủy một Sequence.
 * @param[in] Sequence ID logic của Sequence cần hủy.
 * @return E_OK nếu hủy thành công; E_NOT_OK nếu Sequence không thể hủy.
 */
Std_ReturnType Spi_Cancel(Spi_SequenceType_e Sequence);

#endif
