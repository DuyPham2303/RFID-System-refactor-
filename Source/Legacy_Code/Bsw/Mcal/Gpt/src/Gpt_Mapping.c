#include "Gpt_Mapping.h"

TIM_TypeDef *GetTimerGroup(Gpt_GroupId_Type groupType)
{
    switch (groupType)
    {
    case GPT_GROUP_1:
        return TIM2;
    case GPT_GROUP_2:
        return TIM3;
    case GPT_GROUP_3:
        return TIM4;
    default:
        return NULL_PTR;
    }
}

uint16 GetCounterMode(Gpt_CounterModeType counterMode)
{
    switch (counterMode)
    {
    case GPT_COUNTER_MODE_UP:
        return TIM_CounterMode_Up;
    case GPT_COUNTER_MODE_DOWN:
        return TIM_CounterMode_Down;
    default:
        return NULL_PTR;
    }
}

uint16 GetClockDivider(Gpt_ClockDivType clockDiv)
{
    switch (clockDiv)
    {
    case GPT_CLOCK_DIV_1:
        return TIM_CKD_DIV1;
    case GPT_CLOCK_DIV_2:
        return TIM_CKD_DIV2;
    case GPT_CLOCK_DIV_4:
        return TIM_CKD_DIV4;
    default:
        return NULL_PTR;
    }
}

/**
 * @brief Hàm ánh xạ logic nguồn ngắt GPT sang cờ phần cứng TIM_IT của thư viện
 */
uint16_t Gpt_MapToHardwareItFlag(Gpt_IrqSourceType Source)
{
    switch (Source)
    {
    case GPT_IRQ_SOURCE_UPDATE:
        return TIM_IT_Update;
    case GPT_IRQ_SOURCE_CC1:
        return TIM_IT_CC1;
    case GPT_IRQ_SOURCE_CC2:
        return TIM_IT_CC2;
    case GPT_IRQ_SOURCE_CC3:
        return TIM_IT_CC3;
    case GPT_IRQ_SOURCE_CC4:
        return TIM_IT_CC4;
    default:
        return 0U;
    }
}
