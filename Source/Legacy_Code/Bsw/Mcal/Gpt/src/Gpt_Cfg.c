/**
 * @file Gpt_Cfg.c
 * @brief Định cấu hình cho mô-đun Timer MCAL.
 * @details Cấu hình các tham số Timer cho các kênh khác nhau
 *          dựa trên yêu cầu của ứng dụng.
 */

#include "Gpt_Cfg.h"

static const Mcu_IrqConfigType IrqCfgPtr_s = {
    .Enable = 1,
    .IrqChannel = MCU_GPT_CH1_IRQ,
    .PreemptionPriority = 0,
    .SubPriority = 0};
static const Gpt_ChannelConfigType_s Gpt_ChannelConfigArray[GPT_CFG_COUNT] = {
    [GPT_CFG_DELAY_ID] = {
        .HwTimerId = GPT_GROUP_1,           /* Use TIM2 */
        .PresVal = 7200U - 1U,              /* 0.1 ms per counter tick */
        .ClkDiv = GPT_CLOCK_DIV_1,          /* No clock division */
        .ModeCntType = GPT_COUNTER_MODE_UP, /* Count up mode */
        .PeriodVal = 9999,                  /* update event every 4999 * 0.1ms = 0.5s */
        .RepCntVal = 0U,                    /* No repetition counter */
        .Cmd = GPT_ENABLE,
        .IrqCfgPtr = &IrqCfgPtr_s}};
const Gpt_ConfigType_s Gpt_Config = {
    .ChannelConfigPtr = Gpt_ChannelConfigArray,
    .GptCount = sizeof(Gpt_ChannelConfigArray) / sizeof(Gpt_ChannelConfigArray[0])};
