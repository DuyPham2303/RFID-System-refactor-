#include "Pwm.h"
#include "Pwm_mapping.h"
#include "stm32f10x_tim.h"

/*biến lưu trữ nội bộ cấu hình pwm*/
static const Pwm_ConfigType_s *Pwm_ConfigPtr_s;
static Pwm_ErrorType Pwm_LastError_s = PWM_ERROR_NONE;

static Std_ReturnType Pwm_SetError(Pwm_ErrorType error)
{
    Pwm_LastError_s = error;
    return E_NOT_OK;
}

Pwm_ErrorType Pwm_GetLastError(void)
{
    return Pwm_LastError_s;
}

Std_ReturnType Pwm_ValidateConfig(const Pwm_ConfigType_s *ConfigPtr)
{
    uint8 index;
    uint8 otherIndex;

    if (ConfigPtr == NULL_PTR)
    {
        return Pwm_SetError(PWM_ERROR_NULL_CONFIG);
    }
    if (ConfigPtr->CfgID_Count == 0U)
    {
        return Pwm_SetError(PWM_ERROR_EMPTY_CONFIG);
    }
    if (ConfigPtr->CfgID_Count > PWM_CFG_COUNT)
    {
        return Pwm_SetError(PWM_ERROR_CONFIG_COUNT);
    }

    for (index = 0U; index < ConfigPtr->CfgID_Count; index++)
    {
        const Pwm_ChannelConfigType_s *config = &ConfigPtr->ChannelConfig[index];

        if (config->HwTimerId > GPT_GROUP_3)
        {
            return Pwm_SetError(PWM_ERROR_INVALID_TIMER);
        }
        if (config->ChannelId < PWM_CHANNEL_1 || config->ChannelId > PWM_CHANNEL_4)
        {
            return Pwm_SetError(PWM_ERROR_INVALID_CHANNEL);
        }
        if (config->OcMode > PWM_OC_MODE_PWM2 ||
            config->OutputState > PWM_OUTPUT_ENABLED ||
            config->Polarity > PWM_POLARITY_LOW)
        {
            return Pwm_SetError(PWM_ERROR_INVALID_OPTION);
        }

        for (otherIndex = 0U; otherIndex < index; otherIndex++)
        {
            const Pwm_ChannelConfigType_s *other = &ConfigPtr->ChannelConfig[otherIndex];
            if (other->HwTimerId == config->HwTimerId &&
                other->ChannelId == config->ChannelId)
            {
                return Pwm_SetError(PWM_ERROR_DUPLICATE_CHANNEL);
            }
        }
    }

    Pwm_LastError_s = PWM_ERROR_NONE;
    return E_OK;
}
Std_ReturnType Pwm_Init(const Pwm_ConfigType_s *ConfigPtr)
{
    uint8 index;

    if (Pwm_ValidateConfig(ConfigPtr) != E_OK)
    {
        return E_NOT_OK;
    }

    for (index = 0U; index < ConfigPtr->CfgID_Count; index++)
    {
        /*khai báo cấu hình lưu trữ xuống phần cứng cho kênh timer*/
        TIM_OCInitTypeDef channelConfig;

        /* đọc cấu hình kênh timer tử lớp trên*/
        const Pwm_ChannelConfigType_s *config = &ConfigPtr->ChannelConfig[index];

        /*truy xuát địa chỉ cứng của timer*/
        TIM_TypeDef *timer = GetTimerGroup(config->HwTimerId);

        /*ánh xạ cấu hình từ lớp trên sang biến cục bộ */
        channelConfig.TIM_OCMode = GetPwmOcMode(config->OcMode);
        channelConfig.TIM_OutputState = GetPwmOutputState(config->OutputState);
        channelConfig.TIM_Pulse = GetPwmPulseValue(config->PulseValue);
        channelConfig.TIM_OCPolarity = GetPwmOcPolarity(config->Polarity);

        if (timer == NULL_PTR)
        {
            return Pwm_SetError(PWM_ERROR_INVALID_TIMER);
        }

        /*cài đặt cổng xuất pwm cho kênh timer tương ứng*/
        switch (config->ChannelId)
        {
        case PWM_CHANNEL_1:
            TIM_OC1Init(timer, &channelConfig);
            TIM_OC1PreloadConfig(timer, TIM_OCPreload_Enable);
            break;
        case PWM_CHANNEL_2:
            TIM_OC2Init(timer, &channelConfig);
            TIM_OC2PreloadConfig(timer, TIM_OCPreload_Enable);
            break;
        case PWM_CHANNEL_3:
            TIM_OC3Init(timer, &channelConfig);
            TIM_OC3PreloadConfig(timer, TIM_OCPreload_Enable);
            break;
        case PWM_CHANNEL_4:
            TIM_OC4Init(timer, &channelConfig);
            TIM_OC4PreloadConfig(timer, TIM_OCPreload_Enable);
            break;
        default:
            return Pwm_SetError(PWM_ERROR_INVALID_CHANNEL);
        }
    }

    /*lưu trữ toàn bộ cấu hình vào biến tĩnh để sử dụng nội bộ file*/
    Pwm_ConfigPtr_s = ConfigPtr;
    Pwm_LastError_s = PWM_ERROR_NONE;
    return E_OK;
}
void Pwm_SetDutyCycle(Pwm_ChannelType ChannelNumber, Pwm_DutyCycleType DutyCycle)
{
    uint8 index;

    /*kiểm tra con trỏ lưu trữ cục bộ có hợp lệ*/
    if (Pwm_ConfigPtr_s == NULL_PTR)
    {
        return;
    }

    /*duyệt qua từng id kênh timer để xuất pwm signal tương ứng với cổng*/
    for (index = 0U; index < Pwm_ConfigPtr_s->CfgID_Count; index++)
    {

        /*truy xuất cấu hình pwm của kênh timer tương ứng*/
        const Pwm_ChannelConfigType_s *config = &Pwm_ConfigPtr_s->ChannelConfig[index];
        TIM_TypeDef *timer;

        /*tìm kiếm Id của timer muốn xuất pwm*/
        if (config->ChannelId != ChannelNumber)
        {
            continue; // bỏ qua khối lệnh bên dưới để tiếp tục duyệt trong for loop
        }

        /*truy xuất địa chỉ cứng của timer cần tìm*/
        timer = GetTimerGroup(config->HwTimerId);

        /*xuất pwm theo cổng tương ứng với kênh timer */
        switch (ChannelNumber)
        {
        case PWM_CHANNEL_1:
            TIM_SetCompare1(timer, DutyCycle);
            return;
        case PWM_CHANNEL_2:
            TIM_SetCompare2(timer, DutyCycle);
            return;
        case PWM_CHANNEL_3:
            TIM_SetCompare3(timer, DutyCycle);
            return;
        case PWM_CHANNEL_4:
            TIM_SetCompare4(timer, DutyCycle);
            return;
        default:
            return;
        }
    }
}