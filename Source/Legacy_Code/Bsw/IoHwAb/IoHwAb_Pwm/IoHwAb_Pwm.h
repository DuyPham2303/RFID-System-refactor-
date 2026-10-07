#ifndef IOHWAB_PWM_H
#define IOHWAB_PWM_H

#include "IoHwAb_Pwm_Cfg.h"

/**
 * @brief Loại dữ liệu biểu diễn giá trị duty cycle được tầng phía trên truyền vào.
 * @details Giá trị đại diện cho độ rộng xung logic của Pwm (từ 0 - 100%).
 *          Giá trị này được ánh xạ sang giá trị thô (Raw Duty) của driver MCAL dựa
 *          trên chu kỳ (Period) của bộ Timer.
 */
typedef uint16 Pwm_DutyCycleType;

/* --- CÁC NGUYÊN MẪU HÀM API CHUẨN --- */
void IoHwAb_PwmInit(const IoHwAb_PwmConfigType *ConfigPtr);
void IoHwAb_Pwm_SetDuty(IoHwAb_PwmIdType ChannelId, Pwm_DutyCycleType DutyValue);

#endif /* IOHWAB_PWM_H */
