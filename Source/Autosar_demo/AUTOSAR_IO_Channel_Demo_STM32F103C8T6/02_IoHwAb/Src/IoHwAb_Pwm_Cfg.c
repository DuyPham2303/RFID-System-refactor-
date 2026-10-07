#include "IoHwAb_Pwm_Cfg.h"

/*
 * This is the abstraction boundary:
 *
 * IOHWAB_CHANNEL_MOTOR_PWM
 *          |
 *          v
 * PWM_CHANNEL_TIM1_CH1
 *
 * No physical GPIO pin appears here.
 */
const IoHwAb_PwmChannelConfigType IoHwAb_PwmChannelConfig[] =
{
    {
        .LogicalChannel = IOHWAB_CHANNEL_MOTOR_PWM,
        .PwmChannel     = PWM_CHANNEL_TIM1_CH1
    },

    {
        .LogicalChannel = IOHWAB_CHANNEL_LED_PWM,
        .PwmChannel     = PWM_CHANNEL_TIM3_CH1
    }
};
