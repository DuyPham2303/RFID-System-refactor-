#include "Bsw_Delay.h"
#include "Gpt/inc/Gpt.h"
#include "Gpt/inc/Gpt_Cfg.h"

/** @brief Number of GPT ticks per millisecond */
#define GPT_DELAY_TICKS_PER_MS 10U

void Bsw_DelayMs(uint32 DelayMs)
{
    // Kiểm tra giá trị DelayMs có vượt quá giới hạn của bộ đếm 16-bit không
    if (DelayMs > (0xFFFFU / GPT_DELAY_TICKS_PER_MS))
    {
        return;
    }

    // Tính số lượng tick cần thiết cho khoảng thời gian DelayMs
    uint32 DelayTicks = DelayMs * GPT_DELAY_TICKS_PER_MS;

    // Lấy ID của Timer được cấu hình cho chức năng Delay
    Gpt_GroupType HwTimmerID = Gpt_Config.ChannelConfigPtr[GPT_CFG_DELAY_ID].HwTimerId;

    // kích hoạt bộ timer
    Gpt_StartTimer(HwTimmerID, (Gpt_PeriodValue)DelayTicks);

    /*mô phỏng delay*/
    while (Gpt_GetTimeElapsed(HwTimmerID) < (Gpt_PeriodValue)DelayTicks)
    {
        /* Blocking demo delay */
    }

    // vô hiệu hóa bộ timer
    Gpt_StopTimer(HwTimmerID);
}