#ifndef PORT_PINID_CFG_H
#define PORT_PINID_CFG_H

#include "Std_Types.h"

/*
 * Project-specific symbolic Port Pin IDs.
 *
 * AUTOSAR:
 *   Port_PinType is the symbolic name/type used to identify a Port Pin.
 *
 * STM32F103C8T6 / LQFP48:
 *   PA0..PA15, PB0..PB15, PC13..PC15, PD0..PD1
 *
 * IMPORTANT:
 *   These IDs identify PHYSICAL MCU PINS.
 *   They do NOT identify Timer/SPI/UART channels.
 */

typedef uint16 Port_PinType;

#define PORT_PIN_PA0      ((Port_PinType)0x0000u)
#define PORT_PIN_PA1      ((Port_PinType)0x0001u)
#define PORT_PIN_PA2      ((Port_PinType)0x0002u)
#define PORT_PIN_PA3      ((Port_PinType)0x0003u)
#define PORT_PIN_PA4      ((Port_PinType)0x0004u)
#define PORT_PIN_PA5      ((Port_PinType)0x0005u)
#define PORT_PIN_PA6      ((Port_PinType)0x0006u)
#define PORT_PIN_PA7      ((Port_PinType)0x0007u)
#define PORT_PIN_PA8      ((Port_PinType)0x0008u)
#define PORT_PIN_PA9      ((Port_PinType)0x0009u)
#define PORT_PIN_PA10     ((Port_PinType)0x000Au)
#define PORT_PIN_PA11     ((Port_PinType)0x000Bu)
#define PORT_PIN_PA12     ((Port_PinType)0x000Cu)
#define PORT_PIN_PA13     ((Port_PinType)0x000Du)
#define PORT_PIN_PA14     ((Port_PinType)0x000Eu)
#define PORT_PIN_PA15     ((Port_PinType)0x000Fu)

#define PORT_PIN_PB0      ((Port_PinType)0x0100u)
#define PORT_PIN_PB1      ((Port_PinType)0x0101u)
#define PORT_PIN_PB2      ((Port_PinType)0x0102u)
#define PORT_PIN_PB3      ((Port_PinType)0x0103u)
#define PORT_PIN_PB4      ((Port_PinType)0x0104u)
#define PORT_PIN_PB5      ((Port_PinType)0x0105u)
#define PORT_PIN_PB6      ((Port_PinType)0x0106u)
#define PORT_PIN_PB7      ((Port_PinType)0x0107u)
#define PORT_PIN_PB8      ((Port_PinType)0x0108u)
#define PORT_PIN_PB9      ((Port_PinType)0x0109u)
#define PORT_PIN_PB10     ((Port_PinType)0x010Au)
#define PORT_PIN_PB11     ((Port_PinType)0x010Bu)
#define PORT_PIN_PB12     ((Port_PinType)0x010Cu)
#define PORT_PIN_PB13     ((Port_PinType)0x010Du)
#define PORT_PIN_PB14     ((Port_PinType)0x010Eu)
#define PORT_PIN_PB15     ((Port_PinType)0x010Fu)

#define PORT_PIN_PC13     ((Port_PinType)0x020Du)
#define PORT_PIN_PC14     ((Port_PinType)0x020Eu)
#define PORT_PIN_PC15     ((Port_PinType)0x020Fu)

#define PORT_PIN_PD0      ((Port_PinType)0x0300u)
#define PORT_PIN_PD1      ((Port_PinType)0x0301u)

#endif /* PORT_PINID_CFG_H */
