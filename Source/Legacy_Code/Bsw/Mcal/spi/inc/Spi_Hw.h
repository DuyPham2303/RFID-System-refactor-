/**
 * @file Spi_Hw.h
 * @brief Header file cho tầng phần cứng thấp cấp (Low-Level Hardware Abstraction) của SPI Driver.
 */

#ifndef SPI_HW_H
#define SPI_HW_H

#include "Spi_Types.h"
#include "Spi_map.h"
/**
 * @brief Hàm truyền nhận đồng thời (Full-Duplex) hỗ trợ cho Spi_SyncTransmit.
 */
void Spi_Hw_Sync_TransmitReceive(uint16 *TxPtr,
                                 uint16 *RxPtr,
                                 uint8 length,
                                 Spi_HwUnitType_e HwId,
                                 Spi_DirectionType direction,
                                 Spi_DataSizeType datasize);

void Spi_Hw_StartFirstByteInterrupt(
    SPI_TypeDef *Spix,
    const uint16 *TxPtr,
    uint16 *RxPtr,
    Spi_DirectionType direction,
    Spi_DataSizeType datasize);
#endif /* SPI_HW_H */