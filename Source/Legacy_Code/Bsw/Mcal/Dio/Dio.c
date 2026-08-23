#include "Dio.h"
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
static GPIO_TypeDef* Dio_GetPort(Dio_PortType GPIOx){
		switch(GPIOx){
			case DIO_PORT_A: return GPIOA;
			case DIO_PORT_B: return GPIOB;
			case DIO_PORT_C: return GPIOC;
			default:				 return GPIOD;
		}
}
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId){
		Dio_PortType port_id = ChannelId / 16;	//tim id cua port tuong ung voi ChannelID 
		GPIO_TypeDef* pPORTx = Dio_GetPort(port_id); 	//dia chi cua port
	  uint16 GPIO_Pin = DIO_GET_PIN_MASK(ChannelId);//bitmask cua channelID tren port
		//doc ve gia tri dien ap tren chan Dio cua port
		if(GPIO_ReadOutputDataBit(pPORTx,GPIO_Pin)){
			return STD_HIGH;
		}
		return STD_LOW;
}
void Dio_WriteChannel(Dio_ChannelType ChannelId,Dio_LevelType Level){
		Dio_PortType port_id = ChannelId / 16;					// tim id port tuong ung voi chan DIO
		uint16 GPIO_Pin = DIO_GET_PIN_MASK(ChannelId);	//tim chan GPIO -> tra ve bitmask tuong ung voi pin
		GPIO_TypeDef* pPORTx = Dio_GetPort(port_id);		//tra ve port 
		BitAction value = Level == STD_HIGH ? Bit_SET : Bit_RESET;
		GPIO_WriteBit(pPORTx,GPIO_Pin,value);
}
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId){
			GPIO_TypeDef* pPORTx = Dio_GetPort(PortId);
			Dio_PortLevelType value = (Dio_PortLevelType)GPIO_ReadOutputData(pPORTx);
			return value;
}
void Dio_WritePort(Dio_PortType PortId,Dio_PortLevelType Level){
		GPIO_TypeDef* pPORTx = Dio_GetPort(PortId);
		GPIO_Write(pPORTx,Level);
}
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr){
		GPIO_TypeDef* pPORTx = Dio_GetPort(ChannelGroupIdPtr->port);
		Dio_PortLevelType value = (Dio_PortLevelType)GPIO_ReadOutputData(pPORTx);
		return value;
}
void Dio_WriteChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr,Dio_PortLevelType Level){
	//lay dia chi port	
	GPIO_TypeDef* pPORTx = Dio_GetPort(ChannelGroupIdPtr->port);
	//doc gia tri hien tai cua port
	Dio_PortLevelType value = (Dio_PortLevelType)GPIO_ReadOutputData(pPORTx);
	//xoa cac bit tuong ung voi nhom chan
	value &= ~(ChannelGroupIdPtr->mask << ChannelGroupIdPtr->offset); // dich den vi tri ChannelGroup tuong ung va reset bit 
	//ghi gia tri moi
	value = (Level & ChannelGroupIdPtr->mask) << ChannelGroupIdPtr->offset;
	//cap nhat thanh ghi
	GPIO_Write(pPORTx,value);
}
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelID){
	Dio_PortType port_id = ChannelID / 16;	//lay id cua port tuong ung voi ChannelID
	GPIO_TypeDef* pPORTx = Dio_GetPort(port_id);	//tra ve port
	uint16 GPIO_Pin = DIO_GET_PIN_MASK(ChannelID);	//tim chan GPIO 
	Dio_LevelType Level = (Dio_LevelType)GPIO_ReadOutputDataBit(pPORTx,GPIO_Pin);
	BitAction value = Level == STD_HIGH ? Bit_RESET : Bit_SET;
	GPIO_WriteBit(pPORTx,GPIO_Pin,value);
}




