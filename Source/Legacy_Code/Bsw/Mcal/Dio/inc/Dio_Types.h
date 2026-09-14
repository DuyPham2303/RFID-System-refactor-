/**
 * @file Dio_Types.h
 * @brief Định nghĩa kiểu dữ liệu và hằng số logic của DIO theo AUTOSAR.
 * @details Định nghĩa các kiểu dữ liệu độc lập với nền tảng được sử dụng bởi API
 *          DIO, bao gồm định danh kênh, định danh cổng và mức logic.
 * @req AUTOSAR_SWS_DIODriver
 * @note Các kiểu này nhằm giữ tính di động trên nhiều họ MCU và tách lớp phần
 *       mềm phía trên khỏi chi tiết thanh ghi phần cứng.
 */
#ifndef DIO_TYPES_H
#define DIO_TYPES_H

#include "Std_Types.h"

/********************************************************
 * @typedef Dio_ChannelType
 * @brief kieu du lieu danh so thu tu cho cac chan DIO channel
 * @detail Khai bao kieu du lieu phai du lon de danh so duoc tat ca cac chan DIO tren MCU
 *****************/
typedef uint8 Dio_ChannelType;

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

/*
 * @brief Macro để xác định channelID từ port và pin.
 * @details ChannelID được xác định bằng cách dịch trái giá trị port 4 bit và cộng với giá trị pin.
 * @return Giá trị channelID tương ứng với port và pin.
 */
#define DIO_GET_CHANNEL(GPIOx, pin) (Dio_ChannelType)((GPIOx << 4) + (pin))

#define DIO_CHANNEL_A0 DIO_GET_CHANNEL(DIO_PORT_A, 0)
#define DIO_CHANNEL_A1 DIO_GET_CHANNEL(DIO_PORT_A, 1)
#define DIO_CHANNEL_A2 DIO_GET_CHANNEL(DIO_PORT_A, 2)
#define DIO_CHANNEL_A3 DIO_GET_CHANNEL(DIO_PORT_A, 3)
#define DIO_CHANNEL_A4 DIO_GET_CHANNEL(DIO_PORT_A, 4)
#define DIO_CHANNEL_A5 DIO_GET_CHANNEL(DIO_PORT_A, 5)
#define DIO_CHANNEL_A6 DIO_GET_CHANNEL(DIO_PORT_A, 6)
#define DIO_CHANNEL_A7 DIO_GET_CHANNEL(DIO_PORT_A, 7)
#define DIO_CHANNEL_A8 DIO_GET_CHANNEL(DIO_PORT_A, 8)
#define DIO_CHANNEL_A9 DIO_GET_CHANNEL(DIO_PORT_A, 9)
#define DIO_CHANNEL_A10 DIO_GET_CHANNEL(DIO_PORT_A, 10)
#define DIO_CHANNEL_A11 DIO_GET_CHANNEL(DIO_PORT_A, 11)
#define DIO_CHANNEL_A12 DIO_GET_CHANNEL(DIO_PORT_A, 12)
#define DIO_CHANNEL_A13 DIO_GET_CHANNEL(DIO_PORT_A, 13)
#define DIO_CHANNEL_A14 DIO_GET_CHANNEL(DIO_PORT_A, 14)
#define DIO_CHANNEL_A15 DIO_GET_CHANNEL(DIO_PORT_A, 15)

#define DIO_CHANNEL_B0 DIO_GET_CHANNEL(DIO_PORT_B, 0)
#define DIO_CHANNEL_B1 DIO_GET_CHANNEL(DIO_PORT_B, 1)
#define DIO_CHANNEL_B2 DIO_GET_CHANNEL(DIO_PORT_B, 2)
#define DIO_CHANNEL_B3 DIO_GET_CHANNEL(DIO_PORT_B, 3)
#define DIO_CHANNEL_B4 DIO_GET_CHANNEL(DIO_PORT_B, 4)
#define DIO_CHANNEL_B5 DIO_GET_CHANNEL(DIO_PORT_B, 5)
#define DIO_CHANNEL_B6 DIO_GET_CHANNEL(DIO_PORT_B, 6)
#define DIO_CHANNEL_B7 DIO_GET_CHANNEL(DIO_PORT_B, 7)
#define DIO_CHANNEL_B8 DIO_GET_CHANNEL(DIO_PORT_B, 8)
#define DIO_CHANNEL_B9 DIO_GET_CHANNEL(DIO_PORT_B, 9)
#define DIO_CHANNEL_B10 DIO_GET_CHANNEL(DIO_PORT_B, 10)
#define DIO_CHANNEL_B11 DIO_GET_CHANNEL(DIO_PORT_B, 11)
#define DIO_CHANNEL_B12 DIO_GET_CHANNEL(DIO_PORT_B, 12)
#define DIO_CHANNEL_B13 DIO_GET_CHANNEL(DIO_PORT_B, 13)
#define DIO_CHANNEL_B14 DIO_GET_CHANNEL(DIO_PORT_B, 14)
#define DIO_CHANNEL_B15 DIO_GET_CHANNEL(DIO_PORT_B, 15)

