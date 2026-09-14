/**
 * @file Pwm_mapping.h
 * @brief Ánh xạ các kiểu dữ liệu PWM từ AUTOSAR sang giá trị hardware cụ thể.
 * @details Định nghĩa các hàm inline để chuyển đổi từ các kiểu PWM AUTOSAR
 *          sang các giá trị và cấu trúc hardware tương ứng của STM32F103.
 * @req AUTOSAR_SWS_PWMDriver; tuân theo khái niệm ánh xạ PWM của AUTOSAR MCAL
 *      và yêu cầu riêng của dự án.
 * @note Đây là lớp ánh xạ phụ thuộc phần cứng, không thuộc giao diện
 *       chuẩn của AUTOSAR và cần được tùy chỉnh theo vi điều khiển.
 */
#ifndef PWM_MAPPING_H
#define PWM_MAPPING_H

#include "Pwm_Types.h"
#include "./Gpt/Gpt_Mapping.h"
/********************************************************
 * @brief Lấy chế độ Output Compare cho PWM
 * @param ocMode: Chế độ OC từ cấu hình (Pwm_OcModeType)
 * @return Giá trị chế độ OC cho thanh ghi CR1 của Timer
 ********************************************************/
uint16 GetPwmOcMode(Pwm_OcModeType ocMode);

/********************************************************
 * @brief Lấy trạng thái ngõ ra chính cho PWM
 * @param outputState: Trạng thái từ cấu hình (Pwm_OutputStateType)
 * @return Giá trị trạng thái cho thanh ghi CCER của Timer
 ********************************************************/
uint16 GetPwmOutputState(Pwm_OutputStateType outputState);

/********************************************************
 * @brief Lấy giá trị xung (Pulse) cho PWM
 * @param pulse: Giá trị Pulse từ cấu hình (Pwm_Pulse)
 * @return Giá trị Pulse cho thanh ghi CCR tương ứng
 ********************************************************/
uint16 GetPwmPulseValue(Pwm_Pulse pulse);

/********************************************************
 * @brief Lấy cực tính ngõ ra chính cho PWM
 * @param polarity: Cực tính từ cấu hình (Pwm_PolarityType)
 * @return Giá trị cực tính cho thanh ghi CCER của Timer
 ********************************************************/
uint16 GetPwmOcPolarity(Pwm_PolarityType polarity);

/********************************************************
 * @brief Lấy trạng thái ngõ ra bổ sung (Complementary) cho PWM
 * @param outputNState: Trạng thái từ cấu hình (Pwm_OutputNStateType)
 * @return Giá trị trạng thái cho thanh ghi CCER của Timer
 ********************************************************/
uint16 GetPwmOutputNState(Pwm_OutputNStateType outputNState);

/********************************************************
 * @brief Lấy cực tính ngõ ra bổ sung (Complementary) cho PWM
 * @param polarityN: Cực tính bổ sung từ cấu hình (Pwm_PolarityNType)
 * @return Giá trị cực tính bổ sung cho thanh ghi CCER của Timer
 ********************************************************/
uint16 GetPwmOcNPolarity(Pwm_PolarityNType polarityN);

/********************************************************
 * @brief Lấy trạng thái Idle của ngõ ra chính
 * @param idleState: Trạng thái Idle từ cấu hình (Pwm_IdleStateType)
 * @return Giá trị trạng thái Idle cho thanh ghi CR2 của Timer
 ********************************************************/
uint16 GetPwmOcIdleState(Pwm_IdleStateType idleState);

/********************************************************
 * @brief Lấy trạng thái Idle của ngõ ra bổ sung
 * @param idleStateN: Trạng thái Idle bổ sung từ cấu hình (Pwm_IdleStateNType)
 * @return Giá trị trạng thái Idle bổ sung cho thanh ghi CR2 của Timer
 ********************************************************/
uint16 GetPwmOcNIdleState(Pwm_IdleStateNType idleStateN);

#endif /* PWM_MAPPING_H */