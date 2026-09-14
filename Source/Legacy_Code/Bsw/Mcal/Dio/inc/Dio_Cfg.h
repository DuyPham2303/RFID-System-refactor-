/**
 * @file Dio_Cfg.h
 * @brief Định nghĩa cấu trúc dữ liệu cấu hình của DIO.
 * @details Chứa các cấu trúc dữ liệu dùng để mô tả các nhóm kênh DIO, bao gồm
 *          liên kết cổng, mask bit và thông tin offset.
 * @req AUTOSAR_SWS_DIODriver
 * @note File này dành cho dữ liệu cấu hình và nên tách biệt khỏi logic truy cập
 *       trực tiếp thanh ghi phần cứng.
 */
#ifndef DIO_CFG_H
#define DIO_CFG_H
#include "Dio_Types.h"
/********************************************************
 * @typedef Dio_ChannelGroupType
 * @brief kieu cau truc de dinh nghia nhieu channel DIO tren 1 port
 * @detail khai bao cac thanh phan bao gom mask,offset,port
 *****************/
typedef struct
{
    uint32 mask;       // vi tri cua bit tren channel group
    uint8 offset;      // vi tri cua bit thap nhat tren port
    Dio_PortType port; // port xac dinh boi Channel Group
} Dio_ChannelGroupType;

#endif /* DIO_CFG_H */
