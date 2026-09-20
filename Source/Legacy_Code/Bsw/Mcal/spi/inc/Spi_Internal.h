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

#include "Spi_Cfg.h"

////////////////////////***IMPLEMENTATION tại file Spi_Cfg.c***///////////////////////
/**
 * @brief Bảng đối tượng runtime của SPI driver.
 * @details Lưu trạng thái hoạt động hiện tại của từng SPI instance đã cấu
 *          hình, chẳng hạn trạng thái idle/busy và Channel đang xử lý.
 */
extern Spi_RuntimeType Spi_Channels_Status[SPI_CHANNEL_MAX];

/**
 * @brief Bảng cấu hình các SPI Channel.
 * @details Mỗi phần tử liên kết một Channel ID logic với buffer truyền,
 *          buffer nhận và độ dài dữ liệu tương ứng.
 */
extern Spi_ChannelConfigType Spi_Channels_data[SPI_CHANNEL_MAX];

/**
 * @brief Bảng cấu hình các SPI Job.
 * @details Mỗi Job xác định SPI hardware unit và danh sách Channel được
 *          thực thi trong một giao dịch SPI.
 */
extern const Spi_JobConfigType Spi_Jobs[SPI_JOB_MAX];

/**
 * @brief Bảng cấu hình các SPI Sequence.
 * @details Mỗi Sequence chứa danh sách Job ID theo thứ tự thực thi. Sequence
 *          cho phép driver gom nhiều giao dịch thành một luồng xử lý logic.
 */
extern const Spi_SequenceConfigType Spi_Sequences[SPI_SEQ_MAX];
/**
 * @brief Kiểu callback thông báo hoàn thành SPI.
 * @details Callback do tầng trên đăng ký và được SPI driver gọi sau khi một
 *          Job hoặc Sequence hoàn tất.
 */

////////////////////////***IMPLEMENTATION tại file Spi_Internal.c***///////////////////////

typedef void (*Spi_NotificationType)(void);

/**
 * @brief Đăng ký callback hoàn tất cho một SPI Job.
 * @details Callback được gọi khi Job tương ứng hoàn thành. Truyền NULL_PTR
 *          để hủy callback hiện tại của Job.
 * @param[in] Job ID logic của Job cần đăng ký callback.
 * @param[in] Callback Hàm callback không tham số; có thể là NULL_PTR.
 */

void Spi_RegisterJobNotification(
    Spi_JobId_Type Job,
    Spi_NotificationType Callback);

/* Các hàm hỗ trợ public module Service/CallbackM */

/**
 * @brief Đăng ký callback hoàn tất cho một SPI Sequence.
 * @details Callback được gọi sau khi tất cả Job trong Sequence đã thực thi
 *          xong. Truyền NULL_PTR để hủy callback hiện tại của Sequence.
 * @param[in] Sequence ID logic của Sequence cần đăng ký callback.
 * @param[in] Callback Hàm callback không tham số; có thể là NULL_PTR.
 */
void Spi_RegisterSequenceNotification(
    Spi_SequenceConfigType Sequence,
    Spi_NotificationType Callback);

/* Các hàm hỗ trợ nội bộ Spi.c , không phải API public của SPI. */

/**
 * @brief Bắt đầu thực thi nội bộ một SPI Job.
 * @param Job ID logic của Job cần thực thi.
 * @note Chỉ được gọi bởi phần triển khai SPI driver sau khi đã kiểm tra
 *       trạng thái driver và cấu hình liên quan.
 */
void Spi_InternalStartJob(
    Spi_JobId_Type Job);

/**
 * @brief Hoàn tất xử lý nội bộ một SPI Job.
 * @param Job ID logic của Job vừa thực thi xong.
 * @note Hàm cập nhật trạng thái runtime và thực hiện notification của Job
 *       nếu callback tương ứng đã được đăng ký.
 */
void Spi_InternalFinishJob(
    Spi_JobId_Type Job);

#endif