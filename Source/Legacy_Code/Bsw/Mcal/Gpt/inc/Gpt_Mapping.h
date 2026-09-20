/**
 * @file Gpt_Mapping.h
 * @brief Ánh xạ các kiểu dữ liệu Timer từ AUTOSAR sang giá trị hardware cụ thể.
 * @details Định nghĩa các hàm inline để chuyển đổi từ các kiểu Timer AUTOSAR
 *          sang các giá trị và cấu trúc hardware tương ứng của STM32F103.
 * @req Tuân theo khái niệm ánh xạ Timer của AUTOSAR MCAL
 *      và yêu cầu riêng của dự án.s
 */
#ifndef GPT_MAPPING_H
#define GPT_MAPPING_H

#include "stm32f10x_tim.h" // Keil::Device:StdPeriph Drivers:Timer
#include "Gpt_dtypes.h"

/********************************************************
 * @brief Lấy con trỏ đếnTimer Group
 * @param groupType: ID nhóm GPT (Gpt_GroupId_Type)
 * @return Con trỏ đến cấu trúc TIM_TypeDef tương ứng
 ********************************************************/
TIM_TypeDef *GetTimerGroup(Gpt_GroupId_Type groupType);

/********************************************************
 * @brief Lấy chế độ đếm của Timer (Up/Down)
 * @param counterMode: Chế độ đếm từ cấu hình (Gpt_CounterModeType)
 * @return Giá trị CR1 DIR bit cho Timer
 ********************************************************/
uint16 GetCounterMode(Gpt_CounterModeType counterMode);

/********************************************************
 * @brief Lấy giá trị Clock Division (CKD)
 * @param clockDivl: Giá trị chia clock từ cấu hình (Gpt_ClockDivl)
 * @return Giá trị CKD cho Timer
 ********************************************************/
uint16 GetClockDivider(Gpt_ClockDivType clockDiv);

/**
 * @brief Hàm ánh xạ logic nguồn ngắt GPT sang cờ phần cứng TIM_IT của thư viện
 */
uint16_t Gpt_MapToHardwareItFlag(Gpt_IrqSourceType Source);

#endif /* GPT_MAPPING_H */
