#ifndef MCU_IRQ_CFG_H
#define MCU_IRQ_CFG_H

#include "Mcu_IrqTypes.h"

/**
 * @brief Cấu hình NVIC dùng chung cho các module mcal
 */
typedef struct
{
    bool Enable;
    Mcu_IrqIdType IrqChannel;
    uint8 PreemptionPriority;
    uint8 SubPriority;
} Mcu_IrqConfigType;

#endif
