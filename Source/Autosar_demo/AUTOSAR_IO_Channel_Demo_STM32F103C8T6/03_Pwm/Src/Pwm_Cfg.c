#include "Pwm_Cfg.h"

/*
 * PWM channel -> physical output pin.
 *
 * This is MCU/project configuration. The PWM driver does not need
 * to hard-code "PA8" in its algorithm.
 *
 * TIM1_CH1 default route on STM32F103C8T6:
 *     TIM1_CH1 -> PA8
 */
const Pwm_ChannelConfigType Pwm_ChannelConfig[] =
{
    {
        .Channel    = PWM_CHANNEL_TIM1_CH1,
        .OutputPin  = PORT_PIN_PA8,
        .Remap      = 0u
    },

    {
        .Channel    = PWM_CHANNEL_TIM3_CH1,
        .OutputPin  = PORT_PIN_PA6,
        .Remap      = 0u
    }
};
