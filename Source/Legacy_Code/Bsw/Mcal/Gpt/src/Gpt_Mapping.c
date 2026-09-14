#include "Gpt_Mapping.h"

TIM_TypeDef *GetTimerGroup(Gpt_GroupType groupType)
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
