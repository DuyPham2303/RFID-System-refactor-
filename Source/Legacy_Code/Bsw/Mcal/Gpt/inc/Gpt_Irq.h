/**
 * @file Gpt_Irq.h
 * @brief Giao diện IRQ cho module GPT.
 * @details Khai báo các API cấu hình NVIC, đăng ký notification và xử lý
 *          ngắt timer. Phần triển khai nằm trong Gpt.c hoặc Gpt_Irq.c.
 */
#ifndef GPT_IRQ_H
#define GPT_IRQ_H
#include "Std_Types.h"
typedef enum Gpt_CbServiceType
{
    GPT_BLINKLED_GROUP1 = 0U,
    GPT_MAX_GROUP
} Gpt_CbServiceType;
// Định nghĩa kiểu cho hàm callback của GPT
typedef void (*Gpt_ApiCallbackPtr)(void);

/**
 * @brief Đăng ký callback cho một ID GPT logic.
 * @param ServiceId ID cấu hình GPT.
 * @param Cb Con trỏ đến hàm callback.
 * @return Kết quả thực hiện.
 */
Std_ReturnType Gpt_RegisterApiCallback(
    Gpt_CbServiceType ServiceId,
    Gpt_ApiCallbackPtr Cb);

/**
 * @brief Cho phép hoặc vô hiệu hóa notification của một ID GPT logic.
 * @param ServiceId ID cấu hình GPT.
 * @param Enable Trạng thái cho phép.
 * @return Kết quả thực hiện.
 */
Std_ReturnType Gpt_SetCallbackEnable(
    Gpt_CbServiceType ServiceId,
    bool Enable);

/**
 * @brief Trả về callback đã đăng ký của một ID GPT logic.
 * @param ServiceId ID cấu hình GPT.
 * @return Con trỏ đến hàm callback.
 */
Gpt_ApiCallbackPtr Gpt_GetCallback(Gpt_CbServiceType ServiceId);

#endif /* GPT_IRQ_H */
