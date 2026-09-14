/**
 * @file Gpt.c
 * @brief Phần triển khai trình điều khiển TIM của MCAL.
 *
 * @section gpt_implementation_guidance Hướng dẫn triển khai
 *
 * - Trong Gpt_Init(), duyệt qua bảng cấu hình, cấu hình tần số cơ sở cho
 *   timer vật lý tương ứng (HwTimerId), sau đó lưu con trỏ hàm callback
 *   (Notification) vào một mảng tĩnh bên trong file này.
 *
 * - Khi phần cứng STM32 phát sinh ngắt tràn (Update Interrupt) của TIMx,
 *   trong hàm ISR (Interrupt Service Routine) tương ứng, gọi con trỏ hàm
 *   callback nếu notification đã được kích hoạt qua Gpt_EnableNotification.
 *   Nhờ đó, logic xử lý ngắt có thể được truyền ra tầng bên ngoài linh hoạt
 *   mà không bị gắn cứng vào phần driver phần cứng.
 */
#include "Gpt.h"
#include "Gpt_Mapping.h"

/*Biến tĩnh lưu trữ cấu hình cục bộ hàm*/
static const Gpt_ConfigType_s *Gpt_GroupConfig_s;

Std_ReturnType Gpt_Init(const Gpt_ConfigType_s *ConfigPtr)
{
    if (ConfigPtr == NULL_PTR)
    {
        return E_NOT_OK;
    }

    for (uint8 index = 0U; index < ConfigPtr->GptCount; index++)
    {

        /*khai báo cấu hình lưu trữ xuống thanh ghi thực tế*/
        TIM_TimeBaseInitTypeDef Gpt_TimbaseCfg_s;

        /* Lấy cấu hình kênh GPT hiện tại từ mảng cấu hình */
        const Gpt_ChannelConfigType_s *ChannelConfig = &ConfigPtr->ChannelConfigPtr[index];

        /* Lấy nhóm timer từ ID timer phần cứng */
        TIM_TypeDef *Gpt_Group = GetTimerGroup(ChannelConfig->HwTimerId);

        /*xác nhận địa chỉ timer hợp lệ truy xuất qua ID enum*/
        if (Gpt_Group == NULL_PTR)
        {
            return E_NOT_OK;
        }

        Gpt_TimbaseCfg_s.TIM_ClockDivision = GetClockDivider(ChannelConfig->ClkDiv);
        Gpt_TimbaseCfg_s.TIM_CounterMode = GetCounterMode(ChannelConfig->ModeCntType);
        Gpt_TimbaseCfg_s.TIM_Period = ChannelConfig->PeriodVal;
        Gpt_TimbaseCfg_s.TIM_Prescaler = ChannelConfig->PresVal;

        /*ánh xạ cấu hình lưu trữ xuống thanh ghi cứng*/
        TIM_TimeBaseInit(Gpt_Group, &Gpt_TimbaseCfg_s);

        if (ChannelConfig->Cmd == GPT_ENABLE)
        {
            TIM_Cmd(Gpt_Group, ENABLE);
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