#define DIO_CHANNEL_C0 DIO_GET_CHANNEL(DIO_PORT_C, 0)
#define DIO_CHANNEL_C1 DIO_GET_CHANNEL(DIO_PORT_C, 1)
#define DIO_CHANNEL_C2 DIO_GET_CHANNEL(DIO_PORT_C, 2)
#define DIO_CHANNEL_C3 DIO_GET_CHANNEL(DIO_PORT_C, 3)
#define DIO_CHANNEL_C4 DIO_GET_CHANNEL(DIO_PORT_C, 4)
#define DIO_CHANNEL_C5 DIO_GET_CHANNEL(DIO_PORT_C, 5)
#define DIO_CHANNEL_C6 DIO_GET_CHANNEL(DIO_PORT_C, 6)
#define DIO_CHANNEL_C7 DIO_GET_CHANNEL(DIO_PORT_C, 7)
#define DIO_CHANNEL_C8 DIO_GET_CHANNEL(DIO_PORT_C, 8)
#define DIO_CHANNEL_C9 DIO_GET_CHANNEL(DIO_PORT_C, 9)
#define DIO_CHANNEL_C10 DIO_GET_CHANNEL(DIO_PORT_C, 10)
#define DIO_CHANNEL_C11 DIO_GET_CHANNEL(DIO_PORT_C, 11)
#define DIO_CHANNEL_C12 DIO_GET_CHANNEL(DIO_PORT_C, 12)
#define DIO_CHANNEL_C13 DIO_GET_CHANNEL(DIO_PORT_C, 13)
#define DIO_CHANNEL_C14 DIO_GET_CHANNEL(DIO_PORT_C, 14)
#define DIO_CHANNEL_C15 DIO_GET_CHANNEL(DIO_PORT_C, 15)

#define DIO_CHANNEL_D0 DIO_GET_CHANNEL(DIO_PORT_D, 0)
#define DIO_CHANNEL_D1 DIO_GET_CHANNEL(DIO_PORT_D, 1)
#define DIO_CHANNEL_D2 DIO_GET_CHANNEL(DIO_PORT_D, 2)
#define DIO_CHANNEL_D3 DIO_GET_CHANNEL(DIO_PORT_D, 3)
#define DIO_CHANNEL_D4 DIO_GET_CHANNEL(DIO_PORT_D, 4)
#define DIO_CHANNEL_D5 DIO_GET_CHANNEL(DIO_PORT_D, 5)
#define DIO_CHANNEL_D6 DIO_GET_CHANNEL(DIO_PORT_D, 6)
#define DIO_CHANNEL_D7 DIO_GET_CHANNEL(DIO_PORT_D, 7)
#define DIO_CHANNEL_D8 DIO_GET_CHANNEL(DIO_PORT_D, 8)
#define DIO_CHANNEL_D9 DIO_GET_CHANNEL(DIO_PORT_D, 9)
#define DIO_CHANNEL_D10 DIO_GET_CHANNEL(DIO_PORT_D, 10)
#define DIO_CHANNEL_D11 DIO_GET_CHANNEL(DIO_PORT_D, 11)
#define DIO_CHANNEL_D12 DIO_GET_CHANNEL(DIO_PORT_D, 12)
#define DIO_CHANNEL_D13 DIO_GET_CHANNEL(DIO_PORT_D, 13)
#define DIO_CHANNEL_D14 DIO_GET_CHANNEL(DIO_PORT_D, 14)
#define DIO_CHANNEL_D15 DIO_GET_CHANNEL(DIO_PORT_D, 15)

/*
 * @brief Macro để xác định port từ channelID.
 * @details Port được xác định bằng cách dịch phải channelID 4 bit.
 * @return Giá trị port tương ứng với channelID.
 */
#define DIO_GET_PIN_MASK(channelID) (1 << ((channelID) % 16))
/********************************************************
 * @typedef Dio_LevelType
 * @brief dinh nghia cac muc dien ap tren DIO channel
 * @detail dinh nghia cac gia tri macro STD_HIGH,STD_LOW dai dien cho cac muc dien ap
 *****************/

typedef uint8 Dio_LevelType;           // kieu du lieu dai dien cho gia tri dien ap
#define STD_HIGH (Dio_LevelType)(0x01) // muc dien ap 5 hoac 3.3v
#define STD_LOW (Dio_LevelType)(0x00)  // muc dien ap v
/********************************************************
 * @typedef Dio_PortLevelType
 * @detail tra ve so nguyen uint16 dai dien cho trang thai cua cac bit tren port
 *****************/
typedef uint16 Dio_PortLevelType;

/*
 * @brief Macro để xác định vị trí bit thấp nhất của một giá trị pin.
 * @details Vị trí bit thấp nhất được xác định bằng cách dịch phải giá trị pin cho đến khi gặp bit 1 đầu tiên.
 * @return Vị trí bit thấp nhất (offset) của giá trị pin.
 */
#define DIO_OFFSET_POSITION(pinNum)    \
    ({                                 \
        uint32 _tempPin = (pinNum);    \
        uint8 _offcnt = 0;             \
        if (_tempPin != 0)             \
        {                              \
            while (!(_tempPin & 0x01)) \
            {                          \
                _offcnt += 1;          \
                _tempPin >>= 1;        \
            }                          \
        }                              \
        _offcnt; /* Trả về giá trị */  \
    })

#endif /* DIO_TYPES_H */