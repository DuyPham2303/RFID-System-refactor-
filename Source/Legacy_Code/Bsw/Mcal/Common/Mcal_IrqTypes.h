/**
 * @file Mcal_IrqTypes.h
 * @brief Kiểu cấu hình IRQ dùng chung cho các module MCAL.
 * @details Định nghĩa cấu hình NVIC và callback chung. Các module MCAL
 *          chuyển kiểu trừu tượng này sang NVIC_InitTypeDef trong file .c.
 * @req AUTOSAR MCAL common support; cấu hình IRQ phụ thuộc vào MCU.
 */
#ifndef MCAL_IRQ_TYPES_H
#define MCAL_IRQ_TYPES_H

#include "Std_Types.h"
#include "stm32f10x.h"

/** Callback không tham số được gọi khi sự kiện ngắt xảy ra. */
typedef void (*Mcal_IrqCallbackType)(void);

/** Cấu hình NVIC cho một nguồn ngắt của module MCAL. */
typedef struct
{
    bool Enable;
    IRQn_Type IrqChannel;
    uint8 PreemptionPriority;
    uint8 SubPriority;
} Mcal_IrqConfigType;

#endif /* MCAL_IRQ_TYPES_H */
