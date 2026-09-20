#ifndef __MCU_IRQ_H
#define __MCU_IRQ_H

#include "Mcu_IrqTypes.h"
/**
 * @brief Cấu hình NVIC cho các nguồn ngắt đã được cấu hình.
 * @param ConfigPtr Con trỏ đến cấu hình NVIC.
 * @return Kết quả thực hiện.
 */
Std_ReturnType Mcu_IrqInit(const Mcu_IrqConfigType *ConfigPtr);

#endif /* MCU_IRQ_TYPES_H */