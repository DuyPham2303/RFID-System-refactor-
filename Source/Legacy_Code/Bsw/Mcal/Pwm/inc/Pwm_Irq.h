/**
 * @file Pwm_Irq.h
 * @brief Giao diện IRQ tùy chọn cho module PWM.
 * @details PWM chỉ cần header này khi sử dụng update hoặc compare interrupt.
 *          Nếu PWM chỉ xuất xung, không cần triển khai các API IRQ này.
 */
#ifndef PWM_IRQ_H
#define PWM_IRQ_H

#include "Pwm_Cfg.h"
#include "../Common/Mcal_IrqTypes.h"

/** Cấu hình NVIC cho nguồn ngắt PWM. */
Std_ReturnType Pwm_IrqInit(const Mcal_IrqConfigType *ConfigPtr);

/** Đăng ký callback cho một ID PWM logic. */
Std_ReturnType Pwm_RegisterNotification(
    Pwm_ConfigIdType ConfigId,
    Mcal_IrqCallbackType Callback);

/** Cho phép hoặc vô hiệu hóa notification của một ID PWM logic. */
Std_ReturnType Pwm_SetNotificationEnable(
    Pwm_ConfigIdType ConfigId,
    bool Enable);

/** Trả về callback đã đăng ký của một ID PWM logic. */
Mcal_IrqCallbackType Pwm_GetNotification(Pwm_ConfigIdType ConfigId);

#endif /* PWM_IRQ_H */
