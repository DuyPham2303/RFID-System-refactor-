/********************************************************
 *******************
 * @file Std_Types.h
 * @brief Khai bao cac kieu du lieu va macro su dung cho viec doc/ghi DIO
 * @details Khai bao cac kieu tra ve bao gom channel,port,level
 * @version 1.0
 * @date 2025-19-1
 * @author HALA Academy
 * @website https://hala.edu.vn/
 *********************************************************
 ******************/
#ifndef DIO_H
#define DIO_H
#include "Std_Types.h"

/********************************************************
* @typedef Dio_ChannelType
* @brief kieu du lieu danh so thu tu cho cac chan DIO channel
* @detail Khai bao kieu du lieu phai du lon de danh so duoc tat ca cac chan DIO tren MCU
*****************/
typedef uint8 Dio_ChannelType; 

//macro dinh nghia cac channelID
#define DIO_GET_CHANNEL(GPIOx,pin) (Dio_ChannelType)((GPIOx << 4) + (pin)) //tra ve bitmask tuong ung voi pin va port
	
//macro xac dinh bitmask tuong ung voi channelID tren port
#define DIO_GET_PIN_MASK(channelID) (1 << ((channelID) % 16)) //tra ve gia tri bitmask tren thanh ghi dang lam viec

#define DIO_CHANNEL_A0	DIO_GET_CHANNEL(DIO_PORT_A,0)
#define DIO_CHANNEL_A1	DIO_GET_CHANNEL(DIO_PORT_A,1)
#define DIO_CHANNEL_A2	DIO_GET_CHANNEL(DIO_PORT_A,2)
#define DIO_CHANNEL_A3	DIO_GET_CHANNEL(DIO_PORT_A,3)
#define DIO_CHANNEL_A4	DIO_GET_CHANNEL(DIO_PORT_A,4)
#define DIO_CHANNEL_A5	DIO_GET_CHANNEL(DIO_PORT_A,5)
#define DIO_CHANNEL_A6	DIO_GET_CHANNEL(DIO_PORT_A,6)
#define DIO_CHANNEL_A7	DIO_GET_CHANNEL(DIO_PORT_A,7)
#define DIO_CHANNEL_A8	DIO_GET_CHANNEL(DIO_PORT_A,8)
#define DIO_CHANNEL_A9	DIO_GET_CHANNEL(DIO_PORT_A,9)
#define DIO_CHANNEL_A10	DIO_GET_CHANNEL(DIO_PORT_A,10)
#define DIO_CHANNEL_A11	DIO_GET_CHANNEL(DIO_PORT_A,11)
#define DIO_CHANNEL_A12	DIO_GET_CHANNEL(DIO_PORT_A,12)
#define DIO_CHANNEL_A13	DIO_GET_CHANNEL(DIO_PORT_A,13)
#define DIO_CHANNEL_A14	DIO_GET_CHANNEL(DIO_PORT_A,14)
#define DIO_CHANNEL_A15	DIO_GET_CHANNEL(DIO_PORT_A,15)

#define DIO_CHANNEL_B0	DIO_GET_CHANNEL(DIO_PORT_B,0)
#define DIO_CHANNEL_B1	DIO_GET_CHANNEL(DIO_PORT_B,1)
#define DIO_CHANNEL_B2	DIO_GET_CHANNEL(DIO_PORT_B,2)
#define DIO_CHANNEL_B3	DIO_GET_CHANNEL(DIO_PORT_B,3)
#define DIO_CHANNEL_B4	DIO_GET_CHANNEL(DIO_PORT_B,4)
#define DIO_CHANNEL_B5	DIO_GET_CHANNEL(DIO_PORT_B,5)
#define DIO_CHANNEL_B6	DIO_GET_CHANNEL(DIO_PORT_B,6)
#define DIO_CHANNEL_B7	DIO_GET_CHANNEL(DIO_PORT_B,7)
#define DIO_CHANNEL_B8	DIO_GET_CHANNEL(DIO_PORT_B,8)
#define DIO_CHANNEL_B9	DIO_GET_CHANNEL(DIO_PORT_B,9)
#define DIO_CHANNEL_B10	DIO_GET_CHANNEL(DIO_PORT_B,10)
#define DIO_CHANNEL_B11	DIO_GET_CHANNEL(DIO_PORT_B,11)
#define DIO_CHANNEL_B12	DIO_GET_CHANNEL(DIO_PORT_B,12)
#define DIO_CHANNEL_B13	DIO_GET_CHANNEL(DIO_PORT_B,13)
#define DIO_CHANNEL_B14	DIO_GET_CHANNEL(DIO_PORT_B,14)
#define DIO_CHANNEL_B15	DIO_GET_CHANNEL(DIO_PORT_B,15)

