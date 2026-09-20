/**
 * @file Mcu_IrqTypes.h
 * @brief Lựa chọn Kiểu cấu hình IRQ dùng chung cho các module MCAL.
 * @details Định nghĩa các kiểu dữ liệu ánh xạ cho IRQn, Priority Group. Các module MCAL
 *          chuyển kiểu trừu tượng này sang NVIC_InitTypeDef trong file .c.
 * @req AUTOSAR MCAL common support; cấu hình IRQ phụ thuộc vào MCU.
 */
#ifndef __MCU_IRQ_TYPES_H
#define __MCU_IRQ_TYPES_H
#include "./Bsw/Services/Common/Std_Types.h"

/**
 * @brief enum định danh cho tầng logic sử dụng để lấy IRQn muốn ánh xạ cho ngoại vi ngắt tương ứng
 * @details mỗi IRQn được CPU tra cứu trong vector table để gọi hàm ISR cần thiết
 */
typedef enum Mcu_IrqIdType
{
    MCU_GPT_CH1_IRQ = 0U,
    MCU_GPT_CH2_IRQ,
    MCU_GPT_CH3_IRQ,
    MCU_SPI_CH1_IRQ,
    MCU_SPI_CH2_IRQ,
    // ... các nguồn ngắt logic toàn cục
    MCU_IRQ_MAX
} Mcu_IrqIdType;

/**
 * @brief Cấu hình NVIC dùng chung cho các module mcal
 */
typedef struct
{
    bool cmd;
    Mcu_IrqIdType IrqChannel;
    uint8 PreemptionPriority;
    uint8 SubPriority;
} Mcu_IrqConfigType;

/**
 * @brief Định nghĩa kiểu con trỏ hàm cho các hàm cấu hình IT phần cứng (Driver-specific API mapping)
 */
typedef void (*Mcal_ItConfigFuncType)(uint32_t Instance, uint16_t InterruptSource, boolean NewState);

#endif /* MCU_IRQ_TYPES_H */
