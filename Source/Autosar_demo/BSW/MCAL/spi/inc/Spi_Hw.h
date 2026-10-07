/**
 * @file Spi_Hw.h
 * @brief Header file cho tầng phần cứng thấp cấp (Low-Level Hardware Abstraction) của SPI Driver.
 */

#ifndef SPI_HW_H
#define SPI_HW_H

#include "Spi_Types.h"
#include "Spi_map.h"

/* ==========================================================
 * PROTOTYPE CỦA HÀM XỬ LÝ TRUYỀN NHẬN (IMPLEMENT TRONG Spi_Hw.c)
 * ========================================================== */
/**
 *
 */
typedef struct Spi_Hw_dataConfig
{
    Spi_GroupId_Type HwId;
    Spi_DirectionType direction;
    Spi_DataSizeType SizeType;
    const void *pTxData;
    void *pRxData;
    uint8 length
} Spi_Hw_dataConfigType;
/**
 * @brief Hàm truyền nhận đồng thời (Full-Duplex) hỗ trợ cho Spi_SyncTransmit.
 */
void Spi_Hw_Sync_TransmitReceive(const Spi_Hw_dataConfigType *HwDataCfgPtr);
void Spi_Hw_Async_TransmitReceive();
#endif /* SPI_HW_H */