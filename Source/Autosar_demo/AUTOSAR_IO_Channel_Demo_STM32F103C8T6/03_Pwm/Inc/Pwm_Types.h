#ifndef PWM_TYPES_H
#define PWM_TYPES_H

#include "Std_Types.h"

/*
 * Project-specific symbolic PWM channel type.
 * It represents a TIMER/PWM peripheral channel, not a GPIO pin.
 */
typedef uint8 Pwm_ChannelType;

#define PWM_CHANNEL_TIM1_CH1   ((Pwm_ChannelType)0u)
#define PWM_CHANNEL_TIM1_CH2   ((Pwm_ChannelType)1u)
#define PWM_CHANNEL_TIM1_CH3   ((Pwm_ChannelType)2u)
#define PWM_CHANNEL_TIM1_CH4   ((Pwm_ChannelType)3u)

#define PWM_CHANNEL_TIM3_CH1   ((Pwm_ChannelType)4u)
#define PWM_CHANNEL_TIM3_CH2   ((Pwm_ChannelType)5u)
#define PWM_CHANNEL_TIM3_CH3   ((Pwm_ChannelType)6u)
#define PWM_CHANNEL_TIM3_CH4   ((Pwm_ChannelType)7u)

#define PWM_CHANNEL_INVALID    ((Pwm_ChannelType)0xFFu)

#endif /* PWM_TYPES_H */
