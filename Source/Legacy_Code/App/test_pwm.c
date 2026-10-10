
#include "Dio/inc/Dio.h"
#include "Port/inc/Port.h"
#include "Gpt/inc/Gpt.h"
#include "Pwm/inc/Pwm.h"
#include "./Bsw/Services/Os/Os.h"
#include "System/Board_Clock.h"
#include "./Bsw/IoHwAb/IoHwAb_Pwm/IoHwAb_Pwm.h"

/*IoHwAb
 */
static void IoHwAb_Led_SetToggle(void)
{
    Dio_FlipChannel(DIO_CHANNEL_C13);
}
/* Rte */
static void Rte_Call_Led_Flip(void)
{
    IoHwAb_Led_SetToggle();
}
/* Application */
static void BlinkLed_Runnable(void)
{
    Rte_Call_Led_Flip();
}

volatile uint8 g_count = 0;
void main()
{
    Board_PeripheralsClock_Init();
    Port_Init(&g_Port_ConfigGroup);
    Gpt_Init(&g_Gpt_ConfigGroup);
    Pwm_Init(&g_Pwm_Config);
    IoHwAb_PwmInit(&g_IoHwAb_Pwm_Config);
    while (1)
    {
        IoHwAb_Pwm_SetDuty(IOHWAB_LED_GREEN_PWM, 25);
        IoHwAb_Pwm_SetDuty(IOHWAB_LED_BLUE_PWM, 50);
        IoHwAb_Pwm_SetDuty(IOHWAB_LED_YELLOW_PWM, 75);
        IoHwAb_Pwm_SetDuty(IOHWAB_LED_RED_PWM, 100);
        // if (g_Flagupdate_Periodic == TRUE)
        // {
        //     BlinkLed_Runnable();
        //     /*reset cờ cho lần ngắt kế tiếp*/
        //     g_Flagupdate_Periodic = FALSE;
        // }
        // Dio_FlipChannel(DIO_CHANNEL_C13);
        g_count++;
        Os_DelayMs(500);
    }
}
