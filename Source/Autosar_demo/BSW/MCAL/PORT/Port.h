#ifndef PORT_H
#define PORT_H
#include "Std_Types.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"  

typedef uint8 PortType;

#define PORT_PIN_A	(PortType)(0x00)
#define PORT_PIN_B	(PortType)(0x01)
#define PORT_PIN_C	(PortType)(0x02)
#define PORT_PIN_D	(PortType)(0x03)

/********************************************************
* @typedef PORT_PinType
* @brief kieu du lieu danh so thu tu cho cac port pin
* @detail Khai bao kieu du lieu phai du lon de danh so duoc tat ca cac port pin tren MCU
*****************/
typedef uint16	Port_PinType;

//macro lay ra bitmask cua pin tuong ung
#define GET_PORT_PIN(PinNumber)	(Port_PinType)(1 << (PinNumber % 16))

#define PORT_PIN_A0		GET_PORT_PIN(0)
#define PORT_PIN_A1		GET_PORT_PIN(1)
#define PORT_PIN_A2		GET_PORT_PIN(2)
#define PORT_PIN_A3		GET_PORT_PIN(3)
#define PORT_PIN_A4		GET_PORT_PIN(4)
#define PORT_PIN_A5		GET_PORT_PIN(5)
#define PORT_PIN_A6		GET_PORT_PIN(6)
#define PORT_PIN_A7		GET_PORT_PIN(7)
#define PORT_PIN_A8		GET_PORT_PIN(8)
#define PORT_PIN_A9		GET_PORT_PIN(9)
#define PORT_PIN_A10	GET_PORT_PIN(10)
#define PORT_PIN_A11	GET_PORT_PIN(11)
#define PORT_PIN_A12	GET_PORT_PIN(12)
#define PORT_PIN_A13	GET_PORT_PIN(13)
#define PORT_PIN_A14	GET_PORT_PIN(14)
#define PORT_PIN_A15	GET_PORT_PIN(15)

#define PORT_PIN_B0		GET_PORT_PIN(16)
#define PORT_PIN_B1		GET_PORT_PIN(17)
#define PORT_PIN_B2		GET_PORT_PIN(18)
#define PORT_PIN_B3		GET_PORT_PIN(19)
#define PORT_PIN_B4		GET_PORT_PIN(20)
#define PORT_PIN_B5		GET_PORT_PIN(21)
#define PORT_PIN_B6		GET_PORT_PIN(22)
#define PORT_PIN_B7		GET_PORT_PIN(23)
#define PORT_PIN_B8		GET_PORT_PIN(24)
#define PORT_PIN_B9		GET_PORT_PIN(25)
#define PORT_PIN_B10	GET_PORT_PIN(26)
#define PORT_PIN_B11	GET_PORT_PIN(27)
#define PORT_PIN_B12	GET_PORT_PIN(28)
#define PORT_PIN_B13	GET_PORT_PIN(29)
#define PORT_PIN_B14	GET_PORT_PIN(30)
#define PORT_PIN_B15	GET_PORT_PIN(31)

#define PORT_PIN_C0		GET_PORT_PIN(32)
#define PORT_PIN_C1		GET_PORT_PIN(33)
#define PORT_PIN_C2		GET_PORT_PIN(34)
#define PORT_PIN_C3		GET_PORT_PIN(35)
#define PORT_PIN_C4		GET_PORT_PIN(36)
#define PORT_PIN_C5		GET_PORT_PIN(37)
#define PORT_PIN_C6		GET_PORT_PIN(38)
#define PORT_PIN_C7		GET_PORT_PIN(39)
#define PORT_PIN_C8		GET_PORT_PIN(40)
#define PORT_PIN_C9		GET_PORT_PIN(41)
#define PORT_PIN_C10	GET_PORT_PIN(42)
#define PORT_PIN_C11	GET_PORT_PIN(43)
#define PORT_PIN_C12	GET_PORT_PIN(44)
#define PORT_PIN_C13	GET_PORT_PIN(45)
#define PORT_PIN_C14	GET_PORT_PIN(46)
#define PORT_PIN_C15	GET_PORT_PIN(47)

