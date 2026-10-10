/**
 * @file        Spi_Map.h
 * @brief       Kiểu cấu hình và API ánh xạ SPI MCAL sang STM32F10x SPL.
 * @details
 * 1. Các enum SPI_MR_* độc lập với các macro cấu hình SPL.
 * 2. Các hàm Spi_Map_* chuyển enum logic sang giá trị/peripheral SPL tương ứng.
 * 3. Header này phụ thuộc trực tiếp vào STM32 Standard Peripheral Library.
 */
#ifndef SPI_MAP_H
#define SPI_MAP_H

#include "./Bsw/Services/Common/Std_Types.h"
#include "stm32f10x_spi.h"
#include "Spi_Types.h"

/** @brief Bộ chia clock SPI; hệ số chia tương ứng từ 2 đến 256. */
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

/** @brief Độ rộng mỗi phần tử dữ liệu trên bus SPI. */
typedef enum
{
    SPI_MR_DATASIZE_8B = 0U,
    SPI_MR_DATASIZE_16B
} Spi_DataSizeType;

/** @brief Vai trò của SPI peripheral trên bus: master hoặc slave. */
typedef enum
{
    SPI_MR_MODE_MASTER = 0U,
    SPI_MR_MODE_SLAVE
} Spi_ModeType;

/** @brief Cấu hình số đường dữ liệu và hướng truyền/nhận của SPI. */
typedef enum
{
    SPI_MR_2LINES_FD = 0U,
    SPI_MR_2LINES_RX_ONLY,
    SPI_MR_1LINE_RX,
    SPI_MR_1LINE_TX
} Spi_DirectionType;

/** @brief Nguồn điều khiển chân NSS: phần mềm hoặc phần cứng. */
typedef enum
{
    SPI_MR_NSS_SOFT = 0U,
    SPI_MR_NSS_HARD
} Spi_NssType;

/** @brief Mức cực tính idle của tín hiệu clock SCK. */
typedef enum
{
    SPI_MR_CPOL_LOW = 0U,
    SPI_MR_CPOL_HIGH
} Spi_CpolType;

/** @brief Cạnh SCK dùng để lấy mẫu dữ liệu, kết hợp với CPOL tạo SPI mode. */
typedef enum
{
    SPI_MR_CPHA_1EDGE = 0U,
    SPI_MR_CPHA_2EDGE
} Spi_CphaType;

/** @brief Thứ tự dịch bit dữ liệu: MSB trước hoặc LSB trước. */
typedef enum
{
    SPI_MR_FIRSTBIT_MSB = 0U,
    SPI_MR_FIRSTBIT_LSB
} Spi_FirstBitType;

/**
 * @brief ID logic của các nguồn interrupt SPI cần ánh xạ sang SPL.
 * @details TXE báo thanh ghi truyền trống; RXNE báo có dữ liệu nhận; ERROR
 *          đại diện nhóm nguồn lỗi SPI được SPL hỗ trợ.
 */
typedef enum
{
    Spi_IRQ_SOURCE_TXE = 0U, // Thanh ghi truyền trống, có thể ghi phần tử tiếp theo.
    Spi_IRQ_SOURCE_RXNE,     // Có dữ liệu mới trong thanh ghi nhận.
    Spi_IRQ_SOURCE_ERROR,    // Nhóm nguồn ngắt lỗi SPI.
    Spi_IRQ_SOURCE_MAX
} Spi_IrqSourceType;

/**
 * @brief Ánh xạ vai trò master/slave sang hằng số SPL.
 * @param Mode Vai trò SPI theo enum MCAL.
 * @return Giá trị SPI_Mode_* tương ứng; giá trị không hợp lệ dùng mặc định
 *         master.
 */
uint16 Spi_Map_GetMode(Spi_ModeType Mode);

/**
 * @brief Ánh xạ cấu hình NSS sang hằng số SPL.
 * @param Nss Nguồn điều khiển NSS theo enum MCAL.
 * @return Giá trị SPI_NSS_* tương ứng; giá trị không hợp lệ dùng NSS mềm.
 */
uint16 Spi_Map_GetNss(Spi_NssType Nss);

/**
 * @brief Ánh xạ hướng truyền/nhận sang hằng số SPL.
 * @param Dir Hướng truyền theo enum MCAL.
 * @return Giá trị SPI_Direction_* tương ứng; mặc định full-duplex.
 */
uint16 Spi_Map_GetDir(Spi_DirectionType Dir);

/**
 * @brief Ánh xạ hệ số chia baud rate sang hằng số SPL.
 * @param BaudRate Hệ số chia clock SPI theo enum MCAL.
 * @return Giá trị SPI_BaudRatePrescaler_* tương ứng.
 */
uint16 Spi_Map_GetBaudRate(Spi_BaudRateType BaudRate);

/**
 * @brief Ánh xạ độ rộng dữ liệu sang hằng số SPL.
 * @param DataSize Độ rộng 8-bit hoặc 16-bit.
 * @return Giá trị SPI_DataSize_* tương ứng; mặc định 8-bit.
 */
uint16 Spi_Map_GetDataSize(Spi_DataSizeType DataSize);

/**
 * @brief Ánh xạ cực tính clock CPOL sang hằng số SPL.
 * @param Cpol Cực tính clock theo enum MCAL.
 * @return Giá trị SPI_CPOL_* tương ứng; mặc định mức thấp.
 */
uint16 Spi_Map_GetCpol(Spi_CpolType Cpol);

/**
 * @brief Ánh xạ pha clock CPHA sang hằng số SPL.
 * @param Cpha Cạnh lấy mẫu theo enum MCAL.
 * @return Giá trị SPI_CPHA_* tương ứng; mặc định cạnh thứ nhất.
 */
uint16 Spi_Map_GetCpha(Spi_CphaType Cpha);

/**
 * @brief Ánh xạ thứ tự bit sang hằng số SPL.
 * @param FirstBit Thứ tự bit theo enum MCAL.
 * @return Giá trị SPI_FirstBit_* tương ứng; mặc định MSB.
 */
uint16 Spi_Map_GetFirstBit(Spi_FirstBitType FirstBit);

/**
 * @brief Lấy địa chỉ peripheral ứng với ID Hardware Unit logic.
 * @param HwUnitId ID SPI Hardware Unit.
 * @return SPI1/SPI2 tương ứng hoặc NULL_PTR nếu ID không được hỗ trợ.
 */
SPI_TypeDef *Spi_Map_GetHwInstance(Spi_HwUnitType_e HwUnitId);

/**
 * @brief Ánh xạ nguồn ngắt logic sang cờ interrupt SPL.
 * @param IrqSource Nguồn ngắt theo enum MCAL.
 * @return Cờ SPI_I2S_IT_* tương ứng; 0U nếu nguồn không hợp lệ.
 */
uint32_t Spi_MapIrqSourceToSplFlag(Spi_IrqSourceType IrqSource);
#endif /* SPI_MAP_H */