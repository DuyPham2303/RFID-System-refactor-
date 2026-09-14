/**
 * @file Pwm_Cfg.h
 * @brief Giao diện cấu hình trình điều khiển PWM của MCAL.
 * @details Khai báo số lượng kênh PWM, tên tượng trưng và cấu hình
 *          được sử dụng bởi tầng ứng dụng và tầng dịch vụ.
 * @req Mô-đun PWM của AUTOSAR MCAL; cấu hình phụ thuộc vào từng project.
 */
#ifndef PWM_CFG_H
#define PWM_CFG_H

#include "Std_Types.h"
#include "Pwm_Types.h"
#include "./Gpt/Gpt_dtypes.h"

/**
 * @brief ID logic của các chức năng PWM được application sử dụng.
 * @details Application chỉ sử dụng các ID này. Ánh xạ tới timer và channel
 *          phần cứng được thực hiện trong bảng Pwm_Config của MCAL.
 * @note mỗi ID tượng trưng cho 1 kênh timer sử dụng
 */
typedef enum
{
    // PWM_CFG_SERVO_ID = 0U,
    PWM_CFG_CH1_ID = 0u,
    PWM_CFG_CH2_ID,
    PWM_CFG_CH3_ID,
    PWM_CFG_CH4_ID,
    PWM_CFG_COUNT
} Pwm_ConfigIdType;

/* Cấu trúc cấu hình tĩnh cho một kênh PWM */
typedef struct
{
    Pwm_OcModeType OcMode;           /* Trạng thái điện áp ngõ ra */
    Pwm_OutputStateType OutputState; /* Trạng thái ngõ ra chính */
    Pwm_Pulse PulseValue;            /* Giá trị Pulse (CCR) */
    Pwm_PolarityType Polarity;       /* Cực tính ngõ ra chính */
    Gpt_GroupType HwTimerId;         /* ID timer phần cứng   */
    Pwm_ChannelType ChannelId;       /* ID kênh PWM           */

} Pwm_ChannelConfigType_s;

/* Cấu trúc cấu hình tổng thể của module PWM */
typedef struct
{
    const Pwm_ChannelConfigType_s *ChannelConfig; /* con trỏ lưu trữ cấu hình kênh PWM */
    uint8 CfgID_Count;                            /* Số lượng ID logic cấu hình cho các tác vụ */
} Pwm_ConfigType_s;
extern const Pwm_ConfigType_s Pwm_Config;
#endif /* PWM_CFG_H */
