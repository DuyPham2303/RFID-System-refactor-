#include "IoHwAb_Port.h"
#include "IoHwAb_Port_Cfg.h"
#include "Dio.h"
#include "Port.h"

Std_ReturnType IoHwAb_PortInit(void)
{
    if (Port_Init(&g_Port_ConfigGroup) != E_OK)
    {
        return E_NOT_OK;
    }

    Dio_WriteChannel(IOHWAB_PORT_SPI_NSS_CHANNEL,
                     IOHWAB_PORT_SPI_NSS_INITIAL_LEVEL);

    return E_OK;
}

void IoHwAb_Port_FlipLedstattus()
{
    Dio_FlipChannel(IOHWAB_PORT_LED_CHANNEL_C13);
}