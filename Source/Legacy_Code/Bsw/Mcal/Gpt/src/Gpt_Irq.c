#include "Gpt_Cfg.h"
#include "Gpt_Mapping.h"

static void Gpt_IrqHandler(Gpt_GroupId_Type HwId)
{
    TIM_TypeDef *Gptx = GetTimerGroup(HwId);
    if (Gptx == NULL_PTR ||
        g_Gpt_ConfigGroup.ChannelConfigPtr == NULL_PTR)
    {
        return;
    }

    for (uint8 index = 0U; index < g_Gpt_ConfigGroup.GptCount; ++index)
    {
        const Gpt_ChannelConfigType_s *ChannelConfig = &g_Gpt_ConfigGroup.ChannelConfigPtr[index];
        const Gpt_CallbackConfigType *Notification = ChannelConfig->CallbackCfgPtr;

        if (ChannelConfig->HwId != HwId || Notification == NULL_PTR)
        {
            continue;
        }

        uint16_t ItFlag = Gpt_MapToHardwareItFlag(Notification->flag);
        if (ItFlag == 0U || TIM_GetITStatus(Gptx, ItFlag) == RESET)
        {
            continue;
        }

        TIM_ClearITPendingBit(Gptx, ItFlag);
        if (Notification->Noti != NULL_PTR)
        {
            Notification->Noti();
        }
    }
}
void TIM1_UP_IRQHandler(void)
{
    Gpt_IrqHandler(GPT_GROUP_1);
}
void TIM2_IRQHandler(void)
{
    Gpt_IrqHandler(GPT_GROUP_2);
}
void TIM3_IRQHandler(void)
{
    Gpt_IrqHandler(GPT_GROUP_3);
}
