/**
 * @file        Spi_Types.h
 * @brief       SPI Driver Type Definitions.
 * @details
 * This file contains all type definitions required by the SPI MCAL driver.
 *
 * The file defines:
 * - SPI configuration structures
 * - Runtime state structures
 * - Channel, Job and Sequence abstractions
 * - Driver status definitions
 * - Notification callback types
 * - Params configuration
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
typedef enum Spi_GroupId
{
    SPI_GROUP_1 = 0U,
    SPI_GROUP_2,
    SPI_GROUP_3,
    SPI_GROUP_MAX
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
    SPI_CHANNEL_0 = 0U,
    SPI_CHANNEL_1,
    SPI_CHANNEL_2,
    SPI_CHANNEL_3,
    SPI_CHANNEL_4,
    SPI_CHANNEL_MAX
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
    SPI_JOB_0 = 0u,
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
    SPI_SEQUENCE_UPDATE_LCD,
    SPI_SEQ_MAX
} Spi_SequenceId_Type;

/**
 * @brief Trạng thái hiện tại của SPI driver.
 */
typedef enum
{
    SPI_UNINIT = 0U,
    SPI_IDLE,
    SPI_BUSY
} Spi_StatusType;

/**
 * @brief Phương thức truyền dữ liệu của SPI driver.
 */
typedef enum
{
    SPI_POLLING_MODE = 0U,
    SPI_INTERRUPT_MODE
} Spi_TransferModeType;

/* --- 1. ĐỊNH NGHĨA CÁC KIỂM TRA TRỪU TƯỢNG (MCAL Independent Types) --- */

typedef enum
{
    SPI_MR_BAUDRATE_2 = 0U,
    SPI_MR_BAUDRATE_4,
    SPI_MR_BAUDRATE_8,
    SPI_MR_BAUDRATE_16,
    SPI_MR_BAUDRATE_32,
    SPI_MR_BAUDRATE_64,
    SPI_MR_BAUDRATE_128,
    SPI_MR_BAUDRATE_256
} Spi_BaudRateType;

typedef enum
{
    SPI_MR_DATASIZE_8B = 0U,
    SPI_MR_DATASIZE_16B
} Spi_DataSizeType;

typedef enum
{
    SPI_MR_MODE_MASTER = 0U,
    SPI_MR_MODE_SLAVE
} Spi_ModeType;

typedef enum
{
    SPI_MR_2LINES_FD = 0U,
    SPI_MR_2LINES_RX_ONLY,
    SPI_MR_1LINE_RX,
    SPI_MR_1LINE_TX
} Spi_DirectionType;

typedef enum
{
    SPI_MR_NSS_SOFT = 0U,
    SPI_MR_NSS_HARD
} Spi_NssType;

typedef enum
{
    SPI_MR_CPOL_LOW = 0U,
    SPI_MR_CPOL_HIGH
} Spi_CpolType;

typedef enum
{
    SPI_MR_CPHA_1EDGE = 0U,
    SPI_MR_CPHA_2EDGE
} Spi_CphaType;

typedef enum
{
    SPI_MR_FIRSTBIT_MSB = 0U,
    SPI_MR_FIRSTBIT_LSB
} Spi_FirstBitType;

#endif