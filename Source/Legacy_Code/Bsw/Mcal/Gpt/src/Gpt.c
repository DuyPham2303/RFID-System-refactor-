#include "Gpt.h"
#include "Gpt_Mapping.h"
#include "Mcu_Irq.h"

/*biến tĩnh lưu cấu hình nội bộ cho toàn bộ nhóm Gpt */
static const Gpt_ConfigType_s *Gpt_GroupConfig_s;

Std_ReturnType Gpt_Init(const Gpt_ConfigType_s *ConfigPtr)
{
    if (ConfigPtr == NULL_PTR)
    {
        return E_NOT_OK;
    }

    for (uint8 index = 0U; index < ConfigPtr->GptCount; index++)
    {

        /*khai báo cấu hình lưu trữ xuống thanh ghi thực tế cho nhóm Gpt hiện tại*/
        TIM_TimeBaseInitTypeDef Gpt_TimbaseCfg_s;

        /* Lấy cấu hình kênh GPT hiện tại từ mảng cấu hình */
        const Gpt_ChannelConfigType_s *ChannelConfig = &ConfigPtr->ChannelConfigPtr[index];

        /*truy cập cấu hình NVIC*/
        const Mcu_IrqConfigType *IrqConfig_s = ConfigPtr->ChannelConfigPtr[index].IrqCfgPtr;

        /*truyền con trỏ cấu hình xuống tầng thanh ghi*/
        Mcu_IrqInit(IrqConfig_s);

        /* Lấy nhóm timer từ ID timer phần cứng */
        TIM_TypeDef *Gpt_Group = GetTimerGroup(ChannelConfig->HwTimerId);

        /*xác nhận địa chỉ timer hợp lệ*/
        if (Gpt_Group == NULL_PTR)
        {
            return E_NOT_OK;
        }

        /*Lưu trữ cấu hình cục bộ cho toàn bộ nhóm Gpt*/
        Gpt_TimbaseCfg_s.TIM_ClockDivision = GetClockDivider(ChannelConfig->ClkDiv);
        Gpt_TimbaseCfg_s.TIM_CounterMode = GetCounterMode(ChannelConfig->ModeCntType);
        Gpt_TimbaseCfg_s.TIM_Period = ChannelConfig->PeriodVal;
        Gpt_TimbaseCfg_s.TIM_Prescaler = ChannelConfig->PresVal;

        /*ánh xạ cấu hình lưu trữ xuống thanh ghi cứng*/
        TIM_TimeBaseInit(Gpt_Group, &Gpt_TimbaseCfg_s);

        /*kích hoạt ngoại vi timer*/
        if (ChannelConfig->Cmd == GPT_ENABLE)
        {
            TIM_Cmd(Gpt_Group, ENABLE);
        }

        /*kích hoạt ngắt */
        if (IrqConfig_s->Enable)
        {
            TIM_ITConfig(Gpt_Group, TIM_IT_Update, ENABLE);
        }
    }
    /*copy cấu hình lưu trữ sang biến tĩnh sử dụng cục bộ*/
    Gpt_GroupConfig_s = ConfigPtr;
    return E_OK;
}

void Gpt_StartTimer(Gpt_GroupType GroupID, Gpt_PeriodValue TargetTime)
{
    /*lấy timer phần cứng*/
    TIM_TypeDef *Timer = GetTimerGroup(GroupID);
    if (Timer == NULL_PTR)
    {
        return;
    }
    /*cài đặt giá trị đếm ban đầu*/
    TIM_SetCounter(Timer, 0U);

    /*cài đặt giá trị tự động tải lại khi chạm giới hạn*/
    TIM_SetAutoreload(Timer, TargetTime);

    /* kich hoat timer */
    TIM_Cmd(Timer, ENABLE);
}

Gpt_PeriodValue Gpt_GetTimeElapsed(Gpt_GroupType HwTimerId)
{
    TIM_TypeDef *Timer = GetTimerGroup(HwTimerId);
    if (Timer == NULL_PTR)
    {
        return 0U;
    }
    return TIM_GetCounter(Timer);
}

void Gpt_StopTimer(Gpt_GroupType GroupID)
{
    TIM_TypeDef *Timer = GetTimerGroup(GroupID);
    if (Timer == NULL_PTR)
    {
        return;
    }
    TIM_Cmd(Timer, DISABLE);
}
