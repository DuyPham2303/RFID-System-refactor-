#include "IoHwAb_Pwm_Cfg.h"

/* Bảng ánh xạ chi tiết giữa kênh logic và tài nguyên phần cứng */
const IoHwAb_PwmChannelConfigType IoHwAb_Pwm_Channels[IOHWAB_MAX_LOGIC_CHANNELS] = {
    {
        .LogicChannelId = IOHWAB_LED_GREEN_PWM, /* ID kênh logic của tầng ứng dụng */
        .McalChannelId = PWM_CHANNEL_CH1,       /* Map vào PwmChannel_1 của MCAL */
        .SafeStateDuty = 0                      /* Trạng thái an toàn: Dừng động cơ */
    },
    {.LogicChannelId = IOHWAB_LED_BLUE_PWM,
     .McalChannelId = PWM_CHANNEL_CH2,
     .SafeStateDuty = 0},
    {.LogicChannelId = IOHWAB_LED_YELLOW_PWM,
     .McalChannelId = PWM_CHANNEL_CH3,
     .SafeStateDuty = 0},
    {.LogicChannelId = IOHWAB_LED_RED_PWM,
     .McalChannelId = PWM_CHANNEL_CH4,
     .SafeStateDuty = 0}};
/* Cấu trúc tổng thể được truyền vào hàm Init */
const IoHwAb_PwmConfigType g_IoHwAb_Pwm_Config = {
    .ChannelConfigPtr = IoHwAb_Pwm_Channels,
    .CfgID_Count = sizeof(IoHwAb_Pwm_Channels) / sizeof(IoHwAb_Pwm_Channels[0])};
