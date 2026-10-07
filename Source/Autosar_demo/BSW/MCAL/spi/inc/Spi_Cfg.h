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

// #include "Spi_Runtime.h"
#include "Spi_Internal.h" // typedef trung gian để ánh xạ tới macro địa chỉ thực tế
#include "Spi_map.h"      // API trung gian để ánh xạ tới macro địa chỉ thực tế
#include "Mcu_IrqTypes.h" // typedef trung gian để ánh xạ tới macro địa chỉ thực tế

/**
 * @brief Cấu hình phần cứng của một SPI unit.
 * @details Chứa các tham số cần thiết để khởi tạo peripheral SPI. Bảng cấu
 *          hình được xem là tĩnh và không được thay đổi sau Spi_Init().
 * @note Spi_GroupId_Type và các giá trị tham số hiện vẫn phụ thuộc vào STM32 SPL;
 *       application nên truy cập thông qua lớp MCAL/IoHwAb.
 */
typedef struct Spi_ParanConfig
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
    const Mcu_NvicConfigType *NvicCfgPtr;
    const Spi_AsyncSeqCfgType *AsyncNotiPtr;
} Spi_ParamConfigType;

typedef struct Spi_ConfigTypes_s
{
    Spi_ParamConfigType *ChannelParamCfgPtr;
    uint8 SpiCfgCount;
} Spi_ConfigTypes_s;

/**
 * @brief Bảng cấu hình phần cứng SPI.
 * @details Mỗi phần tử mô tả một SPI hardware unit và các tham số khởi tạo
 *          được sử dụng bởi Spi_Init().
 */
extern const Spi_ConfigTypes_s Spi_ConfigSet;

#endif