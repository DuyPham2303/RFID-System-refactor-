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
#include "Mcu_IrqTypes.h"

#define MAX_CHANS_PER_JOB 4 // số lượng channel tối đa mà 1 tác vu (job) có thể xử lý
#define MAX_JOBS_PER_SEQ 4  //  số lượng channel tối đa mà 1 nghiệp vu (sequence)) có thể xử lý

/**
 * @brief Cấu hình phần cứng của một SPI unit.
 * @details Chứa các tham số cần thiết để khởi tạo peripheral SPI. Bảng cấu
 *          hình được xem là tĩnh và không được thay đổi sau Spi_Init().
 * @note Spi_GroupId_Type và các giá trị tham số hiện vẫn phụ thuộc vào STM32 SPL;
 *       application nên truy cập thông qua lớp MCAL/IoHwAb.
 */
typedef struct Spi_Config
{
    Spi_GroupId_Type HwId;
    Spi_BaudRateType BaudRatePrescaler;
    Spi_DataSizeType DataSize;
    Spi_CpolType CPOL;
    Spi_CphaType CPHA;
    Spi_FirstBitType FirstBit;
    Spi_NssType Nss;
    Spi_ModeType Mode;
    Spi_DirectionType Dir;
    const Mcu_IrqConfigType *IrqCfgPtr;
} Spi_ConfigType;

/**
 * @brief Trạng thái runtime của SPI driver.
 * @details Lưu trạng thái driver, Channel hiện tại và trạng thái truyền/nhận
 *          để phục vụ xử lý đồng bộ hoặc bất đồng bộ.
 */
typedef struct Spi_Runtime
{
    Spi_StatusType Status;
    boolean TxBusy;
    boolean RxBusy;
} Spi_RuntimeType;

/**
 * @brief Cấu hình dữ liệu của một SPI Channel.
 * @details Mỗi Channel quản lý buffer truyền, buffer nhận và số byte cần
 *          truyền. Channel là đơn vị dữ liệu logic được một Job tham chiếu.
 * @note Chỉ cần biết đang trỏ vào buffer nào và truyền bao nhiêu byte:
 */
typedef struct Spi_ChannelConfig
{
    uint16 *TxBuffer;
    uint16 *RxBuffer;
    uint16 dataWidthByte; /*số byte tôi đa được phép trao đổi trong 1 phiên truyền/nhận*/
} Spi_ChannelConfigType;

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
    uint16 CsPinId;
} Spi_JobConfigType;

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

/**
 * @brief Bảng cấu hình phần cứng SPI.
 * @details Mỗi phần tử mô tả một SPI hardware unit và các tham số khởi tạo
 *          được sử dụng bởi Spi_Init().
 */
extern const Spi_ConfigType Spi_ConfigSet[SPI_CHANNEL_MAX];
#endif