#ifndef IOHWAB_SPI_H
#define IOHWAB_SPI_H

#include "./Bsw/Services/Common/Std_Types.h"

/* ============================================================
 * LED commands
 * ============================================================ */

#define IOHWAB_SPI_LED_OFF 0x00
#define IOHWAB_SPI_LED_GREEN 0x12
#define IOHWAB_SPI_LED_ORANGE 0x37
#define IOHWAB_SPI_LED_RED 0xA8
#define IOHWAB_SPI_LED_BLUE 0xB9
#define IOHWAB_SPI_LED_ALL 0xC1

/* ============================================================
 * Runtime status
 * ============================================================ */

typedef enum
{
    IOHWAB_SPI_STATUS_IDLE = 0u,
    IOHWAB_SPI_STATUS_BUSY,
    IOHWAB_SPI_STATUS_COMPLETE,
    IOHWAB_SPI_STATUS_ERROR
} IoHwAb_SpiStatusType;

Std_ReturnType IoHwAb_SpiInit(void);
/* ============================================================
 * Synchronous API
 * ============================================================ */

Std_ReturnType IoHwAb_Spi_SetLedStatus(uint8 LedStatus);

/* ============================================================
 * Asynchronous API
 * ============================================================ */

Std_ReturnType IoHwAb_Spi_StartLedStatus(uint8 LedStatus);

/* ============================================================
 * Runtime status API
 * ============================================================ */

IoHwAb_SpiStatusType IoHwAb_Spi_GetStatus(void);

/* ============================================================
 * MCAL callback
 * ============================================================ */

void IoHwAb_Spi_LedSequenceComplete(void);

#endif