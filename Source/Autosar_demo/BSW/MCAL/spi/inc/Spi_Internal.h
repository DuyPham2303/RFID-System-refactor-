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

////////////////////////***Memory pool sử dụng nội bộ Spi Driver***///////////////////////

extern const Spi_JobConfigType Spi_Jobs[SPI_JOB_MAX];

extern const Spi_SequenceConfigType Spi_Sequences[SPI_SEQ_MAX];

////////////////////////////////////////////////*** Spi's Async Param config & callback type ***////////////////////////////////////////////////

/**
 * @brief Kiểu callback thông báo hoàn thành SPI.
 * @details Callback do tầng trên đăng ký và được SPI driver gọi sau khi một
 *          Job hoặc Sequence hoàn tất.
 */
typedef void (*Spi_notificationType)(void);
/**
 * @brief Enum định danh các nguồn ngắt logic cho module Spi (SPI1, SPI2, SPI3...)
 */
typedef enum
{
    Spi_IRQ_SOURCE_TXE = 0U, // Ngắt báo bộ đệm truyền trống (Transmit Buffer Empty - Sẵn sàng gửi byte tiếp theo)
    Spi_IRQ_SOURCE_RXNE,     // Ngắt báo bộ đệm nhận đầy (Receive Buffer Not Empty - Có dữ liệu mới vừa nhận về)
    Spi_IRQ_SOURCE_ERROR,    // Ngắt báo lỗi đường truyền (Overrun, Mode Fault, CRC Error...)
    Spi_IRQ_SOURCE_MAX
} Spi_IrqSourceType;
/**
 * @brief kiểu sữ liệu chuẩn hóa cấu hình cho đối tượng Job được xử lý ở module Spi_Internal
 * @details dữ liệu sử dụng cục bộ trong module mcal khi xử lý các tác vụ bất đồng bộ
 *          liên quan tới quản lý,đăng ký và thực thi callback function
 * @param [SeqNoti]      địa chỉ nghiệp vụ callback
 * @param [flag]         cờ ngắt cần xử lý
 */
typedef struct Spi_AsyncJobCfg
{
    Spi_notificationType JobNoti;
    Spi_IrqSourceType flag;
} Spi_AsyncJobCfgType;
/**
 * @brief kiểu sữ liệu chuẩn hóa cấu hình cho đối tượng Sequence được xử lý ở module Spi_Internal
 * @details dữ liệu sử dụng cục bộ trong module mcal khi xử lý các tác vụ bất đồng bộ
 *          liên quan tới quản lý,đăng ký và thực thi callback function
 * @param [JobAsyncArray] mảng tĩnh lưu trữ cấu hình các tác vụ, quy định mỗi nghiệp vụ
 *                       chỉ có thể quản lý tối đa số lượng tác vụ callback ứng với
 *                       danh sách enum đã quy định
 * @param [SeqNoti]      địa chỉ nghiệp vụ callback
 */
typedef struct Spi_AsyncSeqCfg
{
    Spi_AsyncJobCfgType JobAsyncArray[SPI_JOB_MAX];
    Spi_notificationType SeqNoti;
} Spi_AsyncSeqCfgType;

////////////////////////***Private API sử dụng nội bộ Spi Driver ***///////////////////////

/**
 * @brief Đăng ký callback hoàn tất cho một Sequence và các Job mà nó quản lý.
 *
 * @param AsyncSeqCfgPtr cấu trúc lưu trữ địa chỉ callback của seq và các job tương ứng, cũng như nguồn ngắt
 * @param JobId Id ánh xạ tới job cần đăng ký hàm callback
 * @param SeqId Id ánh xạ tới sequence cần đăng ký hàm callback
 * @return Std_ReturnType
 * [E_OK]     : nếu đăng ký thành công
 * [E_NOT_OK] : nếu không tìm thấy ID của callback cần đăng ký
 *
 */
Std_ReturnType Spi_RegisterSeqNoti(const Spi_AsyncSeqCfgType *AsyncSeqCfgPtr, Spi_JobId_Type JobId, Spi_SequenceId_Type SeqId);
/**
 * @brief Hàm tìm kiếm và trả về Id của Job và sequence tương ứng với channel
 *
 * @param channelId   Id của channel cần tham chiếu
 * @param foundJobId  con trỏ lưu Job Id tìm được
 * @param foundSeqId  con trỏ lưu Seq Id tìm được
 * @return Std_ReturnType
 */
Std_ReturnType Spi_GetJobAndSeqId(Spi_ChannelId_Type channelId, Spi_JobId_Type *foundJobId, Spi_SequenceId_Type *foundSeqId);
/**
 * @brief Bắt đầu thực thi nội bộ một SPI Job.
 * @param Job ID logic của Job cần thực thi.
 * @note Chỉ được gọi bởi phần triển khai SPI driver sau khi đã kiểm tra
 *       trạng thái driver và cấu hình liên quan.
 */
void Spi_InternalStartJob(Spi_JobId_Type JobId, Spi_SequenceId_Type SeqId);
/**
 * @brief Hoàn tất xử lý nội bộ một SPI Job.
 * @param Job ID logic của Job vừa thực thi xong.
 * @note Hàm cập nhật trạng thái runtime và thực hiện notification của Job
 *       nếu callback tương ứng đã được đăng ký.
 */
void Spi_InternalFinishJob(Spi_JobId_Type JobId, Spi_SequenceId_Type SeqId);
/**
 * @brief Hàm khởi tạo Memory pool nội bộ do Spi Driver quản lý
 * @details Gọi trong Spi_Init() để reset toàn bộ dữ liệu tránh byte rác
 *          trước khi thực hiện copy data thực tế từ App
 */
void Spi_InitInternalBuffers(void);

#endif