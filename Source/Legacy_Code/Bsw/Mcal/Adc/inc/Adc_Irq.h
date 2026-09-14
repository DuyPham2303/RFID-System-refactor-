/**
 * @file Adc_Irq.h
 * @brief Giao diện IRQ và notification cho module ADC.
 * @details Khai báo cấu hình NVIC và API đăng ký callback theo nhóm ADC.
 *          Phần chuyển đổi sang thanh ghi NVIC/ADC nằm trong file .c của MCAL.
 */
#ifndef ADC_IRQ_H
#define ADC_IRQ_H

#include "Adc_Types.h"
#include "../Common/Mcal_IrqTypes.h"

/** Cấu hình NVIC cho nguồn ngắt ADC. */
Std_ReturnType Adc_IrqInit(const Mcal_IrqConfigType *ConfigPtr);

/** Đăng ký callback cho một nhóm ADC logic. */
Std_ReturnType Adc_RegisterNotification(
    Adc_GroupType Group,
    Mcal_IrqCallbackType Callback);

/** Cho phép hoặc vô hiệu hóa notification của một nhóm ADC. */
Std_ReturnType Adc_SetNotificationEnable(
    Adc_GroupType Group,
    bool Enable);

/** Trả về callback đã đăng ký của một nhóm ADC. */
Mcal_IrqCallbackType Adc_GetNotification(Adc_GroupType Group);

#endif /* ADC_IRQ_H */
