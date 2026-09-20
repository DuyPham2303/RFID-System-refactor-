#include "Pwm_mapping.h"
#include "stm32f10x_tim.h"
/********************************************************
 * @brief Lấy chế độ Output Compare cho PWM~
 ********************************************************/
uint16 GetPwmOcMode(Pwm_OcModeType ocMode)
{
    switch (ocMode)
    {
    case PWM_OC_MODE_TIMING:
        return TIM_OCMode_Timing;
    case PWM_OC_MODE_ACTIVE:
        return TIM_OCMode_Active;
    case PWM_OC_MODE_INACTIVE:
        return TIM_OCMode_Inactive;
    case PWM_OC_MODE_TOGGLE:
        return TIM_OCMode_Toggle;
    case PWM_OC_MODE_PWM1:
        return TIM_OCMode_PWM1;
    case PWM_OC_MODE_PWM2:
        return TIM_OCMode_PWM2;
    default:
        return TIM_OCMode_Timing;
    }
}

/********************************************************
 * @brief Lấy trạng thái ngõ ra chính cho PWM
 ********************************************************/
uint16 GetPwmOutputState(Pwm_OutputStateType outputState)
{
    switch (outputState)
    {
    case PWM_OUTPUT_DISABLED:
        return TIM_OutputState_Disable;
    case PWM_OUTPUT_ENABLED:
        return TIM_OutputState_Enable;
    default:
        return TIM_OutputState_Disable;
    }
}

/********************************************************
 * @brief Lấy giá trị xung (Pulse) cho PWM
 ********************************************************/
uint16 GetPwmPulseValue(Pwm_Pulse pulse)
{
    return pulse;
}

/********************************************************
 * @brief Lấy cực tính ngõ ra chính cho PWM
 ********************************************************/
uint16 GetPwmOcPolarity(Pwm_PolarityType polarity)
{
    switch (polarity)
    {
    case PWM_POLARITY_HIGH:
        return TIM_OCPolarity_High;
    case PWM_POLARITY_LOW:
        return TIM_OCPolarity_Low;
    default:
        return TIM_OCPolarity_High;
    }
}

/********************************************************
 * @brief Lấy trạng thái ngõ ra bổ sung (Complementary) cho PWM
 ********************************************************/
uint16 GetPwmOutputNState(Pwm_OutputNStateType outputNState)
{
    switch (outputNState)
    {
    case PWM_OUTPUT_N_DISABLED:
        return TIM_OutputNState_Disable;
    case PWM_OUTPUT_N_ENABLED:
        return TIM_OutputNState_Enable;
    default:
        return TIM_OutputNState_Disable;
    }
}

/********************************************************
 * @brief Lấy cực tính ngõ ra bổ sung (Complementary) cho PWM
 ********************************************************/
uint16 GetPwmOcNPolarity(Pwm_PolarityNType polarityN)
{
    switch (polarityN)
    {
    case PWM_POLARITY_N_HIGH:
        return TIM_OCNPolarity_High;
    case PWM_POLARITY_N_LOW:
        return TIM_OCNPolarity_Low;
    default:
        return TIM_OCNPolarity_High;
    }
}

/********************************************************
 * @brief Lấy trạng thái Idle của ngõ ra chính
 ********************************************************/
uint16 GetPwmOcIdleState(Pwm_IdleStateType idleState)
{
    switch (idleState)
    {
    case PWM_IDLE_SET:
        return TIM_OCIdleState_Set;
    case PWM_IDLE_RESET:
        return TIM_OCIdleState_Reset;
    default:
        return TIM_OCIdleState_Reset;
    }
}

/********************************************************
 * @brief Lấy trạng thái Idle của ngõ ra bổ sung
 ********************************************************/
uint16 GetPwmOcNIdleState(Pwm_IdleStateNType idleStateN)
{
    switch (idleStateN)
    {
    case PWM_IDLE_N_SET:
        return TIM_OCNIdleState_Set;
    case PWM_IDLE_N_RESET:
        return TIM_OCNIdleState_Reset;
    default:
        return TIM_OCNIdleState_Reset;
    }
}
