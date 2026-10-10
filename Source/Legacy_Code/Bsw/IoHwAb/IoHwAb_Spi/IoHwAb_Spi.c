#include "IoHwAb_Spi.h"
#include "Spi.h"

/* ============================================================
 * Runtime buffers
 *
 * Must remain valid while asynchronous SPI transfer is running.
 * ============================================================ */

static uint8 s_LedTxBuffer[] = {0x10, 0x12, 0x13, 0x14, 0x00};
static uint8 s_fanTxBuffer[] = {0x12, 0x24, 0x21, 0x44, 0x00};
static uint8 s_sensorTxBuffer[] = {0x12, 0x89, 0x21, 0x56, 0x56, 0x43, 0x00};
static uint8 s_speedTxBuffer[] = {0x21, 0x24, 0x21, 0x87, 0x00};
static uint8 s_wheelTxBuffer[] = {0x12, 0x24, 0x00};

/* ============================================================
 * Runtime state
 * ============================================================ */

volatile IoHwAb_SpiStatusType s_SpiStatus = IOHWAB_SPI_STATUS_IDLE;

static Std_ReturnType IoHwAb_Spi_SetupSequenceBuffers()
{
    if (Spi_SetupEB(SPI_CH_LED_CMD, s_LedTxBuffer, NULL_PTR, sizeof(s_LedTxBuffer)) != E_OK ||
        Spi_SetupEB(SPI_CH_SENSOR_READ, s_sensorTxBuffer, NULL_PTR, sizeof(s_sensorTxBuffer)) != E_OK ||
        Spi_SetupEB(SPI_CH_FAN_CMD, s_fanTxBuffer, NULL_PTR, sizeof(s_fanTxBuffer)) != E_OK ||
        Spi_SetupEB(SPI_CH_WHEEL_CMD, s_wheelTxBuffer, NULL_PTR, sizeof(s_wheelTxBuffer)) != E_OK ||
        Spi_SetupEB(SPI_CH_SPEED_CMD, s_speedTxBuffer, NULL_PTR, sizeof(s_speedTxBuffer)) != E_OK)
    {
        return E_NOT_OK;
    }

    return E_OK;
}

Std_ReturnType IoHwAb_SpiInit(void)
{
    if (Spi_Init(&g_Spi_ConfigSet) != E_OK)
    {
        s_SpiStatus = IOHWAB_SPI_STATUS_ERROR;
        return E_NOT_OK;
    }

    s_SpiStatus = IOHWAB_SPI_STATUS_IDLE;

    if (IoHwAb_Spi_SetupSequenceBuffers() != E_OK)
    {
        s_SpiStatus = IOHWAB_SPI_STATUS_ERROR;

        return E_NOT_OK;
    }
    return E_OK;
}

/* ============================================================
 * Synchronous transmission
 * ============================================================ */

Std_ReturnType IoHwAb_Spi_SetLedStatus(uint8 LedStatus)
{
    if (Spi_SyncTransmit(SPI_SEQ_UPDATE_LED_STATUS) != E_OK)
    {
        s_SpiStatus = IOHWAB_SPI_STATUS_ERROR;

        return E_NOT_OK;
    }

    s_SpiStatus = IOHWAB_SPI_STATUS_COMPLETE;

    return E_OK;
}

Std_ReturnType IoHwAb_Spi_StartLedStatus(uint8 LedStatus)
{
    if (s_SpiStatus == IOHWAB_SPI_STATUS_BUSY)
    {
        return E_NOT_OK;
    }

    s_SpiStatus = IOHWAB_SPI_STATUS_BUSY;

    if (Spi_AsyncTransmit(SPI_SEQ_UPDATE_LED_STATUS) != E_OK)
    {
        s_SpiStatus = IOHWAB_SPI_STATUS_ERROR;

        return E_NOT_OK;
    }

    return E_OK;
}
void IoHwAb_Spi_LedSequenceComplete(void)
{
    s_SpiStatus = IOHWAB_SPI_STATUS_COMPLETE;
}