#include "Port.h"
static GPIO_TypeDef* Get_Port(PortType GPIOx){
	  switch(GPIOx){
			case PORT_PIN_A: return GPIOA;
			case PORT_PIN_B: return GPIOB;
			case PORT_PIN_C: return GPIOC;
		default:				 return GPIOD;
		}
}
static GPIOMode_TypeDef Get_PortPinMode(Port_PinModeType mode){
	switch(mode){
			case PORT_PIN_MODE_AF_OD: return GPIO_Mode_AF_OD;
			case PORT_PIN_MODE_AF_PP: return GPIO_Mode_AF_PP;
			case PORT_PIN_MODE_AIN: return GPIO_Mode_AIN;
			case PORT_PIN_MODE_IN_FLOATING: return GPIO_Mode_IN_FLOATING;
			case PORT_PIN_MODE_IPD: return GPIO_Mode_IPD;
			case PORT_PIN_MODE_IPU: return GPIO_Mode_IPU;
			case PORT_PIN_MODE_OUT_OD: return GPIO_Mode_Out_OD;
		default:
				return GPIO_Mode_Out_PP;;
		}
}
static GPIOSpeed_TypeDef Get_PortPinSpeed(Port_PinSpeedType speed){
	switch(speed){
			case PORT_PIN_SPEED_NORMAL: return GPIO_Speed_10MHz;
			case PORT_PIN_SPEED_HIGH: return GPIO_Speed_2MHz;
		default:
			return GPIO_Speed_50MHz;
	}
}
static void Clock_config(GPIO_TypeDef* pGPIOx){
	if(pGPIOx == GPIOA)	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	else if(pGPIOx == GPIOB)	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	else if(pGPIOx == GPIOC)	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	else if(pGPIOx == GPIOD)	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
}
void Port_Init (const Port_ConfigType* ConfigPtr){
	//cau hinh port GPIO
	GPIO_TypeDef* pGPIOx = Get_Port(ConfigPtr->port);
	//cau hinh chan,mode,va speed
	GPIO_InitTypeDef GPIO_config;
	GPIO_config.GPIO_Pin = ConfigPtr->pin;
	GPIO_config.GPIO_Mode = Get_PortPinMode(ConfigPtr->ModeType);
	GPIO_config.GPIO_Speed = Get_PortPinSpeed(ConfigPtr->SpeedType);
	//cap clock
	Clock_config(pGPIOx);
	GPIO_Init(pGPIOx,&GPIO_config);
	
}
