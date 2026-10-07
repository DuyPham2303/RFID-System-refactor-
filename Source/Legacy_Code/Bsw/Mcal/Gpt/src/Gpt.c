#include "Gpt.h"
#include "Gpt_Mapping.h"

#include "Mcu_Irq.h"

/*biến tĩnh lưu cấu hình nội bộ cho toàn bộ nhóm Gpt */
static const Gpt_ConfigType_s *s_Gpt_ConfigGroup;

Std_ReturnType Gpt_Init(const Gpt_ConfigType_s *ConfigPtr)
{
    if (ConfigPtr == NULL_PTR)
    {
        return E_NOT_OK;
    }

    for (uint8 index = 0U; index < ConfigPtr->GptCount; index++)
    {

        /*khai báo cấu hình lưu trữ xuống thanh ghi thực tế cho nhóm Gpt hiện tại*/
        TIM_TimeBaseInitTypeDef Gpt_TimbaseInit;

        /* Lấy cấu hình kênh GPT hiện tại từ mảng cấu hình */
        const Gpt_ChannelConfigType_s *ChannelConfig = &ConfigPtr->ChannelConfigPtr[index];

        /* Lấy nhóm Gptx từ ID Gptx phần cứng */
        Gpt_GroupId_Type HwId = ChannelConfig->HwId;
        TIM_TypeDef *Gptx = GetTimerGroup(HwId);

        /*xác nhận địa chỉ Gptx hợp lệ*/
        if (Gptx == NULL_PTR)
        {
            return E_NOT_OK;
        }

        /*đọc cấu hình NVIC và ánh xạ xuống thanh ghi phần cứng*/
        if (ChannelConfig->NvicCfgPtr != NULL_PTR)
        {
            Mcu_IrqInit(ChannelConfig->NvicCfgPtr);
        }

        /*Lưu trữ cấu hình cục bộ cho toàn bộ nhóm Gpt*/
        Gpt_TimbaseInit.TIM_ClockDivision = GetClockDivider(ChannelConfig->ClkDiv);
        Gpt_TimbaseInit.TIM_CounterMode = GetCounterMode(ChannelConfig->ModeCntType);
        Gpt_TimbaseInit.TIM_Period = ChannelConfig->PeriodVal;
        Gpt_TimbaseInit.TIM_Prescaler = ChannelConfig->PresVal;
        Gpt_TimbaseInit.TIM_RepetitionCounter = ChannelConfig->RepCntVal;

        /*ánh xạ cấu hình lưu trữ xuống thanh ghi cứng*/
        TIM_TimeBaseInit(Gptx, &Gpt_TimbaseInit);

        /*kích hoạt  Gptx*/
        TIM_Cmd(Gptx, ChannelConfig->Cmd);

        if (ChannelConfig->CallbackCfgPtr != NULL_PTR)
        {
            /*đăng ký hàm callback*/
            TIM_ITConfig(Gptx, Gpt_MapToHardwareItFlag(ChannelConfig->CallbackCfgPtr->flag), ENABLE);
        }
    }
    /*copy cấu hình lưu trữ sang biến tĩnh sử dụng cục bộ*/
    s_Gpt_ConfigGroup = ConfigPtr;
    return E_OK;
}

void Gpt_StartTimer(Gpt_GroupId_Type GroupID, Gpt_PeriodValue TargetTime)
{
    /*lấy Gptx phần cứng*/
    TIM_TypeDef *Gptx = GetTimerGroup(GroupID);
    if (Gptx == NULL_PTR)
    {
        return;
    }
    /*cài đặt giá trị đếm ban đầu*/
    TIM_SetCounter(Gptx, 0U);

    /*cài đặt giá trị tự động tải lại khi chạm giới hạn*/
    TIM_SetAutoreload(Gptx, TargetTime);

    /* kich hoat Gptx */
    TIM_Cmd(Gptx, ENABLE);
}

void Gpt_StopTimer(Gpt_GroupId_Type GroupID)
{
    TIM_TypeDef *Gptx = GetTimerGroup(GroupID);
    if (Gptx == NULL_PTR)
    {
        return;
    }
    TIM_Cmd(Gptx, DISABLE);
}

Gpt_PeriodValue Gpt_GetTimeElapsed(Gpt_GroupId_Type HwId)
{
    TIM_TypeDef *Gptx = GetTimerGroup(HwId);
    if (Gptx == NULL_PTR)
    {
        return 0U;
    }
    return TIM_GetCounter(Gptx);
}
