#include "Pwm_Cfg.h"

static const Pwm_ChannelConfigType Pwm_ChannelConfigs[] =
    {
        {.OcMode = PWM_OC_MODE_PWM1,
         .OutputState = PWM_OUTPUT_ENABLED,
         .Polarity = PWM_POLARITY_HIGH,
         .DefaultDuty = 0,
         .ChannelId = PWM_CHANNEL_CH1},
        {.OcMode = PWM_OC_MODE_PWM1,
         .OutputState = PWM_OUTPUT_ENABLED,
         .Polarity = PWM_POLARITY_HIGH,
         .DefaultDuty = 0,
         .ChannelId = PWM_CHANNEL_CH2},
        {.OcMode = PWM_OC_MODE_PWM1,
         .OutputState = PWM_OUTPUT_ENABLED,
         .Polarity = PWM_POLARITY_HIGH,
         .DefaultDuty = 0,
         .ChannelId = PWM_CHANNEL_CH3},
        {.OcMode = PWM_OC_MODE_PWM1,
         .OutputState = PWM_OUTPUT_ENABLED,
         .Polarity = PWM_POLARITY_HIGH,
         .DefaultDuty = 0,
         .ChannelId = PWM_CHANNEL_CH4}};

const Pwm_TimerConfigType g_Pwm_Config = {
    .ClkDiv = PWM_CLOCK_DIV_1,          /* No clock division */
    .ModeCntType = PWM_COUNTER_MODE_UP, /* Count up mode */
    .PresVal = 720U - 1U,               /* 0.01 ms per counter tick */
    .PeriodVal = 4999,                  /* update event every 50 ms */
    .RepCntVal = 0U,
    .HwTimerGroup = HW_TIMER_GROUP_2,
    .ChannelsArray = Pwm_ChannelConfigs,
    .ChannelCount = sizeof(Pwm_ChannelConfigs) / sizeof(Pwm_ChannelConfigs[0])};