#define DIO_CHANNEL_C0	DIO_GET_CHANNEL(DIO_PORT_C,0)
#define DIO_CHANNEL_C1	DIO_GET_CHANNEL(DIO_PORT_C,1)
#define DIO_CHANNEL_C2	DIO_GET_CHANNEL(DIO_PORT_C,2)
#define DIO_CHANNEL_C3	DIO_GET_CHANNEL(DIO_PORT_C,3)
#define DIO_CHANNEL_C4	DIO_GET_CHANNEL(DIO_PORT_C,4)
#define DIO_CHANNEL_C5	DIO_GET_CHANNEL(DIO_PORT_C,5)
#define DIO_CHANNEL_C6	DIO_GET_CHANNEL(DIO_PORT_C,6)
#define DIO_CHANNEL_C7	DIO_GET_CHANNEL(DIO_PORT_C,7)
#define DIO_CHANNEL_C8	DIO_GET_CHANNEL(DIO_PORT_C,8)
#define DIO_CHANNEL_C9	DIO_GET_CHANNEL(DIO_PORT_C,9)
#define DIO_CHANNEL_C10	DIO_GET_CHANNEL(DIO_PORT_C,10)
#define DIO_CHANNEL_C11	DIO_GET_CHANNEL(DIO_PORT_C,11)
#define DIO_CHANNEL_C12	DIO_GET_CHANNEL(DIO_PORT_C,12)
#define DIO_CHANNEL_C13	DIO_GET_CHANNEL(DIO_PORT_C,13)
#define DIO_CHANNEL_C14	DIO_GET_CHANNEL(DIO_PORT_C,14)
#define DIO_CHANNEL_C15	DIO_GET_CHANNEL(DIO_PORT_C,15)

#define DIO_CHANNEL_D0	DIO_GET_CHANNEL(DIO_PORT_D,0)
#define DIO_CHANNEL_D1	DIO_GET_CHANNEL(DIO_PORT_D,1)
#define DIO_CHANNEL_D2	DIO_GET_CHANNEL(DIO_PORT_D,2)
#define DIO_CHANNEL_D3	DIO_GET_CHANNEL(DIO_PORT_D,3)
#define DIO_CHANNEL_D4	DIO_GET_CHANNEL(DIO_PORT_D,4)
#define DIO_CHANNEL_D5	DIO_GET_CHANNEL(DIO_PORT_D,5)
#define DIO_CHANNEL_D6	DIO_GET_CHANNEL(DIO_PORT_D,6)
#define DIO_CHANNEL_D7	DIO_GET_CHANNEL(DIO_PORT_D,7)
#define DIO_CHANNEL_D8	DIO_GET_CHANNEL(DIO_PORT_D,8)
#define DIO_CHANNEL_D9	DIO_GET_CHANNEL(DIO_PORT_D,9)
#define DIO_CHANNEL_D10	DIO_GET_CHANNEL(DIO_PORT_D,10)
#define DIO_CHANNEL_D11	DIO_GET_CHANNEL(DIO_PORT_D,11)
#define DIO_CHANNEL_D12	DIO_GET_CHANNEL(DIO_PORT_D,12)
#define DIO_CHANNEL_D13	DIO_GET_CHANNEL(DIO_PORT_D,13)
#define DIO_CHANNEL_D14	DIO_GET_CHANNEL(DIO_PORT_D,14)
#define DIO_CHANNEL_D15	DIO_GET_CHANNEL(DIO_PORT_D,15)



