/**
 * @file Gpt_Cfg.c
 * @brief Định cấu hình cho mô-đun Timer MCAL.
 * @details Cấu hình các tham số Timer cho các kênh khác nhau
 *          dựa trên yêu cầu của ứng dụng.
 */

#include "Gpt_Cfg.h"

/* Timer input clock is 72 MHz; this prescaler produces a 100 us counter tick. */
#define GPT_PRESCALER_100US_TICK (7200U - 1U)
#define GPT_PERIOD_100MS (1000U - 1U)
#define GPT_PERIOD_1S (10000U - 1U)

/*Callback API được ISR xử lý*/
volatile uint8 g_Flagupdate_Periodic = FALSE;

static void Gpt_NotiHandler()
{
    g_Flagupdate_Periodic = TRUE;
}

/////////////////////*** ĐỐI TƯỢNG CẤU HÌNH TĨNH CỤC BỘ***/////////////////////

/**
 * @brief Bảng cấu hình các thông số cần thiết cho xử lý calback API
 */
static const Gpt_CallbackConfigType s_callbackCfg = {
    .Noti = Gpt_NotiHandler,
    .flag = GPT_IRQ_SOURCE_UPDATE};

/**
 * @brief bảng cấu hình thông số NVIC và IRQ priority,channel
 */
static const Mcu_NvicConfigType_s s_NvicCfg = {
    .IrqChannel = MCU_GPT_GROUP1_IRQ,
    .PreemptionPriority = 0,
    .SubPriority = 0};

/**
 * @brief bảng cấu hình tổng thể thông số cho tất cả phần cứng Timer
 */
static const Gpt_ChannelConfigType_s Gpt_ChannelConfigArray[] = {
    // {.HwId = GPT_GROUP_1,
    //  .PresVal = GPT_PRESCALER_100US_TICK,
    //  .ClkDiv = GPT_CLOCK_DIV_1,          /* No clock division */
    //  .ModeCntType = GPT_COUNTER_MODE_UP, /* Count up mode */
    //  .PeriodVal = GPT_PERIOD_100MS,      /* 10000 ticks x 100 us = 1 s */
    //  .RepCntVal = 0U,                    /* No repetition counter */
    //  .Cmd = true,
    //  .NvicCfgPtr = &s_NvicCfg,
    //  .CallbackCfgPtr = &s_callbackCfg},
    {.HwId = GPT_GROUP_3,
     .PresVal = GPT_PRESCALER_100US_TICK,
     .ClkDiv = GPT_CLOCK_DIV_1,          /* No clock division */
     .ModeCntType = GPT_COUNTER_MODE_UP, /* Count up mode */
     .PeriodVal = GPT_PERIOD_100MS,      /* 1000 ticks x 100 us = 100 ms */
     .RepCntVal = 0U,                    /* No repetition counter */
     .Cmd = false,
     .NvicCfgPtr = NULL_PTR,
     .CallbackCfgPtr = NULL_PTR}};
/////////////////////*** ĐỐI TƯỢNG CẤU HÌNH TĨNH TOÀN CỤC***/////////////////////

/**
 * @brief đối tượng lưu trữ mảng cấu hình của tất cả phần cứng timer
 */
const Gpt_ConfigType_s g_Gpt_ConfigGroup = {
    .ChannelConfigPtr = Gpt_ChannelConfigArray,
    .GptCount = sizeof(Gpt_ChannelConfigArray) / sizeof(Gpt_ChannelConfigArray[0])};
