#ifndef IOHWAB_PWM_CFG_H
#define IOHWAB_PWM_CFG_H

#include "Pwm_Types.h"

/* --- 1. THIẾT LẬP ÁNH XẠ LOGIC (MAPPING ID CHO TẦNG ỨNG DỤNG) --- */
typedef uint16 IoHwAb_PwmIdType;

#define IOHWAB_LED_RED_PWM ((IoHwAb_PwmIdType)0U)
#define IOHWAB_LED_GREEN_PWM ((IoHwAb_PwmIdType)1U)
#define IOHWAB_LED_BLUE_PWM ((IoHwAb_PwmIdType)2U)
#define IOHWAB_LED_YELLOW_PWM ((IoHwAb_PwmIdType)3U)
#define IOHWAB_MAX_LOGIC_CHANNELS ((IoHwAb_PwmIdType)4U)

/* Cấu trúc định nghĩa một kênh logic IoHwAb */
typedef struct
{
    IoHwAb_PwmIdType LogicChannelId; /* ID kênh logic của ứng dụng */
    Pwm_ChannelType McalChannelId;   /* ID kênh phần cứng tương ứng ở MCAL */
    uint16 SafeStateDuty;            /* Giá trị Duty an toàn khi khởi tạo */
} IoHwAb_PwmChannelConfigType;

typedef struct
{
    const IoHwAb_PwmChannelConfigType *ChannelConfigPtr;
    uint8 CfgID_Count; /* Số lượng kênh logic được cấu hình trong hệ thống */
} IoHwAb_PwmConfigType;

/* Khai báo extern cho biến cấu trúc cấu hình hệ thống */
extern const IoHwAb_PwmConfigType g_IoHwAb_Pwm_Config;

#endif /* IOHWAB_PWM_CFG_H */