/********************************************************
* @typedef Dio_PortType
* @brief kieu du lieu danh so thu tu cho cac port DIO
* @detail Khai bao kieu du lieu phai du lon de danh so duoc tat ca cac port DIO tren MCU
*****************/
typedef uint8 Dio_PortType;
#define DIO_PORT_A (Dio_PortType)(0x00)
#define DIO_PORT_B (Dio_PortType)(0x01)
#define DIO_PORT_C (Dio_PortType)(0x02)
#define DIO_PORT_D (Dio_PortType)(0x03)

/********************************************************
* @typedef Dio_ChannelGroupType
* @brief kieu cau truc de dinh nghia nhieu channel DIO tren 1 port
* @detail khai bao cac thanh phan bao gom mask,offset,port
*****************/
typedef struct{
	uint32 mask;	 //vi tri cua bit tren channel group
	uint8 offset;	 //vi tri cua bit tren port 
	Dio_PortType port;	//port xac dinh boi Channel Group
}Dio_ChannelGroupType;

/********************************************************
* @typedef Dio_LevelType
* @brief dinh nghia cac muc dien ap tren DIO channel
* @detail dinh nghia cac gia tri macro STD_HIGH,STD_LOW dai dien cho cac muc dien ap
*****************/


typedef uint8 Dio_LevelType; //kieu du lieu dai dien cho gia tri dien ap
#define STD_HIGH	(Dio_LevelType)(0x01) //muc dien ap 5 hoac 3.3v
#define STD_LOW		(Dio_LevelType)(0x00) //muc dien ap v
/********************************************************
* @typedef Dio_PortLevelType
* @detail tra ve so nguyen uint16 dai dien cho trang thai cua cac bit tren port
*****************/
typedef uint16 Dio_PortLevelType;

														 /*upper layer modules.*/

/********************************************************
* @brief Doc gia tri dien ap tren DIO channel
* @param ChannelId : chan DIO muon doc gia tri
* @return tra ve STD_HIGH hoac STD_LOW
*****************/
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId);

/********************************************************
* @brief ghi gia tri dien ap tren DIO channel
* @param ChannelId : chan DIO muon ghi gia tri , 
* @param Level: gia tri dien ap muon ghi
* @return void

*****************/
void Dio_WriteChannel(Dio_ChannelType ChannelId,Dio_LevelType Level);

/********************************************************
* @brief doc gia tri dien ap tren port DIO
* @param PortId: port DIO muon doc gia tri
* @return tra ve STD_HIGH hoac STD_LOW
*****************/
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId);

/********************************************************
* @brief doc gia tri dien ap tren port DIO
* @param PortId: port DIO muon doc gia tri
* @return tra ve 1 so nguyen bieu dien trang thai cac chan
*****************/
void Dio_WritePort(Dio_PortType PortId,Dio_PortLevelType Level);


/********************************************************
* @brief doc gia tri dien ap 1 nhom DIO channel
* @param ChannelGroupIdPtr: con tro den nhom DIO  channel can doc gia tri dien ap
* @return Dio_PortLevelType: gia tri so nguyen bieu dien muc dien ap cua nhom chan
*****************/
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr);

/********************************************************
* @brief ghi gia tri dien ap 1 nhom DIO channel
* @param ChannelGroupIdPtr: con tro den nhom DIO channel can doc gia tri dien ap
* @param Level: muc dien ap can ghi
*****************/
void Dio_WriteChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr,Dio_PortLevelType Level);

/********************************************************
* @brief doc gia tri phien ban kieu du lieu hien tai
* @param VersionInfo: con tro den cau truc luu tru du lieu phien ban
*****************/
void Dio_GetVersionInfo(Std_VersionInfoType* VersionInfo);

/********************************************************
* @brief dao gia tri dien ap tren DIO channel
* @param ChannelId: ID cua channel can dao trang thai
*****************/
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId);
#endif

