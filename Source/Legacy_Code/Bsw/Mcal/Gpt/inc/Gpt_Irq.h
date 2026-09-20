/**
 * @file Gpt_Irq.h
 * @brief Giao diện IRQ cho module GPT.
 * @details Khai báo các API cấu hình NVIC, đăng ký notification và xử lý
 *          ngắt timer. Phần triển khai nằm trong Gpt.c hoặc Gpt_Irq.c.
 */
#ifndef GPT_IRQ_H
#define GPT_IRQ_H
#include "./Bsw/Services/Common/Std_Types.h"
#include "Gpt_dtypes.h"

/**
 * @brief Đăng ký callback cho một ID GPT logic.
 * @param HwId ID cấu hình phần cứng GPT.
 * @param NotiId Id cấu hình đình danh hàm callback
 * @param Cb Con trỏ đến hàm callback.
 * @return Kết quả thực hiện.
 */
Std_ReturnType Gpt_RegisterNotification(
    Gpt_GroupId_Type HwId,
    Gpt_IdnotiType NotiId,
    Gpt_notificationPtr Cb);

/**
 * @brief Cho phép hoặc vô hiệu hóa notification của một ID GPT logic.
 * @param HwId ID cấu hình phần cứng GPT.
 * @param cmd Trạng thái enable/disable.
 * @return Kết quả thực hiện.
 */

Std_ReturnType Gpt_SetNotificationEnable(
    Gpt_GroupId_Type HwId,
    Gpt_IrqSourceType Flag,
    bool cmd);

/**
 * @brief Trả về callback đã đăng ký của một ID GPT logic.
 * @param HwId ID cấu hình phần cứng GPT.
 * @param NotiId Id cấu hình đình danh hàm callback
 * @return Con trỏ đến hàm callback.
 */
Gpt_notificationPtr Gpt_GetNotification(
    Gpt_GroupId_Type HwId,
    Gpt_IdnotiType NotiId);

#endif /* GPT_IRQ_H */
