/**
 * @file Gpt_Cfg.c
 * @brief Định cấu hình cho mô-đun Timer MCAL.
 * @details Cấu hình các tham số Timer cho các kênh khác nhau
 *          dựa trên yêu cầu của ứng dụng.
 */

#include "Gpt_Cfg.h"
#include "stm32f10x.h"
#include "Dio.h"

/*Callback API được ISR xử lý*/
volatile uint8 g_Flagupdate_Periodic = FALSE;
static void Gpt_Callback()
{
    g_Flagupdate_Periodic = TRUE;
}

/////////////////////*** ĐỐI TƯỢNG CẤU HÌNH TĨNH CỤC BỘ***/////////////////////
/*sử dụng nội bộ module mcal --> extern Gpt_Irq.h*/
static const Gpt_IrqConfigNotiType Gpt_NotiMapCfg[GPT_MAX_NOTI] =
    {
        [GPT_UPDATE_1S] = {.cb = Gpt_Callback, .flag = GPT_IRQ_SOURCE_UPDATE}};

static const Mcu_IrqConfigType IrqCfgPtr_s = {
    .cmd = true,
    .IrqChannel = MCU_GPT_CH1_IRQ,
    .PreemptionPriority = 0,
    .SubPriority = 0};
static const Gpt_ChannelConfigType_s Gpt_ChannelConfigArray[GPT_CFG_COUNT] = {
    [GPT_CFG_TIM2_ID] =
        {
            .HwTimerId = GPT_GROUP_1,           /* Use TIM2 */
            .PresVal = 7200U - 1U,              /* 0.1 ms per counter tick */
            .ClkDiv = GPT_CLOCK_DIV_1,          /* No clock division */
            .ModeCntType = GPT_COUNTER_MODE_UP, /* Count up mode */
            .PeriodVal = 1999,                  /* update event every 4999 * 0.1ms = 0.5s */
            .RepCntVal = 0U,                    /* No repetition counter */
            .Cmd = true,
            .IrqCfgPtr = &IrqCfgPtr_s,
            .notiPtr = Gpt_NotiMapCfg},
    [GPT_CFG_TIM3_ID] =
        {.HwTimerId = GPT_GROUP_2,           /* Use TIM3 */
         .PresVal = 7200U - 1U,              /* 0.1 ms per counter tick */
         .ClkDiv = GPT_CLOCK_DIV_1,          /* No clock division */
         .ModeCntType = GPT_COUNTER_MODE_UP, /* Count up mode */
         .PeriodVal = 9999,                  /* update event every 4999 * 0.1ms = 0.5s */
         .RepCntVal = 0U,                    /* No repetition counter */
         .Cmd = false,
         .IrqCfgPtr = NULL_PTR,
         .notiPtr = NULL_PTR}};

/////////////////////*** ĐỐI TƯỢNG CẤU HÌNH TĨNH TOÀN CỤC***/////////////////////

/*Publish cho App để cấu hình phần cứng --> extern Gpt_Cfg.h  */
const Gpt_ConfigType_s Gpt_Config = {
    .ChannelConfigPtr = Gpt_ChannelConfigArray,
    .GptCount = sizeof(Gpt_ChannelConfigArray) / sizeof(Gpt_ChannelConfigArray[0])};
