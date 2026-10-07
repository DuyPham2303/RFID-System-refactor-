#ifndef PORT_PINMODE_CFG_H
#define PORT_PINMODE_CFG_H

#include "Std_Types.h"

/*
 * Port_PinModeType is implementation-specific in AUTOSAR.
 * These are project-specific symbolic modes for STM32F103.
 */
typedef uint8 Port_PinModeType;

#define PORT_PIN_MODE_DIO             ((Port_PinModeType)0u)
#define PORT_PIN_MODE_TIM1_CH1        ((Port_PinModeType)1u)
#define PORT_PIN_MODE_TIM1_CH2        ((Port_PinModeType)2u)
#define PORT_PIN_MODE_TIM1_CH3        ((Port_PinModeType)3u)
#define PORT_PIN_MODE_TIM1_CH4        ((Port_PinModeType)4u)
#define PORT_PIN_MODE_TIM3_CH1        ((Port_PinModeType)5u)
#define PORT_PIN_MODE_TIM3_CH2        ((Port_PinModeType)6u)
#define PORT_PIN_MODE_TIM3_CH3        ((Port_PinModeType)7u)
#define PORT_PIN_MODE_TIM3_CH4        ((Port_PinModeType)8u)

#endif /* PORT_PINMODE_CFG_H */
