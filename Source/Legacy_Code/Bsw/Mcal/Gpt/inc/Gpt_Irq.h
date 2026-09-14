/**
 * @file Gpt_Irq.h
 * @brief Giao diện IRQ cho module GPT.
 * @details Khai báo các API cấu hình NVIC, đăng ký notification và xử lý
 *          ngắt timer. Phần triển khai nằm trong Gpt.c hoặc Gpt_Irq.c.
 */
#ifndef GPT_IRQ_H
#define GPT_IRQ_H

#include "Gpt_Cfg.h"
#include "../Common/Mcal_IrqTypes.h"

/** Cấu hình NVIC cho các nguồn ngắt GPT đã được cấu hình. */
Std_ReturnType Gpt_IrqInit(const Mcal_IrqConfigType *ConfigPtr);

/** Đăng ký callback cho một ID GPT logic. */
Std_ReturnType Gpt_RegisterNotification(
    Gpt_ConfigIdType ConfigId,
    Mcal_IrqCallbackType Callback);

/** Cho phép hoặc vô hiệu hóa notification của một ID GPT logic. */
Std_ReturnType Gpt_SetNotificationEnable(
    Gpt_ConfigIdType ConfigId,
    bool Enable);

/** Trả về callback đã đăng ký của một ID GPT logic. */
Mcal_IrqCallbackType Gpt_GetNotification(Gpt_ConfigIdType ConfigId);

#endif /* GPT_IRQ_H */
