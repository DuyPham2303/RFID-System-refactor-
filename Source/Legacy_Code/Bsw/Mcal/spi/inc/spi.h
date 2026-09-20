/**
 * @file        Spi.h
 * @brief       AUTOSAR-inspired SPI MCAL Driver.
 * @details
 * This file provides the public API of the SPI MCAL driver.
 *
 * The SPI driver offers:
 * - Driver initialization
 * - Internal and External Buffer handling
 * - Synchronous transmission
 * - Asynchronous transmission
 * - Status monitoring
 * - Callback registration
 *
 * The Application Layer may directly use this API for hardware testing.
 * In a complete AUTOSAR architecture, these APIs are intended to be
 * accessed through IoHwAb and RTE.
 *
 * @version     1.0.0
 * @date        2026
 * @author      Pham Cao Duy
 */
#ifndef __SPI_H
#define __SPI_H
#include "Spi_Cfg.h"

/**
 * @brief Khởi tạo SPI driver.
 * @details Kiểm tra cấu hình, khởi tạo SPI hardware và đưa các đối tượng
 *          runtime về trạng thái sẵn sàng. Hàm phải được gọi trước các API
 *          truyền dữ liệu khác.
 * @param[in] ConfigPtr Con trỏ tới bộ cấu hình SPI tĩnh.
 * @return E_OK nếu khởi tạo thành công; E_NOT_OK nếu cấu hình không hợp lệ
 *         hoặc SPI hardware không thể khởi tạo.
 */
Std_ReturnType Spi_Init(const Spi_ConfigType *ConfigPtr);

/**
 * @brief Hủy khởi tạo SPI driver.
 * @details Dừng các hoạt động SPI đang được quản lý và đưa driver về trạng
 *          thái SPI_UNINIT. Sau khi gọi hàm này, phải gọi lại Spi_Init()
 *          trước khi sử dụng driver.
 */
void Spi_DeInit(void);

/**
 * @brief Đọc trạng thái hiện tại của SPI driver.
 * @return SPI_UNINIT, SPI_IDLE hoặc SPI_BUSY.
 */
Spi_StatusType Spi_GetStatus(void);

/**
 * @brief Gán buffer bên ngoài cho một SPI Channel.
 * @details Liên kết buffer truyền và nhận do application cung cấp với Channel
 *          logic. Driver sẽ sử dụng các buffer này khi thực thi Job chứa
 *          Channel tương ứng.
 * @param[in] Channel ID logic của Channel cần cấu hình.
 * @param[in] TxBuffer Buffer chứa dữ liệu cần truyền.
 * @param[out] RxBuffer Buffer nhận dữ liệu từ SPI.
 * @param[in] Length Số byte truyền/nhận.
 * @return E_OK nếu buffer hợp lệ và được gán; E_NOT_OK nếu tham số không hợp
 *         lệ hoặc Channel không tồn tại.
 */
Std_ReturnType Spi_SetupEB(
    Spi_ChannelId_Type Channel,
    uint16 *TxBuffer,
    uint16 *RxBuffer,
    uint16 Length);

/**
 * @brief Ghi dữ liệu vào buffer nội bộ của SPI Channel.
 * @details Sao chép dữ liệu từ DataBuffer vào vùng nhớ truyền do driver
 *          quản lý. Cách này không yêu cầu application duy trì buffer trong
 *          suốt thời gian truyền.
 * @param[in] Channel ID logic của Channel cần ghi dữ liệu.
 * @param[in] DataBuffer Con trỏ tới dữ liệu nguồn.
 * @return E_OK nếu ghi thành công; E_NOT_OK nếu tham số hoặc Channel không hợp lệ.
 */
Std_ReturnType Spi_WriteIB(
    Spi_ChannelId_Type Channel,
    const uint8 *DataBuffer);

/**
 * @brief Đọc dữ liệu từ buffer nhận nội bộ của SPI Channel.
 * @details Sao chép dữ liệu đã nhận từ buffer nội bộ của driver sang vùng
 *          nhớ do application cung cấp.
 * @param[in] Channel ID logic của Channel cần đọc dữ liệu.
 * @param[out] DataBuffer Buffer đích do application cung cấp.
 * @return E_OK nếu đọc thành công; E_NOT_OK nếu dữ liệu chưa sẵn sàng hoặc
 *         tham số không hợp lệ.
 */
Std_ReturnType Spi_ReadIB(
    Spi_ChannelId_Type Channel,
    uint8 *DataBuffer);

/**
 * @brief Thực hiện truyền SPI đồng bộ.
 * @details Driver lần lượt thực thi tất cả Job thuộc Sequence và chỉ trả về
 *          sau khi toàn bộ Sequence hoàn tất hoặc phát hiện lỗi.
 * @param[in] Sequence ID logic của Sequence cần thực thi.
 * @return E_OK nếu truyền hoàn tất; E_NOT_OK nếu driver chưa khởi tạo,
 *         Sequence không hợp lệ hoặc giao dịch thất bại.
 */
Std_ReturnType Spi_SyncTransmit(
    Spi_SequenceId_Type Sequence);

/**
 * @brief Bắt đầu truyền SPI bất đồng bộ.
 * @details Driver bắt đầu thực thi Sequence rồi trả về ngay. Khi hoàn tất,
 *          callback của Job hoặc Sequence đã đăng ký sẽ được gọi.
 * @param[in] Sequence ID logic của Sequence cần thực thi.
 * @return E_OK nếu giao dịch được bắt đầu; E_NOT_OK nếu driver đang bận,
 *         Sequence không hợp lệ hoặc không thể bắt đầu truyền.
 */
Std_ReturnType Spi_AsyncTransmit(
    Spi_SequenceId_Type Sequence);

/**
 * @brief Hủy một SPI Sequence đang thực thi.
 * @details Dừng việc xử lý các Job còn lại thuộc Sequence nếu driver hỗ trợ
 *          hủy ở trạng thái hiện tại.
 * @param[in] Sequence ID logic của Sequence cần hủy.
 * @return E_OK nếu hủy thành công; E_NOT_OK nếu Sequence không chạy hoặc
 *         không thể hủy.
 */
Std_ReturnType Spi_Cancel(
    Spi_SequenceId_Type Sequence);

#endif
