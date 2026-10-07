/**
 * @file Spi_Internal.h
 * @brief Khai báo nội bộ của SPI MCAL.
 * @details File này chỉ được các file triển khai bên trong SPI driver sử
 *          dụng, chẳng hạn Spi.c, Spi_Async.c hoặc Spi_Irq.c. Các đối tượng
 *          và hàm trong file không phải là API dành cho Application Layer,
 *          IoHwAb hoặc các module bên ngoài SPI.
 *
 * @version     1.0.0
 * @date        2026
 * @author      Pham Cao Duy
 */
#ifndef SPI_INTERNAL_H
#define SPI_INTERNAL_H
#include "Spi_Types.h"
/**
 * @brief Bắt đầu thực thi nội bộ một SPI Job.
 * @param Job ID logic của Job cần thực thi.
 * @note Chỉ được gọi bởi phần triển khai SPI driver sau khi đã kiểm tra
 *       trạng thái driver và cấu hình liên quan.
 */
Std_ReturnType Spi_InternalStartJob(Spi_JobType_e JobId, Spi_SequenceType_e SeqId);
/**
 * @brief Hoàn tất xử lý nội bộ một SPI Job.
 * @param JobId: Định danh Job vừa hoàn tất (để truy xuất vào RAM runtime
 *               cập nhật lại trạng thái IsBusy = FALSE).
 * @param Dio_ChannelType Cspin: Chân Chip Select tương ứng để nhả thiết
 *                        bị ngoại vi.
 * @param Spi_SequenceType_e SeqId: (Tùy chọn) Định danh Sequence chứa Job này,
 *                           giúp driver biết được sau Job này thì Sequence cần
 *                           làm gì tiếp theo.
 *
 * @note Hàm cập nhật trạng thái runtime và thực hiện notification của Job
 *       nếu callback tương ứng đã được đăng ký.
 */
Std_ReturnType Spi_InternalFinishJob(Spi_JobType_e JobId, Spi_SequenceType_e SeqId);

#endif