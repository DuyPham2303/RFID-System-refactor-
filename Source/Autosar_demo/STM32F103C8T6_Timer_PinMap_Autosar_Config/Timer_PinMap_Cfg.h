#ifndef TIMER_PINMAP_CFG_H
#define TIMER_PINMAP_CFG_H

#include "Port_PinId_Cfg.h"
#include "Timer_ChannelId_Cfg.h"

/*
 * MCU-specific Timer <-> GPIO mapping.
 *
 * This file is CONFIGURATION, not Timer driver implementation.
 *
 * The Timer MCAL should consume TIMER_CHANNEL_xxx.
 * The Port MCAL should consume PORT_PIN_xxx.
 *
 * The map below documents the selected physical route.
 */

typedef enum
{
    TIMER_REMAP_NONE = 0u,

    TIMER_REMAP_PARTIAL_1 = 1u,
    TIMER_REMAP_PARTIAL_2 = 2u,

    TIMER_REMAP_FULL = 3u
} Timer_RemapType;

typedef struct
{
    Timer_ChannelType Channel;
    Port_PinType      Pin;
    Timer_RemapType   Remap;
} Timer_PinMapType;


/* ============================================================
 * TIM1
 * ============================================================
 *
 * TIM1 no-remap:
 *   CH1  -> PA8
 *   CH2  -> PA9
 *   CH3  -> PA10
 *   CH4  -> PA11
 *
 * TIM1 partial remap:
 *   CH1  -> PA8
 *   CH2  -> PA9
 *   CH3  -> PA10
 *   CH4  -> PA11
 *
 * Complementary outputs:
 *   no-remap:
 *       CH1N -> PB13
 *       CH2N -> PB14
 *       CH3N -> PB15
 *
 *   partial remap:
 *       CH1N -> PA7
 *       CH2N -> PB0
 *       CH3N -> PB1
 *
 * Full remap is not available on the 48-pin package.
 */

#define TIMER_PIN_TIM1_CH1_DEFAULT    PORT_PIN_PA8
#define TIMER_PIN_TIM1_CH2_DEFAULT    PORT_PIN_PA9
#define TIMER_PIN_TIM1_CH3_DEFAULT    PORT_PIN_PA10
#define TIMER_PIN_TIM1_CH4_DEFAULT    PORT_PIN_PA11

#define TIMER_PIN_TIM1_CH1N_DEFAULT   PORT_PIN_PB13
#define TIMER_PIN_TIM1_CH2N_DEFAULT   PORT_PIN_PB14
#define TIMER_PIN_TIM1_CH3N_DEFAULT   PORT_PIN_PB15

#define TIMER_PIN_TIM1_CH1N_PARTIAL   PORT_PIN_PA7
#define TIMER_PIN_TIM1_CH2N_PARTIAL   PORT_PIN_PB0
#define TIMER_PIN_TIM1_CH3N_PARTIAL   PORT_PIN_PB1


/* ============================================================
 * TIM2
 * ============================================================
 *
 * No remap:
 *   CH1 -> PA0
 *   CH2 -> PA1
 *   CH3 -> PA2
 *   CH4 -> PA3
 *
 * Partial remap 1:
 *   CH1 -> PA15
 *   CH2 -> PB3
 *   CH3 -> PA2
 *   CH4 -> PA3
 *
 * Partial remap 2:
 *   CH1 -> PA0
 *   CH2 -> PA1
 *   CH3 -> PB10
 *   CH4 -> PB11
 *
 * Full remap:
 *   CH1 -> PA15
 *   CH2 -> PB3
 *   CH3 -> PB10
 *   CH4 -> PB11
 */

#define TIMER_PIN_TIM2_CH1_DEFAULT    PORT_PIN_PA0
#define TIMER_PIN_TIM2_CH2_DEFAULT    PORT_PIN_PA1
#define TIMER_PIN_TIM2_CH3_DEFAULT    PORT_PIN_PA2
#define TIMER_PIN_TIM2_CH4_DEFAULT    PORT_PIN_PA3

#define TIMER_PIN_TIM2_CH1_PR1        PORT_PIN_PA15
#define TIMER_PIN_TIM2_CH2_PR1        PORT_PIN_PB3
#define TIMER_PIN_TIM2_CH3_PR1        PORT_PIN_PA2
#define TIMER_PIN_TIM2_CH4_PR1        PORT_PIN_PA3

#define TIMER_PIN_TIM2_CH1_PR2        PORT_PIN_PA0
#define TIMER_PIN_TIM2_CH2_PR2        PORT_PIN_PA1
#define TIMER_PIN_TIM2_CH3_PR2        PORT_PIN_PB10
#define TIMER_PIN_TIM2_CH4_PR2        PORT_PIN_PB11

#define TIMER_PIN_TIM2_CH1_FULL       PORT_PIN_PA15
#define TIMER_PIN_TIM2_CH2_FULL       PORT_PIN_PB3
#define TIMER_PIN_TIM2_CH3_FULL       PORT_PIN_PB10
#define TIMER_PIN_TIM2_CH4_FULL       PORT_PIN_PB11


/* ============================================================
 * TIM3
 * ============================================================
 *
 * No remap:
 *   CH1 -> PA6
 *   CH2 -> PA7
 *   CH3 -> PB0
 *   CH4 -> PB1
 *
 * Partial remap:
 *   CH1 -> PB4
 *   CH2 -> PB5
 *   CH3 -> PB0
 *   CH4 -> PB1
 *
 * Full remap is not available on the 48-pin package.
 */

#define TIMER_PIN_TIM3_CH1_DEFAULT    PORT_PIN_PA6
#define TIMER_PIN_TIM3_CH2_DEFAULT    PORT_PIN_PA7
#define TIMER_PIN_TIM3_CH3_DEFAULT    PORT_PIN_PB0
#define TIMER_PIN_TIM3_CH4_DEFAULT    PORT_PIN_PB1

#define TIMER_PIN_TIM3_CH1_PARTIAL    PORT_PIN_PB4
#define TIMER_PIN_TIM3_CH2_PARTIAL    PORT_PIN_PB5
#define TIMER_PIN_TIM3_CH3_PARTIAL    PORT_PIN_PB0
#define TIMER_PIN_TIM3_CH4_PARTIAL    PORT_PIN_PB1


/* ============================================================
 * TIM4
 * ============================================================
 *
 * No remap on STM32F103C8T6 LQFP48:
 *   CH1 -> PB6
 *   CH2 -> PB7
 *   CH3 -> PB8
 *   CH4 -> PB9
 *
 * PD12..PD15 remap belongs to larger packages and is therefore
 * intentionally not exposed here.
 */

#define TIMER_PIN_TIM4_CH1_DEFAULT    PORT_PIN_PB6
#define TIMER_PIN_TIM4_CH2_DEFAULT    PORT_PIN_PB7
#define TIMER_PIN_TIM4_CH3_DEFAULT    PORT_PIN_PB8
#define TIMER_PIN_TIM4_CH4_DEFAULT    PORT_PIN_PB9

#endif /* TIMER_PINMAP_CFG_H */
