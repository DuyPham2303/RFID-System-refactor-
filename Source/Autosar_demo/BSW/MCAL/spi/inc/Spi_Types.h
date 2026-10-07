/**
 * @file        Spi_Types.h
 * @brief       SPI Driver Type Definitions.
 * @details
 * This file contains all type definitions required by the SPI MCAL driver.
 *
 * The file defines:
 * - SPI Param configuration structures & enumeration
 * - Channel, Job and Sequence abstractions
 * - Notification callback types & IRQ
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
#include "Dio.h"

/**
 * @brief Id logic ánh xạ tới địa chỉ cứng của bộ SPI khi cấu hình
 *
 */
typedef enum Spi_GroupId
{
    SPI_GROUP_1 = 0U,
    SPI_GROUP_2,
    SPI_GROUP_3,
    SPI_MAX_GROUP
} Spi_GroupId_Type;
/**
 * @brief ID logic của một SPI Channel.
 * @details Channel đại diện cho một luồng dữ liệu logic và không chứa
 *          thông tin phần cứng cụ thể của vi điều khiển.
 * @implements Cách đặt tên : SPI_CHANNEL_<Tên_Thiết_Bị>_<Chức_Năng>
 *                            (Ví dụ: SPI_CHANNEL_SENSOR_CMD)
 */
typedef enum Spi_ChannelId
{
    SPI_CHANNEL_LED_CMD = 0U,
    SPI_CHANNEL_SENSOR_READ,
    SPI_CHANNEL_FAN_CMD,
    SPI_CHANNEL_SPEED_CMD,
    SPI_CHANNEL_WHEEL_CMD,
    SPI_MAX_CHANNEL
} Spi_ChannelId_Type;

/**
 * @brief ID logic của một SPI Job.
 * @details Job là một giao dịch SPI hoàn chỉnh, có thể chứa một hoặc nhiều
 *          Channel và được tham chiếu bởi một Sequence.
 * @implements Cách đặt tên : SPI_JOB_<Tên_Thiết_Bị>_<Tác_Vụ>
 *                            (Ví dụ: SPI_JOB_READ_SENSOR)
 */
typedef enum Spi_JobId
{
    SPI_JOB_SEND_LED_CMD = 0u,
    SPI_JOB_1,
    SPI_JOB_2,
    SPI_JOB_3,
    SPI_JOB_MAX
} Spi_JobId_Type;
/**
 * @brief ID logic của một SPI Sequence.
 * @details Sequence mô tả thứ tự thực thi của một hoặc nhiều Job.
 * @implements Cách đặt tên : SPI_SEQ_<Nghiệp_Vụ_Hệ_Thống>
 *             (Ví dụ: SPI_SEQ_UPDATE_SENSOR_DATA
 */
typedef enum Spi_SequenceId
{
    SPI_SEQ_UPDATE_LED_STATUS = 0u,
    SPI_SEQ_UPDATE_LCD,
    SPI_SEQ_MAX
} Spi_SequenceId_Type;
/**
 * @brief Phương thức truyền dữ liệu của SPI driver.
 */
typedef enum
{
    SPI_POLLING_MODE = 0U,
    SPI_INTERRUPT_MODE
} Spi_TransferModeType;

#define MAX_CHANS_PER_JOB 4 // số lượng channel tối đa mà 1 tác vu (job) có thể xử lý

/**
 * @brief Cấu hình của một SPI Job.
 * @details Job biểu diễn một giao dịch SPI hoàn chỉnh trên một hardware unit
 *          và có thể chứa một hoặc nhiều Channel theo đúng quan hệ AUTOSAR:
 *          một Job tham chiếu danh sách Channel ID.
 * @note ChannelList chỉ chứa ID logic, bản ghi cấu hình Channel tương ứng
 *       được lưu trong bảng Spi_ChannelConfig.
 *       (Chỉ cần biết dùng chân Chip Select nào và quản lý channel nào:)
 */
typedef struct Spi_JobConfig
{
    Spi_ChannelId_Type ChannelList[MAX_CHANS_PER_JOB]; // Mảng tĩnh chứa danh sách channel
    uint8 ActiveChannelCount;                          // Số lượng channel thực tế đang dùng
    Dio_ChannelType CsPinId;
    Spi_GroupId_Type HwId;
} Spi_JobConfigType;

#define MAX_JOBS_PER_SEQ 4 //  số lượng channel tối đa mà 1 nghiệp vu (sequence)) có thể xử lý

/**
 * @brief Cấu hình của một SPI Sequence.
 * @details Sequence là một danh sách có thứ tự gồm một hoặc nhiều Job ID.
 *          Các Job trong danh sách được thực thi tuần tự.
 * @note JobList chỉ chứa ID logic; bản ghi Job tương ứng được tra cứu trong
 *       bảng Spi_JobConfig.
 */
typedef struct Spi_SequenceConfig
{
    Spi_JobId_Type JobList[MAX_JOBS_PER_SEQ]; // Mảng tĩnh chứa danh sách các Job thuộc Sequence này
    uint8 ActiveJobCount;                     // Số lượng Job thực tế sẽ được thực thi tuần tự
} Spi_SequenceConfigType;

#endif