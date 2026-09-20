/**
 * @file        Spi_Map.h
 * @brief       Hardware Mapping Layer for SPI MCAL Driver (STM32F103 SPL)
 */
#ifndef SPI_MAP_H
#define SPI_MAP_H

#include "./Bsw/Services/Common/Std_Types.h"
#include "stm32f10x_spi.h"
#include "Spi_Types.h"
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
SPI_TypeDef *Spi_Map_GetHwInstance(Spi_GroupId_Type HwUnitId);

#endif /* SPI_MAP_H */