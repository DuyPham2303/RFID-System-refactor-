#include "./Bsw/Services/Os/Os.h"
#include "./Bsw/Services/EcuM/EcuM.h"
#include "System/Board_Clock.h"
#include "./Bsw/IoHwAb/IoHwAb_Spi/IoHwAb_Spi.h"
#include "./Bsw/IoHwAb/IoHwAb_Port/IoHwAb_Port.h"
/* ============================================================
 * Test Case ID
 * ============================================================ */

#define SPI_TEST_INIT 1u
#define SPI_TEST_SYNC_ORANGE 2u
#define SPI_TEST_SYNC_GREEN 3u
#define SPI_TEST_SYNC_RED 4u
#define SPI_TEST_SYNC_BLUE 5u
#define SPI_TEST_SYNC_OFF 6u
#define SPI_TEST_ASYNC_GREEN 7u
#define SPI_TEST_ASYNC_RED 8u

/* ============================================================
 * Application Runtime Variables
 * ============================================================ */

volatile uint8 Spi_TestCase = 0u;

/* ============================================================
 * Runtime Result
 * ============================================================ */

volatile Std_ReturnType App_SpiResult;

/* ============================================================
 * Internal
 * ============================================================ */

static uint8 Spi_LastTestCase = 0u;

/* ============================================================
 * Main Test Function
 * ============================================================ */

void App_SpiTest_MainFunction(void)
{
    /*
     * Execute only when Debug Watch changes TestCase.
     */
    if (Spi_TestCase == Spi_LastTestCase)
    {
        return;
    }

    Spi_LastTestCase = Spi_TestCase;

    switch (Spi_TestCase)
    {
    case SPI_TEST_SYNC_ORANGE:
        (void)IoHwAb_Spi_SetLedStatus(IOHWAB_SPI_LED_ORANGE);
        break;
    case SPI_TEST_SYNC_GREEN:
        (void)IoHwAb_Spi_SetLedStatus(IOHWAB_SPI_LED_GREEN);
        break;
    case SPI_TEST_SYNC_RED:
        (void)IoHwAb_Spi_SetLedStatus(IOHWAB_SPI_LED_RED);
        break;
    case SPI_TEST_SYNC_BLUE:
        (void)IoHwAb_Spi_SetLedStatus(IOHWAB_SPI_LED_BLUE);
        break;
    case SPI_TEST_SYNC_OFF:
        (void)IoHwAb_Spi_SetLedStatus(IOHWAB_SPI_LED_OFF);
        break;
    case SPI_TEST_ASYNC_GREEN:
        App_SpiResult = IoHwAb_Spi_StartLedStatus(IOHWAB_SPI_LED_GREEN);
        break;
    case SPI_TEST_ASYNC_RED:
        App_SpiResult = IoHwAb_Spi_StartLedStatus(IOHWAB_SPI_LED_RED);
        break;
    default:
        break;
    }
}
uint8 count = 0;
extern IoHwAb_SpiStatusType s_SpiStatus;
int main()
{
    EcuM_Init();
    // IoHwAb_Spi_StartLedStatus(IOHWAB_SPI_LED_ORANGE);
    while (1)
    {
        IoHwAb_Spi_StartLedStatus(IOHWAB_SPI_LED_ORANGE);
        if (s_SpiStatus == IOHWAB_SPI_STATUS_COMPLETE)
        {
            IoHwAb_Port_FlipLedstattus();
            s_SpiStatus = IOHWAB_SPI_STATUS_IDLE;
            // break;
            Os_DelayMs(250);
        }
    }
}
