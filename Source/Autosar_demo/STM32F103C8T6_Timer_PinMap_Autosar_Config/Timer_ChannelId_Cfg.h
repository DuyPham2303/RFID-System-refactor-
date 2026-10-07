#ifndef TIMER_CHANNELID_CFG_H
#define TIMER_CHANNELID_CFG_H

#include "Std_Types.h"

/*
 * Project-specific Timer Channel symbolic IDs.
 *
 * These identify a PERIPHERAL CHANNEL, not a physical GPIO pin.
 * The same timer channel may have more than one physical pin
 * depending on AFIO remapping.
 */

typedef uint8 Timer_ChannelType;

#define TIMER_CHANNEL_TIM1_CH1       ((Timer_ChannelType)0u)
#define TIMER_CHANNEL_TIM1_CH2       ((Timer_ChannelType)1u)
#define TIMER_CHANNEL_TIM1_CH3       ((Timer_ChannelType)2u)
#define TIMER_CHANNEL_TIM1_CH4       ((Timer_ChannelType)3u)

#define TIMER_CHANNEL_TIM1_CH1N      ((Timer_ChannelType)4u)
#define TIMER_CHANNEL_TIM1_CH2N      ((Timer_ChannelType)5u)
#define TIMER_CHANNEL_TIM1_CH3N      ((Timer_ChannelType)6u)

#define TIMER_CHANNEL_TIM2_CH1       ((Timer_ChannelType)7u)
#define TIMER_CHANNEL_TIM2_CH2       ((Timer_ChannelType)8u)
#define TIMER_CHANNEL_TIM2_CH3       ((Timer_ChannelType)9u)
#define TIMER_CHANNEL_TIM2_CH4       ((Timer_ChannelType)10u)

#define TIMER_CHANNEL_TIM3_CH1       ((Timer_ChannelType)11u)
#define TIMER_CHANNEL_TIM3_CH2       ((Timer_ChannelType)12u)
#define TIMER_CHANNEL_TIM3_CH3       ((Timer_ChannelType)13u)
#define TIMER_CHANNEL_TIM3_CH4       ((Timer_ChannelType)14u)

#define TIMER_CHANNEL_TIM4_CH1       ((Timer_ChannelType)15u)
#define TIMER_CHANNEL_TIM4_CH2       ((Timer_ChannelType)16u)
#define TIMER_CHANNEL_TIM4_CH3       ((Timer_ChannelType)17u)
#define TIMER_CHANNEL_TIM4_CH4       ((Timer_ChannelType)18u)

#define TIMER_CHANNEL_INVALID        ((Timer_ChannelType)0xFFu)

#endif /* TIMER_CHANNELID_CFG_H */
