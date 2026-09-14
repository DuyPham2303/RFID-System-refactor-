#include "Pwm_Cfg.h"

static const Pwm_ChannelConfigType_s Pwm_ChannelConfigs[PWM_CFG_COUNT] =
    {
        // [PWM_CFG_SERVO_ID] =
        //     {
        //         .ChannelId = PWM_CHANNEL_1,
        //         .HwTimerId = GPT_GROUP_2,
        //         .OcMode = PWM_OC_MODE_PWM1,
        //         .OutputState = PWM_OUTPUT_ENABLED,
        //         .PulseValue = 1000,
        //         .Polarity = PWM_POLARITY_HIGH},
        [PWM_CFG_CH1_ID] =
            {
                .ChannelId = PWM_CHANNEL_1,
                .HwTimerId = GPT_GROUP_2,
                .OcMode = PWM_OC_MODE_PWM1,
                .OutputState = PWM_OUTPUT_ENABLED,
                .PulseValue = 0,
                .Polarity = PWM_POLARITY_HIGH},
        [PWM_CFG_CH2_ID] =
            {
                .ChannelId = PWM_CHANNEL_2,
                .HwTimerId = GPT_GROUP_2,
                .OcMode = PWM_OC_MODE_PWM1,
                .OutputState = PWM_OUTPUT_ENABLED,
                .PulseValue = 0,
                .Polarity = PWM_POLARITY_HIGH},
        [PWM_CFG_CH3_ID] =
            {
                .ChannelId = PWM_CHANNEL_3,
                .HwTimerId = GPT_GROUP_2,
                .OcMode = PWM_OC_MODE_PWM1,
                .OutputState = PWM_OUTPUT_ENABLED,
                .PulseValue = 0,
                .Polarity = PWM_POLARITY_HIGH},
        [PWM_CFG_CH4_ID] =
            {
                .ChannelId = PWM_CHANNEL_4,
                .HwTimerId = GPT_GROUP_2,
                .OcMode = PWM_OC_MODE_PWM1,
                .OutputState = PWM_OUTPUT_ENABLED,
                .PulseValue = 0,
                .Polarity = PWM_POLARITY_HIGH}

};
;
const Pwm_ConfigType_s Pwm_Config = {
    .ChannelConfig = Pwm_ChannelConfigs,
    .CfgID_Count = sizeof(Pwm_ChannelConfigs) / sizeof(Pwm_ChannelConfigs[0])};