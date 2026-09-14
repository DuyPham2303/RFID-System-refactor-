/**
 * @file Port_Cfg.h
 * @brief Định nghĩa cấu trúc dữ liệu cấu hình cho module Port.
 * @details Chứa các cấu trúc dùng để mô tả cách một pin hoặc một nhóm pin được
 *          cấu hình trong hệ thống, bao gồm cổng, chân, mode và tốc độ.
 * @req AUTOSAR_SWS_PortDriver
 * @note File này tập trung vào dữ liệu cấu hình, không chứa logic điều khiển phần
 *       cứng trực tiếp.
 */
#ifndef PORT_CFG_H
#define PORT_CFG_H
#include "Port_dtypes.h"
/**
 * @brief ID logic của các nhóm pin được application/IoHwAb sử dụng.
 * @details Các ID này không chứa thông tin phần cứng; ánh xạ port, pin và
 *          mode cụ thể được thực hiện trong bảng Port_Config của MCAL.
 */
typedef enum Port_ConfigIdType
{
    // SD_CARD_CFG_GENERAL_PIN = 0U,
    // SD_CARD_CFG_CS_PIN,
    // RC522_CFG_GENERAL_PIN,
    // RC522_CFG_RST_PIN,
    // RC522_CFG_CS_PIN,
    // LCD_CFG_GENERAL_PIN,
    // USBTTL_CFG_TX_PIN,
    // USBTTL_CFG_RX_PIN,
    // SERVO_CFG_GENERAL_PIN,
    LED_CFG_GENERAL_PIN = 0U,
    PORT_CFG_COUNT
} Port_ConfigIdType;
/********************************************************
 * @typedef Port_ConfigType
 * @brief kieu cau truc de cau hinh Port cho chan GPIO
 * @detail cau hinh pinMode,pull up/down, speed, OutputType,AltMode
 *****************/
typedef struct Port_ConfigType
{
    Port_PortType HwPortId;
    Port_PinType pin;
    Port_PinModeType ModeType;
    Port_PinSpeedType SpeedType;
} Port_ConfigType;

typedef struct Port_ConfigSetType
{
    const Port_ConfigType *PinCfgGroup;
    uint8 CfgID_Count;
} Port_ConfigSetType;

extern const Port_ConfigSetType Port_Config;

#endif /* PORT_CFG_H */