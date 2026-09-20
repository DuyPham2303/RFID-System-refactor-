/**
 * @file Pwm.h
 * @brief Khai báo giao diện cung cấp dịch vụ của mô-đun PWM.
 * @details Cung cấp các API để khởi tạo mô-đun PWM, thiết lập chu kỳ,
 *          thiết lập duty cycle, bắt đầu và dừng hoạt động của kênh PWM.
 * @req AUTOSAR_SWS_PWMDriver.
 * @note Các hàm trong file này là giao diện sử dụng bởi các tầng phía trên
 *       như IoHwAb và tầng ứng dụng.
 */
#ifndef PWM_H
#define PWM_H

#include "Pwm_Cfg.h"

/**
 * @brief  Khởi tạo module PWM.
 * @param  ConfigPtr: Con trỏ trỏ tới bảng cấu hình phần cứng.
 */
Std_ReturnType Pwm_Init(const Pwm_ConfigType_s *ConfigPtr);

/**
 * @brief  Thiết lập độ rộng xung (Duty Cycle) cho kênh PWM tương ứng (Dùng để điều khiển góc Servo).
 * @param  ChannelNumber: Kênh PWM (Ví dụ: PwmConf_PwmChannel_Servo).
 * @param  DutyCycle: Giá trị từ 0x0000 (0%) đến 0x8000 (100%).
 */
void Pwm_SetDutyCycle(Pwm_ChannelType ChannelNumber, Pwm_DutyCycleType DutyCycle);

/**
 * @brief  Đưa ngõ ra PWM về trạng thái nghỉ (Idle - thường là mức 0).
 * @param  ChannelNumber: Kênh PWM cần đưa về Idle.
 */
Std_ReturnType Pwm_SetOutputToIdle(Pwm_ChannelType ChannelNumber);

/**
 * @brief Returns current output logic level.
 * @param ChannelNumber PWM channel ID.
 * @return PWM_HIGH or PWM_LOW.
 */
Pwm_OutputStateType Pwm_GetOutputState(Pwm_ChannelType ChannelNumber);

#endif /* PWM_H */