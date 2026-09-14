/**
 * @file Gpt.h
 * @brief Giao diện trình điều khiển bộ định thời đa mục đích MCAL.
 * @details Khai báo các kiểu dữ liệu TIM, cấu hình kênh, callback thông báo
 *          và các dịch vụ dùng để điều khiển các kênh timer của MCU.
 * @req Mô-đun TIM của AUTOSAR MCAL; phần triển khai phụ thuộc vào MCU.
 */
#ifndef GPT_H
#define GPT_H

#include "Std_Types.h"
#include "Gpt_Cfg.h"

/**
 * @brief  Khởi tạo module TIM.
 * @param  ConfigPtr: Con trỏ trỏ tới bảng cấu hình phần cứng Timer.
 */
Std_ReturnType Gpt_Init(const Gpt_ConfigType_s *ConfigPtr);

/**
 * @brief  Bắt đầu chạy Timer với chu kỳ đếm (tick) xác định.
 * @param  HwTimerId: Kênh Timer xử lý chức năng Gpt
 * @param  Value: giá trị đếm tràn trước khi phát sinh 1 update event.
 */
void Gpt_StartTimer(Gpt_GroupType HwTimerId, Gpt_PeriodValue Value);

/**
 * @brief  Dừng hoạt động của Timer.
 * @param  HwTimerId: Kênh Timer cần dừng.
 */
void Gpt_StopTimer(Gpt_GroupType HwTimerId);

/**
 * @brief Lấy giá trị thời gian đã trôi qua kể từ khi Timer bắt đầu chạy.
 * @param HwTimerId: Kênh Timer cần lấy giá trị.
 * @return Giá trị thời gian đã trôi qua (tick count).s
 */

Gpt_PeriodValue Gpt_GetTimeElapsed(Gpt_GroupType HwTimerId);
/**
 * @brief  Kích hoạt cơ chế ngắt thông báo (Notification) cho kênh Timer.
 * @param  HwTimerId: Kênh Timer tương ứng.
 */

void Gpt_EnableNotification(Gpt_GroupType HwTimerId);

/**
 * @brief  Vô hiệu hóa ngắt thông báo cho kênh Timer.
 * @param  HwTimerId: Kênh Timer tương ứng.
 */
void Gpt_DisableNotification(Gpt_GroupType HwTimerId);

#endif /* GPT_H */