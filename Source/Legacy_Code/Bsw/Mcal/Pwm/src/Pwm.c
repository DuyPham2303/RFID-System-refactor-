#include "Pwm.h"
#include "Pwm_mapping.h"
/*biến lưu trữ nội bộ cấu hình pwm*/
static const Pwm_TimerConfigType *Pwm_ConfigPtr_s = NULL_PTR;

Std_ReturnType Pwm_Init(const Pwm_TimerConfigType *ConfigPtr)
{

    if (ConfigPtr == NULL_PTR ||
        (ConfigPtr->ChannelCount > 0U && ConfigPtr->ChannelsArray == NULL_PTR))
    {
        return E_NOT_OK;
    }

    /*truy xuát địa chỉ cứng của Timx*/
    TIM_TypeDef *Timx = Pwm_GetTimerGroup(ConfigPtr->HwTimerGroup);

    if (Timx == NULL_PTR)
    {
        return E_NOT_OK;
    }

    TIM_TimeBaseInitTypeDef TimbaseCfg;
    TIM_TimeBaseStructInit(&TimbaseCfg);
    TimbaseCfg.TIM_ClockDivision = Pwm_GetClockDivider(ConfigPtr->ClkDiv);
    TimbaseCfg.TIM_CounterMode = Pwm_GetCounterMode(ConfigPtr->ModeCntType);
    TimbaseCfg.TIM_Period = ConfigPtr->PeriodVal;
    TimbaseCfg.TIM_Prescaler = ConfigPtr->PresVal;
    TimbaseCfg.TIM_RepetitionCounter = ConfigPtr->RepCntVal;

    /* Cấu hình bộ đếm trước khi bật cấu hình các kênh đầu ra */
    TIM_TimeBaseInit(Timx, &TimbaseCfg);

    for (uint8 index = 0U; index < ConfigPtr->ChannelCount; index++)
    {
        /*khai báo cấu hình lưu trữ xuống phần cứng cho kênh Timx*/
        TIM_OCInitTypeDef PwmChannelCfg;

        /* đọc cấu hình kênh Timx tử lớp trên*/
        const Pwm_ChannelConfigType *Ch_config = &ConfigPtr->ChannelsArray[index];

        /*ánh xạ cấu hình từ lớp trên sang biến cục bộ */
        PwmChannelCfg.TIM_OCMode = GetPwmOcMode(Ch_config->OcMode);
        PwmChannelCfg.TIM_OutputState = GetPwmOutputState(Ch_config->OutputState);
        PwmChannelCfg.TIM_Pulse = Ch_config->DefaultDuty;
        PwmChannelCfg.TIM_OCPolarity = GetPwmOcPolarity(Ch_config->Polarity);

        switch (Ch_config->ChannelId)
        {
        case PWM_CHANNEL_CH1:
            TIM_OC1Init(Timx, &PwmChannelCfg);
            TIM_OC1PreloadConfig(Timx, TIM_OCPreload_Enable);
            break;
        case PWM_CHANNEL_CH2:
            TIM_OC2Init(Timx, &PwmChannelCfg);
            TIM_OC2PreloadConfig(Timx, TIM_OCPreload_Enable);
            break;
        case PWM_CHANNEL_CH3:
            TIM_OC3Init(Timx, &PwmChannelCfg);
            TIM_OC3PreloadConfig(Timx, TIM_OCPreload_Enable);
            break;
        case PWM_CHANNEL_CH4:
            TIM_OC4Init(Timx, &PwmChannelCfg);
            TIM_OC4PreloadConfig(Timx, TIM_OCPreload_Enable);
            break;
        default:
            return E_NOT_OK;
        }
    }
    /*kích hoạt  Gptx*/
    TIM_Cmd(Timx, ENABLE);

    /*lưu trữ toàn bộ cấu hình vào biến tĩnh để sử dụng nội bộ file*/
    Pwm_ConfigPtr_s = ConfigPtr;
    return E_OK;
}
void Pwm_SetDutyCycle(Pwm_ChannelType ChannelNumber, Pwm_PeriodValue Rawduty)
{
    uint8 index;

    /*kiểm tra con trỏ lưu trữ cục bộ có hợp lệ*/
    if (Pwm_ConfigPtr_s == NULL_PTR)
    {
        return;
    }

    /*truy xuất địa chỉ cứng của Timx cần tìm*/
    TIM_TypeDef *Timx = Pwm_GetTimerGroup(Pwm_ConfigPtr_s->HwTimerGroup);

    /*duyệt qua từng id kênh Timx để xuất pwm signal tương ứng với cổng*/
    for (index = 0U; index < Pwm_ConfigPtr_s->ChannelCount; index++)
    {
        /*tìm kiếm Id của Timx muốn xuất pwm*/
        if (Pwm_ConfigPtr_s->ChannelsArray[index].ChannelId != ChannelNumber)
        {
            continue; // bỏ qua khối lệnh bên dưới để tiếp tục duyệt trong for loop
        }

        /*xuất pwm theo cổng tương ứng với kênh Timx */
        switch (ChannelNumber)
        {
        case PWM_CHANNEL_CH1:
            TIM_SetCompare1(Timx, Rawduty);
            return;
        case PWM_CHANNEL_CH2:
            TIM_SetCompare2(Timx, Rawduty);
            return;
        case PWM_CHANNEL_CH3:
            TIM_SetCompare3(Timx, Rawduty);
            return;
        case PWM_CHANNEL_CH4:
            TIM_SetCompare4(Timx, Rawduty);
            return;
        default:
            return;
        }
    }
}