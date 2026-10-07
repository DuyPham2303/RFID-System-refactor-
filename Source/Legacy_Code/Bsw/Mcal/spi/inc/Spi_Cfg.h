/**
 * @file Spi_Cfg.h
 * @brief Khai báo cấu hình tĩnh của SPI MCAL.
 * @details File này khai báo các bảng cấu hình được định nghĩa trong
 *          Spi_Cfg.c, bao gồm cấu hình phần cứng, Channel, Job và Sequence.
 *          Application chỉ sử dụng các ID logic; việc ánh xạ tới SPI hardware
 *          và thứ tự giao dịch do MCAL quản lý.
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
#include "Dio.h"

#define SPI_MAX_CH_PER_JOB 4   // số lượng channel tối đa mà 1 tác vu (job) có thể xử lý
#define SPI_MAX_JOBS_PER_SEQ 4 //  số lượng channel tối đa mà 1 nghiệp vu (sequence)) có thể xử lý

/*Cấu hình Channel (Thuần túy logic dữ liệu) */
typedef struct
{
    uint16 DefaultLength;      /* Chiều dài dữ liệu mặc định */
    Spi_BufferType BufferType; /* IB (Internal) hay EB (External) */
} Spi_ChannelConfigType_s;
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
    Spi_ChannelType_e ChannelList[SPI_MAX_CH_PER_JOB]; // Mảng tĩnh chứa danh sách channel
    Dio_ChannelType CsPinId;
    Spi_HwUnitType_e HwId;
    uint8 ActiveTotalChIncurrentJob;
} Spi_JobConfigType_s;
/**
 * @brief Cấu hình của một SPI Sequence.
 * @details Sequence là một danh sách có thứ tự gồm một hoặc nhiều Job ID.
 *          Các Job trong danh sách được thực thi tuần tự.
 * @note JobList chỉ chứa ID logic; bản ghi Job tương ứng được tra cứu trong
 *       bảng Spi_JobConfig.
 */
typedef struct Spi_SequenceConfig
{
    Spi_JobType_e JobList[SPI_MAX_JOBS_PER_SEQ]; // Mảng tĩnh chứa danh sách các Job thuộc Sequence này
    uint8 TotalJobIncurrentSequence;
    Spi_notificationType SeqNoti;
} Spi_SequenceConfigType_s;
/**
 * @brief Cấu hình phần cứng của một SPI unit.
 * @details Chứa các tham số cần thiết để khởi tạo peripheral SPI. Bảng cấu
 *          hình được xem là tĩnh và không được thay đổi sau Spi_Init().
 * @note Spi_HwUnitType_e và các giá trị tham số hiện vẫn phụ thuộc vào STM32 SPL;
 *       application nên truy cập thông qua lớp MCAL/IoHwAb.
 */
typedef struct Spi_ExternalDeviceConfig
{
    Spi_HwUnitType_e HwId;
    Spi_BaudRateType BaudRatePrescaler;
    Spi_DataSizeType DataSize;
    Spi_CpolType CPOL;
    Spi_CphaType CPHA;
    Spi_FirstBitType FirstBit;
    Spi_NssType Nss;
    Spi_ModeType Mode;
    Spi_DirectionType Dir;
    const Mcu_NvicConfigType_s *NvicCfgPtr;
} Spi_ExternalDeviceConfigType_s;

/**
 * @brief Kiểu dữ liệu cấu hình đầy đủ Spi
 *
 */
typedef struct
{
    const Spi_ExternalDeviceConfigType_s *DeviceConfigPtr;
    uint8 DeviceCount;

    const Spi_ChannelConfigType_s *ChannelConfigPtr;
    uint8 ChannelCount;

    const Spi_JobConfigType_s *JobConfigPtr;
    uint8 JobCount;

    const Spi_SequenceConfigType_s *SequenceConfigPtr;
    uint8 SequenceCount;
} Spi_ConfigType_s;

extern const Spi_ConfigType_s g_Spi_ConfigSet;
#endif