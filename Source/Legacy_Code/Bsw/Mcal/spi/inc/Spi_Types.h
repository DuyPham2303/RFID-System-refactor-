/**
 * @file        Spi_Types.h
 * @brief       Các kiểu dữ liệu logic dùng chung của SPI MCAL.
 * @details
 * 1. Khai báo ID logic cho Hardware Unit, Channel, Job và Sequence.
 * 2. Khai báo chế độ truyền, loại buffer và kiểu callback thông báo.
 * 3. Các kiểu cấu hình phần cứng SPI được khai báo trong Spi_map.h.
 *
 * @version     1.0.0
 * @date        2026
 * @author      Pham Cao Duy
 */
#ifndef SPI_TYPES_H
#define SPI_TYPES_H

#include "./Bsw/Services/Common/Std_Types.h"
/**
 * @brief ID logic ánh xạ tới peripheral SPI vật lý qua lớp mapping.
 * @details Các giá trị hợp lệ tương ứng với SPI1 và SPI2 trên STM32F103C8T6;
 *          SPI_HW_MAX_UNIT là số lượng Hardware Unit được khai báo.
 */
typedef enum Spi_HwUnitType_e
{
    SPI_HW_UNIT_1 = 0U,
    SPI_HW_UNIT_2,
    SPI_HW_MAX_UNIT
} Spi_HwUnitType_e;
/**
 * @brief ID logic của một SPI Channel.
 * @details
 * 1. Channel đại diện cho một luồng dữ liệu logic được Job tham chiếu.
 * 2. ID này không chỉ định peripheral SPI, chân CS hay chế độ truyền.
 * @implements Quy ước đặt tên: SPI_CH_<Tên_Thiết_Bị>_<Chức_Năng>.
 */
typedef enum Spi_ChannelType
{
    SPI_CH_LED_CMD = 0U,
    SPI_CH_SENSOR_READ,
    SPI_CH_FAN_CMD,
    SPI_CH_SPEED_CMD,
    SPI_CH_WHEEL_CMD,
    SPI_CH_MAX
} Spi_ChannelType_e;
/**
 * @brief ID logic của một SPI Job.
 * @details Job là đơn vị giao dịch chứa một hoặc nhiều Channel và được
 *          Sequence tham chiếu theo thứ tự thực thi.
 * @implements Quy ước đặt tên: SPI_JOB_<Tên_Thiết_Bị>_<Tác_Vụ>.
 */
typedef enum Spi_JobType
{
    SPI_JOB_SEND_LED_CMD = 0u,
    SPI_JOB_1,
    SPI_JOB_MAX
} Spi_JobType_e;
/**
 * @brief ID logic của một SPI Sequence.
 * @details Sequence chứa danh sách có thứ tự các Job cần thực thi.
 * @implements Quy ước đặt tên: SPI_SEQ_<Nghiệp_Vụ_Hệ_Thống>.
 */
typedef enum Spi_SequenceType
{
    SPI_SEQ_UPDATE_LED_STATUS = 0u,
    SPI_SEQ_UPDATE_LCD,
    SPI_SEQ_MAX
} Spi_SequenceType_e;

/**
 * @brief Phương thức thực thi các Job thuộc Sequence.
 * @details SPI_POLLING_MODE thực hiện truyền/nhận theo kiểu chờ;
 *          SPI_INTERRUPT_MODE khởi chạy giao dịch để tiếp tục xử lý bằng ngắt.
 */
typedef enum
{
    SPI_POLLING_MODE = 1U,
    SPI_INTERRUPT_MODE
} Spi_TransferModeType;

/**
 * @brief Cơ chế lưu buffer dữ liệu của một Channel.
 * @details IB dùng vùng nhớ do driver quản lý; EB tham chiếu vùng nhớ do bên
 *          gọi cung cấp.
 */
typedef enum
{
    SPI_BUFFER_TYPE_IB = 1U, /* Internal Buffer do driver quản lý. */
    SPI_BUFFER_TYPE_EB       /* External Buffer do bên gọi cung cấp. */
} Spi_BufferType;

/**
 * @brief Kiểu hàm callback thông báo kết thúc giao dịch SPI.
 * @details Callback không nhận tham số; trong cấu hình hiện tại callback được
 *          khai báo tại Sequence và gọi khi Sequence hoàn tất.
 */
typedef void (*Spi_notificationType)(void);

#endif