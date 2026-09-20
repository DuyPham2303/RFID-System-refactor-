#include "Gpt.h"
#include "Gpt_Mapping.h"
#include "Gpt_Irq.h"
#include "Mcu_Irq.h"

/*biến tĩnh lưu cấu hình nội bộ cho toàn bộ nhóm Gpt */
static const Gpt_ConfigType_s *Gpt_Config_s;

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

        /* Lấy nhóm timer từ ID timer phần cứng */
        TIM_TypeDef *Gptx = GetTimerGroup(ChannelConfig->HwTimerId);

        /*xác nhận địa chỉ timer hợp lệ*/
        if (Gptx == NULL_PTR)
        {
            return E_NOT_OK;
        }

        const Mcu_IrqConfigType *IrqConfig_s = NULL_PTR;
        const Gpt_IrqConfigNotiType *IrqConfigNoti_s = NULL_PTR;

        /*cấu hình NVIC*/
        if (ChannelConfig->IrqCfgPtr != NULL_PTR && ChannelConfig->notiPtr != NULL_PTR)
        {
            /*đọc cấu hình NVIC và ánh xạ xuống thanh ghi phần cứng*/
            IrqConfig_s = ChannelConfig->IrqCfgPtr;
            Mcu_IrqInit(IrqConfig_s);

            /*truy cập đăng ký callback Api và cấu hình cờ ngắt*/
            IrqConfigNoti_s = ChannelConfig->notiPtr;
            Gpt_RegisterNotification(
                ChannelConfig->HwTimerId,
                (Gpt_IdnotiType)index,
                IrqConfigNoti_s->cb);
        }

        /*Lưu trữ cấu hình cục bộ cho toàn bộ nhóm Gpt*/
        Gpt_TimbaseCfg_s.TIM_ClockDivision = GetClockDivider(ChannelConfig->ClkDiv);
        Gpt_TimbaseCfg_s.TIM_CounterMode = GetCounterMode(ChannelConfig->ModeCntType);
        Gpt_TimbaseCfg_s.TIM_Period = ChannelConfig->PeriodVal;
        Gpt_TimbaseCfg_s.TIM_Prescaler = ChannelConfig->PresVal;

        /*ánh xạ cấu hình lưu trữ xuống thanh ghi cứng*/
        TIM_TimeBaseInit(Gptx, &Gpt_TimbaseCfg_s);

        /*kích hoạt  timer*/

        TIM_Cmd(Gptx, ChannelConfig->Cmd);

        /*enable/disable ngắt và ánh xạ cờ sự kiện xử lý tương ứng*/

        if (IrqConfig_s->cmd == ENABLE)
        {
            Gpt_SetNotificationEnable(
                ChannelConfig->HwTimerId,
                IrqConfigNoti_s->flag,
                ChannelConfig->IrqCfgPtr->cmd);
        }
    }
    /*copy cấu hình lưu trữ sang biến tĩnh sử dụng cục bộ*/
    Gpt_Config_s = ConfigPtr;
    return E_OK;
}

void Gpt_StartTimer(Gpt_GroupId_Type GroupID, Gpt_PeriodValue TargetTime)
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

void Gpt_StopTimer(Gpt_GroupId_Type GroupID)
{
    TIM_TypeDef *Timer = GetTimerGroup(GroupID);
    if (Timer == NULL_PTR)
    {
        return;
    }
    TIM_Cmd(Timer, DISABLE);
}

Gpt_PeriodValue Gpt_GetTimeElapsed(Gpt_GroupId_Type HwTimerId)
{
    TIM_TypeDef *Timer = GetTimerGroup(HwTimerId);
    if (Timer == NULL_PTR)
    {
        return 0U;
    }
    return TIM_GetCounter(Timer);
}
