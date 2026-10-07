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
#include "Port_Types.h"

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

extern const Port_ConfigSetType g_Port_ConfigGroup;

#endif /* PORT_CFG_H */