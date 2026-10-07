/**
 * @file        Spi_Map.h
 * @brief       Hardware Mapping Layer for SPI MCAL Driver (STM32F103 SPL)
 */
#ifndef SPI_MAP_H
#define SPI_MAP_H

#include "./Bsw/Services/Common/Std_Types.h"
#include "stm32f10x_spi.h"
#include "Spi_Types.h"

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
 * @brief Ánh xạ Mode từ MCAL sang SPL Macro
 */
uint16 Spi_Map_GetMode(Spi_ModeType Mode);

/**
 * @brief Ánh xạ tín hiệu NSS từ MCAL sang SPL Macro
 */
uint16 Spi_Map_GetNss(Spi_NssType Nss);

/**
 * @brief Ánh xạ hướng truyền từ MCAL sang SPL Macro
 */
uint16 Spi_Map_GetDir(Spi_DirectionType Dir);

/**
 * @brief Ánh xạ Baudrate từ MCAL sang SPL Macro
 */
uint16 Spi_Map_GetBaudRate(Spi_BaudRateType BaudRate);

/**
 * @brief Ánh xạ Data Size từ MCAL sang SPL Macro
 */
uint16 Spi_Map_GetDataSize(Spi_DataSizeType DataSize);

/**
 * @brief Ánh xạ CPOL từ MCAL sang SPL Macro
 */
uint16 Spi_Map_GetCpol(Spi_CpolType Cpol);

/**
 * @brief Ánh xạ CPHA từ MCAL sang SPL Macro
 */
uint16 Spi_Map_GetCpha(Spi_CphaType Cpha);

/**
 * @brief Ánh xạ First Bit (MSB/LSB) từ MCAL sang SPL Macro
 */
uint16 Spi_Map_GetFirstBit(Spi_FirstBitType FirstBit);

/**
 * @brief Ánh xạ từ ID phần cứng logic sang con trỏ thanh ghi SPI_TypeDef của ST
 */
SPI_TypeDef *Spi_Map_GetHwInstance(Spi_HwUnitType_e HwUnitId);

/**
 * @brief Hàm ánh xạ từ nguồn ngắt logic sang macro cờ ngắt của STM32 SPL
 */
uint32_t Spi_MapIrqSourceToSplFlag(Spi_IrqSourceType IrqSource);
#endif /* SPI_MAP_H */