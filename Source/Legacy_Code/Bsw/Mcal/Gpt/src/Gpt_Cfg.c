/**
 * @file Gpt_Cfg.c
 * @brief Định cấu hình cho mô-đun Timer MCAL.
 * @details Cấu hình các tham số Timer cho các kênh khác nhau
 *          dựa trên yêu cầu của ứng dụng.
 */

#include "Gpt_Cfg.h"
/********************************************************
 * @brief Callback notification function for LED Blink Timer
 * @details This function is called when the timer overflows
 ********************************************************/
static void TimerLedBlinkCallback(void)
{
    /* LED Blink handler - Toggle LED here */
    /* Example: LED_Toggle(); */
}

/********************************************************
 * @brief Global Timer module configuration
 * @details Main configuration structure for Gpt module
 *          Points to the channel configurations array
 ********************************************************/
static const Gpt_ChannelConfigType_s Gpt_ChannelConfigArray[GPT_CFG_COUNT] = {
    // [GPT_CFG_LED_ID] = {
    //     .HwTimerId = GPT_GROUP_1,             /* Use TIM2 */
    //     .PresVal = 36000U,                    /* Prescaler: 36MHz / 36000 = 1kHz (1ms tick) */
    //     .ClkDiv = GPT_CLOCK_DIV_1,            /* No clock division */
    //     .ModeCntType = GPT_COUNTER_MODE_UP,   /* Count up mode */
    //     .PeriodVal = 500U,                    /* Period: 500ms (500 * 1ms) */
    //     .RepCntVal = 0U,                      /* No repetition counter */
    //     .Notification = TimerLedBlinkCallback /* Callback for LED Blink */
    // },
    // [GPT_CFG_SERVO_ID] = {
    //     .HwTimerId = GPT_GROUP_1,           /* Use TIM2 */
    //     .PresVal = 36000U,                  /* Prescaler: 36MHz / 36000 = 1kHz (1ms tick) */
    //     .ClkDiv = GPT_CLOCK_DIV_1,          /* No clock division */
    //     .ModeCntType = GPT_COUNTER_MODE_UP, /* Count up mode */
    //     .PeriodVal = 2000U,                 /* Period: 2000ms (2s) */
    //     .RepCntVal = 0U,                    /* No repetition counter */
    //     .Notification = NULL_PTR            /* No callback for Servo Timer */
    // },
    [GPT_CFG_DELAY_ID] = {
        .HwTimerId = GPT_GROUP_1,           /* Use TIM2 */
        .PresVal = 7200U - 1U,              /* 0.1 ms per counter tick */
        .ClkDiv = GPT_CLOCK_DIV_1,          /* No clock division */
        .ModeCntType = GPT_COUNTER_MODE_UP, /* Count up mode */
        .PeriodVal = 0xFFFFU,               /* Blocking delay counter range */
        .RepCntVal = 0U,                    /* No repetition counter */
        .Cmd = GPT_DISABLE,
        .Notification = NULL_PTR /* Blocking service does not notify */
    },
    [GPT_CFG_PWM_ID] = {
        .HwTimerId = GPT_GROUP_2,           /* Use TIM3 */
        .PresVal = 7200U - 1U,              /* 0.1 ms per counter tick */
        .ClkDiv = GPT_CLOCK_DIV_1,          /* No clock division */
        .ModeCntType = GPT_COUNTER_MODE_UP, /* Count up mode */
        .PeriodVal = PERIOD_UPDATE_250MS,   /* counter period in ARR register  */
        .RepCntVal = 0U,                    /* No repetition counter */
        .Cmd = GPT_ENABLE,
        .Notification = NULL_PTR /* Blocking service does not notify */
    }};
const Gpt_ConfigType_s Gpt_Config = {
    .ChannelConfigPtr = Gpt_ChannelConfigArray,
    .GptCount = sizeof(Gpt_ChannelConfigArray) / sizeof(Gpt_ChannelConfigArray[0])};