#define PORT_PIN_D0		GET_PORT_PIN(48)
#define PORT_PIN_D1		GET_PORT_PIN(49)
#define PORT_PIN_D2		GET_PORT_PIN(50)
#define PORT_PIN_D3		GET_PORT_PIN(51)
#define PORT_PIN_D4		GET_PORT_PIN(52)
#define PORT_PIN_D5		GET_PORT_PIN(53)
#define PORT_PIN_D6		GET_PORT_PIN(54)
#define PORT_PIN_D7		GET_PORT_PIN(55)
#define PORT_PIN_D8		GET_PORT_PIN(56)
#define PORT_PIN_D9		GET_PORT_PIN(57)
#define PORT_PIN_D10	GET_PORT_PIN(58)
#define PORT_PIN_D11	GET_PORT_PIN(59)
#define PORT_PIN_D12	GET_PORT_PIN(60)
#define PORT_PIN_D13	GET_PORT_PIN(61)
#define PORT_PIN_D14	GET_PORT_PIN(62)
#define PORT_PIN_D15	GET_PORT_PIN(63)


/********************************************************
* @typedef Port_PinDirectionType
* @brief kieu du lieu enum xac dinh mode cua in/out chan port pin
* @detail mode in/out duoc quy dinh la 0 hoac 1
*****************/
typedef enum{
	PORT_PIN_IN,
	PORT_PIN_OUT
}Port_PinDirectionType;


typedef uint8 Port_PinModeType;

#define PORT_PIN_MODE_AIN					(Port_PinModeType)(0x01)
#define PORT_PIN_MODE_IN_FLOATING	(Port_PinModeType)(0x02)
#define PORT_PIN_MODE_IPD					(Port_PinModeType)(0x03)
#define PORT_PIN_MODE_IPU					(Port_PinModeType)(0x04)
#define PORT_PIN_MODE_OUT_OD			(Port_PinModeType)(0x05)
#define PORT_PIN_MODE_OUT_PP			(Port_PinModeType)(0x06)
#define PORT_PIN_MODE_AF_OD				(Port_PinModeType)(0x07)
#define PORT_PIN_MODE_AF_PP				(Port_PinModeType)(0x08)

typedef uint8 Port_PinSpeedType;
#define PORT_PIN_SPEED_NORMAL		 (Port_PinSpeedType)(0x01)
#define PORT_PIN_SPEED_HIGH			 (Port_PinSpeedType)(0x02)
#define PORT_PIN_VERY_HIGH			 (Port_PinSpeedType)(0x03)
/********************************************************
* @typedef Port_ConfigType
* @brief kieu cau truc de cau hinh Port cho chan GPIO
* @detail cau hinh pinMode,pull up/down, speed, OutputType,AltMode
*****************/
typedef struct Port_ConfigType{
		PortType port;
		Port_PinType pin;
		Port_PinModeType ModeType;
		Port_PinSpeedType SpeedType;
}Port_ConfigType;


/********************************************************
* @brief cau hinh cac thong so cho port
* @param ConfigPtr: con tro den cau truc de cau hinh port
*****************/
void Port_Init (const Port_ConfigType* ConfigPtr);

/********************************************************
* @brief cau hinh che do input/output cua pin
* @param Pin: Macro ID cua pin can cau hinh 
* @param Direction: truyen vao PORT_PIN_IN / PORT_PIN_IN
*****************/
void Port_SetPinDirection(
	Port_PinType Pin,
  Port_PinDirectionType Direction);

/********************************************************
* @brief reset che do input/output cua pin
*****************/

void Port_RefreshPortDirection();

/********************************************************
* @brief cau hinh mode cho pin
* @param Pin: Macro ID cua pin can cau hinh 
* @param Mode: Macro cua mode can cau hinh
*****************/
void Port_SetPinMode (
	Port_PinType Pin,
	Port_PinModeType Mode
);
#endif