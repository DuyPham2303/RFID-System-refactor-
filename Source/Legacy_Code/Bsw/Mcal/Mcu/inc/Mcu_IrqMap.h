
#ifndef MCAL_IRQ_MAP_H
#define MCAL_IRQ_MAP_H
#include "Mcu_IrqTypes.h"
#include "stm32f10x.h"

extern const IRQn_Type Mcu_IrqTable[MCU_IRQ_MAX];

/*API ánh xạ logic ID của tầng ứng dụng sang Macro Group priority thực tế */
uint32 Mcu_MapPriorityGroup(Mcu_PriorityGroupType Id);
#endif
