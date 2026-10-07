/**
 * @file        Spi_Types.h
 * @brief       SPI Driver Type Definitions.
 * @details
 * This file contains all type definitions required by the SPI MCAL driver.
 *
 * The file defines:
 * - SPI Param configuration structures & enumeration
 * - Channel, Job and Sequence abstractions
 *
 * These definitions are shared between Spi.h, Spi.c and Spi_Cfg.c.
 *
 * @version     1.0.0
 * @date        2026
 * @author      Pham Cao Duy
 */
#ifndef SPI_TYPES_H
#define SPI_TYPES_H

#include "./Bsw/Services/Common/Std_Types.h"
/**
 * @brief Id logic ánh xạ tới địa chỉ cứng của bộ SPI khi cấu hình
 *
 */
typedef enum Spi_HwUnitType_e
{
    SPI_HW_UNIT_1 = 0U,
    SPI_HW_UNIT_2,
    SPI_HW_MAX_UNIT
} Spi_HwUnitType_e;
/**
 * @brief ID logic của một SPI Channel.
 * @details Channel đại diện cho một luồng dữ liệu logic và không chứa
 *          thông tin phần cứng cụ thể của vi điều khiển.
 * @implements Cách đặt tên : SPI_CH_<Tên_Thiết_Bị>_<Chức_Năng>
 *                            (Ví dụ: SPI_CH_SENSOR_CMD)
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
 * @details Job là một giao dịch SPI hoàn chỉnh, có thể chứa một hoặc nhiều
 *          Channel và được tham chiếu bởi một Sequence.
 * @implements Cách đặt tên : SPI_JOB_<Tên_Thiết_Bị>_<Tác_Vụ>
 *                            (Ví dụ: SPI_JOB_READ_SENSOR)
 */
typedef enum Spi_JobType
{
    SPI_JOB_SEND_LED_CMD = 0u,
    SPI_JOB_1,
    SPI_JOB_MAX
} Spi_JobType_e;
/**
 * @brief ID logic của một SPI Sequence.
 * @details Sequence mô tả thứ tự thực thi của một hoặc nhiều Job.
 * @implements Cách đặt tên : SPI_SEQ_<Nghiệp_Vụ_Hệ_Thống>
 *             (Ví dụ: SPI_SEQ_UPDATE_SENSOR_DATA
 */
typedef enum Spi_SequenceType
{
    SPI_SEQ_UPDATE_LED_STATUS = 0u,
    SPI_SEQ_UPDATE_LCD,
    SPI_SEQ_MAX
} Spi_SequenceType_e;

/**
 * @brief Phương thức truyền dữ liệu của SPI driver.
 */
typedef enum
{
    SPI_POLLING_MODE = 0U,
    SPI_INTERRUPT_MODE
} Spi_TransferModeType;

typedef enum
{
    SPI_BUFFER_TYPE_IB = 0, // Internal Buffer
    SPI_BUFFER_TYPE_EB      // External Buffer
} Spi_BufferType;

/**
 * @brief Kiểu callback thông báo hoàn thành SPI.
 * @details Callback do tầng trên đăng ký và được SPI driver gọi sau khi một
 *          Job hoặc Sequence hoàn tất.
 */
typedef void (*Spi_notificationType)(void);

#endif