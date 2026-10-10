/**
 * @file Spi_Internal.h
 * @brief Khai báo các hàm phối hợp nội bộ của SPI MCAL.
 * @details
 * 1. Spi_InternalStartJob() thiết lập và bắt đầu một Job, theo polling hoặc
 *    interrupt tùy chế độ runtime của Sequence.
 * 2. Spi_InternalFinishJob() giải phóng Job, khởi chạy Job kế tiếp nếu còn,
 *    hoặc kết thúc Sequence.
 * 3. Các hàm này chỉ dành cho các thành phần triển khai bên trong SPI driver.
 *
 * @version     1.0.0
 * @date        2026
 * @author      Pham Cao Duy
 */
#ifndef SPI_INTERNAL_H
#define SPI_INTERNAL_H
#include "Spi_Types.h"
/**
 * @brief Chuẩn bị và bắt đầu thực thi một Job thuộc Sequence.
 * @param JobId ID logic của Job cần chạy.
 * @param SeqId ID logic của Sequence chứa Job.
 * @return E_OK nếu Job được bắt đầu/hoàn tất thành công; E_NOT_OK nếu không
 *         lấy được runtime cần thiết hoặc Hardware Unit không sẵn sàng.
 * @details
 * 1. Đọc cấu hình Device và khởi tạo trạng thái runtime/CS của Job.
 * 2. Chạy tuần tự bằng polling hoặc lưu context và bật TXE/RXNE interrupt.
 */
Std_ReturnType Spi_InternalStartJob(Spi_JobType_e JobId, Spi_SequenceType_e SeqId);
/**
 * @brief Hoàn tất một Job và điều phối phần còn lại của Sequence.
 * @param JobId ID logic của Job vừa hoàn tất.
 * @param SeqId ID logic của Sequence chứa Job.
 * @return E_OK nếu chuyển tiếp hoặc kết thúc thành công; E_NOT_OK nếu runtime
 *         Job/Sequence không hợp lệ.
 * @details
 * 1. Đánh dấu Job hiện tại không bận và tiến chỉ số Job trong Sequence.
 * 2. Khởi chạy Job kế tiếp nếu còn; nếu không, giải phóng CS/bus và gọi
 *    callback Sequence nếu được cấu hình.
 */
Std_ReturnType Spi_InternalFinishJob(Spi_JobType_e JobId, Spi_SequenceType_e SeqId);

#endif