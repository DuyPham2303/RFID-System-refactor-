#include "Mcu_IrqMap.h"
#include "stm32f10x.h"

// Bảng map tập trung toàn cục cho chip
const IRQn_Type Mcu_IrqTable[MCU_IRQ_MAX] = {
    [MCU_GPT_CH1_IRQ] = TIM2_IRQn};

uint32 Mcu_MapPriorityGroup(Mcu_PriorityGroupType Id)
{
    switch (Id)
    {
    case MCU_PRIOR_GROUP_0:
        return NVIC_PriorityGroup_0;
    case MCU_PRIOR_GROUP_1:
        return NVIC_PriorityGroup_1;
    case MCU_PRIOR_GROUP_2:
        return NVIC_PriorityGroup_2;
    case MCU_PRIOR_GROUP_4:
        return NVIC_PriorityGroup_4;
    default:
        return 0;
    }
}