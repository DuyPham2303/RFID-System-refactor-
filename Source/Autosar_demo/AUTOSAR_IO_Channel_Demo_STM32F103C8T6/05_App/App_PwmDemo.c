#include "IoHwAb_Pwm.h"

/*
 * Application sees only the logical IoHwAb service.
 * It does NOT pass Port_ConfigType to Port_Init().
 */
void App_MotorPwmDemo(void)
{
    (void)IoHwAb_SetMotorPwm(500u);
}
