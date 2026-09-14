/**
 * @file Dio.h
 * @brief Giao diện public của driver DIO MCAL theo AUTOSAR.
 * @details Khai báo các dịch vụ công khai và các định danh ký hiệu mà các lớp
 *          phần mềm phía trên có thể sử dụng để đọc và ghi tín hiệu I/O kỹ thuật
 *          số của MCU.
 * @req AUTOSAR_SWS_DIODriver
 * @note File này định nghĩa API độc lập với phần cứng; chi tiết truy cập và ánh
 *       xạ theo MCU được đặt ở các file lớp thấp hơn.
 */
#ifndef DIO_H
#define DIO_H

#include "Dio_Cfg.h"


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
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);

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
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level);

/********************************************************
 * @brief doc gia tri dien ap 1 nhom DIO channel
 * @param ChannelGroupIdPtr: con tro den nhom DIO  channel can doc gia tri dien ap
 * @return Dio_PortLevelType: gia tri so nguyen bieu dien muc dien ap cua nhom chan
 *****************/
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr);

/********************************************************
 * @brief ghi gia tri dien ap 1 nhom DIO channel
 * @param ChannelGroupIdPtr: con tro den nhom DIO channel can doc gia tri dien ap
 * @param Level: muc dien ap can ghi
 *****************/
void Dio_WriteChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr, Dio_PortLevelType Level);

/********************************************************
 * @brief dao gia tri dien ap tren DIO channel
 * @param ChannelId: ID cua channel can dao trang thai
 *****************/
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId);

/********************************************************
 * @brief doc gia tri phien ban kieu du lieu hien tai
 * @param VersionInfo: con tro den cau truc luu tru du lieu phien ban
 *****************/
void Dio_GetVersionInfo(Std_VersionInfoType *VersionInfo);

#endif
