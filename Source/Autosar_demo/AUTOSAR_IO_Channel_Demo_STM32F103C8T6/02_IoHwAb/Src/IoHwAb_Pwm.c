#include "IoHwAb_Pwm.h"
#include "Pwm.h"

void IoHwAb_PwmInit(void)
{
    /*
     * In a real AUTOSAR startup sequence, PWM itself is initialized
     * by the ECU/BSW startup path. This function is intentionally kept
     * empty in this demo.
     */
}

Std_ReturnType IoHwAb_SetMotorPwm(uint16 DutyCycle)
{
    if (DutyCycle > 1000u)
    {
        return E_NOT_OK;
    }

    Pwm_SetDutyCycle(
        IoHwAb_PwmChannelConfig[IOHWAB_CHANNEL_MOTOR_PWM].PwmChannel,
        DutyCycle);

    return E_OK;
}
