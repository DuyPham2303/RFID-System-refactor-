#include "Dio.h"
#include "Port.h"
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
void GPIO_Config(){
	Port_ConfigType portInit;
	portInit.port = PORT_PIN_A;
	portInit.pin = PORT_PIN_C13;
	portInit.ModeType = PORT_PIN_MODE_OUT_PP;
	portInit.SpeedType = PORT_PIN_VERY_HIGH;
	Port_Init(&portInit);
}
void delay(int delay_){
	int i;
	for(i = 0 ; i < delay_*2500000;i++){}
}
int count;

Dio_ChannelGroupType groupConfig;

int main(){
	GPIO_Config();
	
	groupConfig.mask = 0x1f;
	groupConfig.offset = 2;
	groupConfig.port = DIO_PORT_A;
	/*
	Dio_WriteChannel(DIO_CHANNEL_B5,STD_HIGH);
	Dio_WriteChannel(DIO_CHANNEL_B6,STD_HIGH);
	Dio_WriteChannel(DIO_CHANNEL_B7,STD_HIGH);
	Dio_WriteChannel(DIO_CHANNEL_B8,STD_HIGH);
	
	
	Dio_WriteChannel(DIO_PIN_B1,STD_HIGH);
	Dio_WriteChannel(DIO_PIN_B4,STD_HIGH);
	Dio_WriteChannel(DIO_PIN_B7,STD_HIGH);
	Dio_WriteChannel(DIO_PIN_B10,STD_HIGH);
	

	Dio_WritePort(DIO_PORT_A,0xf0ff);
	Dio_WritePort(DIO_PORT_B,0xff0f);
	Dio_WritePort(DIO_PORT_C,0xfff0);
	Dio_WritePort(DIO_PORT_D,0xffff);
	*/


	Dio_WriteChannelGroup(&groupConfig,0x1f);
	return 0;
	/*
	while(1){
			Dio_FlipChannel(DIO_CHANNEL_A10);
			delay(1);
			count++;
	}
	*/
	
}
