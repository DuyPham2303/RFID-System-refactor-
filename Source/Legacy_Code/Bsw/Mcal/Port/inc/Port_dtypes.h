/**
 * @file Port_dtypes.h
 * @brief Định nghĩa các kiểu dữ liệu cơ bản của module Port.
 * @details Bao gồm các kiểu cho cổng, chân pin, hướng vào/ra, mode và tốc độ của
 *          pin để tách giao diện Port khỏi phần cứng cụ thể của MCU.
 * @req AUTOSAR_SWS_PortDriver
 * @note Các kiểu này nên được giữ ở mức trừu tượng để tầng trên không phụ thuộc
 *       vào cách tổ chức thanh ghi của vi điều khiển.
 */
#ifndef PORT_DTYPES_H
#define PORT_DTYPES_H
#include "Std_Types.h"
typedef enum Port_PortType
{
    PORT_A = 0U,
    PORT_B,
    PORT_C,
    PORT_D
} Port_PortType;

/********************************************************
 * @typedef PORT_PinType
 * @brief kieu du lieu danh so thu tu cho cac port pin
 * @detail Khai bao kieu du lieu phai du lon de danh so duoc tat ca cac port pin tren MCU
 *****************/
typedef uint16 Port_PinType;

/*
 * @brief Macro để xác định bitmask của pin từ số pin.
 * @details Bitmask được xác định bằng cách dịch trái giá trị 1 bit theo số pin.
 * @return Giá trị bitmask tương ứng với số pin.
 */
#define GET_PORT_PIN(PinNumber) (Port_PinType)(1 << (PinNumber % 16))

#define PORT_PIN_A0 GET_PORT_PIN(0)
#define PORT_PIN_A1 GET_PORT_PIN(1)
#define PORT_PIN_A2 GET_PORT_PIN(2)
#define PORT_PIN_A3 GET_PORT_PIN(3)
#define PORT_PIN_A4 GET_PORT_PIN(4)
#define PORT_PIN_A5 GET_PORT_PIN(5)
#define PORT_PIN_A6 GET_PORT_PIN(6)
#define PORT_PIN_A7 GET_PORT_PIN(7)
#define PORT_PIN_A8 GET_PORT_PIN(8)
#define PORT_PIN_A9 GET_PORT_PIN(9)
#define PORT_PIN_A10 GET_PORT_PIN(10)
#define PORT_PIN_A11 GET_PORT_PIN(11)
#define PORT_PIN_A12 GET_PORT_PIN(12)
#define PORT_PIN_A13 GET_PORT_PIN(13)
#define PORT_PIN_A14 GET_PORT_PIN(14)
#define PORT_PIN_A15 GET_PORT_PIN(15)

#define PORT_PIN_B0 GET_PORT_PIN(16)
#define PORT_PIN_B1 GET_PORT_PIN(17)
#define PORT_PIN_B2 GET_PORT_PIN(18)
#define PORT_PIN_B3 GET_PORT_PIN(19)
#define PORT_PIN_B4 GET_PORT_PIN(20)
#define PORT_PIN_B5 GET_PORT_PIN(21)
#define PORT_PIN_B6 GET_PORT_PIN(22)
#define PORT_PIN_B7 GET_PORT_PIN(23)
#define PORT_PIN_B8 GET_PORT_PIN(24)
#define PORT_PIN_B9 GET_PORT_PIN(25)
#define PORT_PIN_B10 GET_PORT_PIN(26)
#define PORT_PIN_B11 GET_PORT_PIN(27)
#define PORT_PIN_B12 GET_PORT_PIN(28)
#define PORT_PIN_B13 GET_PORT_PIN(29)
#define PORT_PIN_B14 GET_PORT_PIN(30)
#define PORT_PIN_B15 GET_PORT_PIN(31)

#define PORT_PIN_C0 GET_PORT_PIN(32)
#define PORT_PIN_C1 GET_PORT_PIN(33)
#define PORT_PIN_C2 GET_PORT_PIN(34)
#define PORT_PIN_C3 GET_PORT_PIN(35)
#define PORT_PIN_C4 GET_PORT_PIN(36)
#define PORT_PIN_C5 GET_PORT_PIN(37)
#define PORT_PIN_C6 GET_PORT_PIN(38)
#define PORT_PIN_C7 GET_PORT_PIN(39)
#define PORT_PIN_C8 GET_PORT_PIN(40)
#define PORT_PIN_C9 GET_PORT_PIN(41)
#define PORT_PIN_C10 GET_PORT_PIN(42)
#define PORT_PIN_C11 GET_PORT_PIN(43)
#define PORT_PIN_C12 GET_PORT_PIN(44)
#define PORT_PIN_C13 GET_PORT_PIN(45)
#define PORT_PIN_C14 GET_PORT_PIN(46)
#define PORT_PIN_C15 GET_PORT_PIN(47)

#define PORT_PIN_D0 GET_PORT_PIN(48)
#define PORT_PIN_D1 GET_PORT_PIN(49)
#define PORT_PIN_D2 GET_PORT_PIN(50)
#define PORT_PIN_D3 GET_PORT_PIN(51)
#define PORT_PIN_D4 GET_PORT_PIN(52)
#define PORT_PIN_D5 GET_PORT_PIN(53)
#define PORT_PIN_D6 GET_PORT_PIN(54)
#define PORT_PIN_D7 GET_PORT_PIN(55)
#define PORT_PIN_D8 GET_PORT_PIN(56)
#define PORT_PIN_D9 GET_PORT_PIN(57)
#define PORT_PIN_D10 GET_PORT_PIN(58)
#define PORT_PIN_D11 GET_PORT_PIN(59)
#define PORT_PIN_D12 GET_PORT_PIN(60)
#define PORT_PIN_D13 GET_PORT_PIN(61)
#define PORT_PIN_D14 GET_PORT_PIN(62)
#define PORT_PIN_D15 GET_PORT_PIN(63)

/********************************************************
 * @typedef Port_PinDirectionType
 * @brief kieu du lieu enum xac dinh mode cua in/out chan port pin
 * @detail mode in/out duoc quy dinh la 0 hoac 1
 *****************/
typedef enum Port_PinDirectionType
{
    PORT_PIN_IN,
    PORT_PIN_OUT
} Port_PinDirectionType;

typedef enum Port_PinModeType
{
    PORT_MODE_ANALOG = 0U,
    PORT_MODE_INPUT_FLOATING,
    PORT_MODE_INPUT_PULL_DOWN,
    PORT_MODE_INPUT_PULL_UP,
    PORT_MODE_OUTPUT_OPEN_DRAIN,
    PORT_MODE_OUTPUT_PUSH_PULL,
    PORT_MODE_ALTERNATE_OPEN_DRAIN,
    PORT_MODE_ALTERNATE_PUSH_PULL
} Port_PinModeType;

typedef enum Port_PinSpeedType
{
    PORT_SPEED_2MHZ = 0U,
    PORT_SPEED_10MHZ,
    PORT_SPEED_50MHZ
} Port_PinSpeedType;

#endif /* PORT_DTYPES_H */