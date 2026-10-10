/**
 * @file Spi_Cfg.h
 * @brief Khai báo cấu hình tĩnh của SPI MCAL.
 * @details Header khai báo kiểu dữ liệu cấu hình cho Hardware Unit, Channel,
 *          Job và Sequence, cùng bộ cấu hình tĩnh do module cấu hình cung cấp.
 *          Application sử dụng ID logic; việc ánh xạ phần cứng và thứ tự giao
 *          dịch do MCAL quản lý.
 *
 * @version     1.0.0
 * @date        2026
 * @author      Pham Cao Duy
 */
#ifndef __SPI_CFG_H
#define __SPI_CFG_H

#include "Spi_Types.h"
#include "Spi_map.h"
#include "Mcu_IrqTypes.h"
#include "Dio_PinMode_Cfg.h"

/** @brief Số Channel tối đa được liệt kê trong một Job. */
#define SPI_MAX_CH_PER_JOB 4
/** @brief Số Job tối đa được liệt kê trong một Sequence. */
#define SPI_MAX_JOBS_PER_SEQ 4

/**
 * @brief Cấu hình tĩnh cho một Channel logic.
 * @details Channel xác định loại buffer và độ dài mặc định; không xác định
 *          Hardware Unit hay chân CS.
 */
typedef struct
{
    uint16 DefaultLength;      /* Độ dài mặc định tính theo phần tử dữ liệu. */
    Spi_BufferType BufferType; /* Cơ chế buffer: IB hoặc EB. */
} Spi_ChannelConfigType_s;
/**
 * @brief Cấu hình tĩnh của một Job SPI.
 * @details Job tham chiếu các Channel theo thứ tự và gắn chúng với một
 *          Hardware Unit cùng chân CS.
 * 1. ChannelList chứa ID logic của các Channel.
 * 2. ActiveTotalChIncurrentJob cho biết số phần tử đầu danh sách có hiệu lực.
 * 3. CsPinId và HwId xác định chân chọn thiết bị và peripheral thực thi Job.
 */
typedef struct Spi_JobConfig
{
    Spi_ChannelType_e ChannelList[SPI_MAX_CH_PER_JOB]; /* Danh sách Channel theo thứ tự. */
    Dio_ChannelModeType CsPinId;                      /* ID chân Chip Select của Job. */
    Spi_HwUnitType_e HwId;                             /* Hardware Unit dùng để chạy Job. */
    uint8 ActiveTotalChIncurrentJob;                   /* Số Channel hợp lệ trong ChannelList. */
} Spi_JobConfigType_s;
/**
 * @brief Cấu hình tĩnh của một Sequence.
 * @details
 * 1. JobList chứa các ID Job theo thứ tự thực thi.
 * 2. TotalJobIncurrentSequence xác định số phần tử hợp lệ trong JobList.
 * 3. SeqNoti là callback tùy chọn được gọi khi Sequence kết thúc.
 */
typedef struct Spi_SequenceConfig
{
    Spi_JobType_e JobList[SPI_MAX_JOBS_PER_SEQ]; /* Danh sách Job theo thứ tự chạy. */
    uint8 TotalJobIncurrentSequence;              /* Số Job hợp lệ trong JobList. */
    Spi_notificationType SeqNoti;                 /* Callback kết thúc Sequence; NULL nếu không dùng. */
} Spi_SequenceConfigType_s;
/**
 * @brief Cấu hình tĩnh của một SPI Hardware Unit/Device.
 * @details Chứa tham số khởi tạo peripheral, hướng truyền và cấu hình NVIC.
 *          Bảng cấu hình được cung cấp cho Spi_Init().
 * 1. HwId xác định peripheral SPI.
 * 2. BaudRatePrescaler, DataSize, CPOL, CPHA, FirstBit, Nss, Mode và Dir
 *    xác định thuộc tính giao tiếp.
 * 3. NvicCfgPtr trỏ tới cấu hình NVIC tùy chọn; có thể NULL_PTR khi không dùng.
 */
typedef struct Spi_ExternalDeviceConfig
{
    Spi_HwUnitType_e HwId;                 /* Peripheral SPI được cấu hình. */
    Spi_BaudRateType BaudRatePrescaler;     /* Hệ số chia clock SPI. */
    Spi_DataSizeType DataSize;              /* Độ rộng dữ liệu 8-bit hoặc 16-bit. */
    Spi_CpolType CPOL;                      /* Cực tính clock ở trạng thái idle. */
    Spi_CphaType CPHA;                      /* Cạnh lấy mẫu dữ liệu. */
    Spi_FirstBitType FirstBit;              /* Thứ tự dịch bit. */
    Spi_NssType Nss;                        /* Quản lý NSS bằng phần mềm/phần cứng. */
    Spi_ModeType Mode;                      /* Vai trò master hoặc slave. */
    Spi_DirectionType Dir;                  /* Hướng và số đường dữ liệu. */
    const Mcu_NvicConfigType_s *NvicCfgPtr; /* Cấu hình NVIC tùy chọn. */
} Spi_ExternalDeviceConfigType_s;

/**
 * @brief Bộ cấu hình tĩnh đầy đủ của SPI Driver.
 * @details Mỗi con trỏ trỏ tới một mảng cấu hình có số phần tử tương ứng
 *          trong trường Count; các bảng phải còn hiệu lực trong thời gian
 *          driver sử dụng.
 */
typedef struct
{
    const Spi_ExternalDeviceConfigType_s *DeviceConfigPtr; /* Bảng Hardware Unit/Device. */
    uint8 DeviceCount;                                     /* Số cấu hình Device. */

    const Spi_ChannelConfigType_s *ChannelConfigPtr;       /* Bảng Channel logic. */
    uint8 ChannelCount;                                    /* Số cấu hình Channel. */

    const Spi_JobConfigType_s *JobConfigPtr;               /* Bảng Job. */
    uint8 JobCount;                                        /* Số cấu hình Job. */

    const Spi_SequenceConfigType_s *SequenceConfigPtr;     /* Bảng Sequence. */
    uint8 SequenceCount;                                   /* Số cấu hình Sequence. */
} Spi_ConfigType_s;

/** @brief Bộ cấu hình SPI tĩnh mặc định được cung cấp bởi cấu hình MCAL. */
extern const Spi_ConfigType_s g_Spi_ConfigSet;
#endif