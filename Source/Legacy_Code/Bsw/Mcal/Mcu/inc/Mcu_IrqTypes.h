/**
 * @file Mcu_IrqTypes.h
 * @brief Lựa chọn Kiểu cấu hình IRQ dùng chung cho các module MCAL.
 * @details Định nghĩa các kiểu dữ liệu ánh xạ cho IRQn, Priority Group. Các module MCAL
 *          chuyển kiểu trừu tượng này sang NVIC_InitTypeDef trong file .c.
 * @req AUTOSAR MCAL common support; cấu hình IRQ phụ thuộc vào MCU.
 */
#ifndef MCU_IRQ_TYPES_H
#define MCU_IRQ_TYPES_H
#include "Std_Types.h"
/**
 * @brief enum định danh ánh xạ cho group priority cấu hình
 * @details mỗi giá trị là 1 ID logic được tầng ứng dụng lựa chọn để cấu hình
 *          gián tiếp và truyền cho tầng dưới để ánh xạ sang macro cứng trên
 *          MCU thực tế
 */
typedef enum Mcu_PriorityGroupType
{
    MCU_PRIOR_GROUP_0 = 0U,
    MCU_PRIOR_GROUP_1,
    MCU_PRIOR_GROUP_2,
    MCU_PRIOR_GROUP_3,
    MCU_PRIOR_GROUP_4,
} Mcu_PriorityGroupType;

/**
 * @brief enum định danh cho tầng logic sử dụng để lấy IRQn muốn ánh xạ cho ngoại vi ngắt tương ứng
 * @details mỗi IRQn được CPU tra cứu trong vector table để gọi hàm ISR cần thiết
 */
typedef enum Mcu_IrqIdType
{
    MCU_GPT_CH1_IRQ = 0U,
    // ... các nguồn ngắt logic toàn cục
    MCU_IRQ_MAX
} Mcu_IrqIdType;

#endif /* MCU_IRQ_TYPES_H */
