#include "Os.h"
#include "Gpt/inc/Gpt.h"

/** @brief Number of GPT ticks per millisecond */
#define GPT_DELAY_TICKS_PER_MS 10U

#ifdef OS_CFG
/**
 * @brief Id định danh cho các Task sẽ xử lý tác vụ ỳ6u cấu
 * @details mỗi task sẽ gắn với 1 hàm callback được điều phối xử lý theo Priority
 */
typedef enum Os_TaskIdc
{
    TASK_PERIODIC = 0U, /*xử lý các sự kiện ngắt timer*/
    TASK_COM,           /*xử lý các sự kiện ngắt truyền thông*/
    TASK_OS_MAX
} Os_TaskIdType;

/*bitmask cho task Periodic*/
#define MASK_UPDATE_1Ms (1 << 0u)

/*bitmask cho task Com*/
#define MASK_COM_TX (1 << 0U)
#define MASK_COM_RX (1 << 1U)
/*Kiểu dữ liệu quản lý trạng thái cơ ngắt*/
typedef uint8 eventflag;

/*API cục bộ để thao tác đặt/xóa/đọc trạng thái trạng thái cờ bit flag update*/
static void SetFlagStatus(eventflag *reg, uint8 option);
static void ResetFlagStatus(eventflag *reg, uint8 option);
static boolean ReadFlagStatus(eventflag reg, uint8 option);

typedef struct Os_Cfg
{
    void (*func)(void); /*con trỏ hàm callback*/
    eventflag Mask;
} Os_CfgTypedef;
/*bảng ánh xạ kiểu dữ liệu các cờ trạng thái và hàm callback */
static Os_CfgTypedef OsTaskCfg_Table[TASK_OS_MAX] = {
    [TASK_PERIODIC] = {.Mask = 0, .func = NULL_PTR},
    [TASK_COM] = {.Mask = 0, .func = NULL_PTR}};

static void SetFlagStatus(eventflag *reg, uint8 option)
{
    *reg |= option;
}
static void ResetFlagStatus(eventflag *reg, uint8 option)
{
    *reg &= ~option;
}
static boolean ReadFlagStatus(eventflag reg, uint8 option)
{
    return reg & option ? true : false;
}
#endif /* OS_CFG */
void Os_DelayMs(uint32 DelayMs)
{
    // Kiểm tra giá trị DelayMs có vượt quá giới hạn của bộ đếm 16-bit không
    if (DelayMs > (0xFFFFU / GPT_DELAY_TICKS_PER_MS))
    {
        return;
    }

    // Tính số lượng tick cần thiết cho khoảng thời gian DelayMs
    uint32 DelayTicks = DelayMs * GPT_DELAY_TICKS_PER_MS;

    // Lấy ID của Timer được cấu hình cho chức năng Delay
    for (uint8 index = 0; index < g_Gpt_ConfigGroup.GptCount; index++)
    {
        if (g_Gpt_ConfigGroup.ChannelConfigPtr[index].HwId == GPT_GROUP_3)
        {
            Gpt_GroupId_Type HwTimmerID = g_Gpt_ConfigGroup.ChannelConfigPtr[index].HwId;

            // Kích hoạt bộ timer
            Gpt_StartTimer(HwTimmerID, (Gpt_PeriodValue)DelayTicks);

            // Mô phỏng delay
            while (Gpt_GetTimeElapsed(HwTimmerID) < (Gpt_PeriodValue)DelayTicks)
            {
                /* Blocking demo delay */
            }

            // Vô hiệu hóa bộ timer
            Gpt_StopTimer(HwTimmerID);
            return;
        }
    }
    Gpt_GroupId_Type HwTimmerID = g_Gpt_ConfigGroup.ChannelConfigPtr[GPT_GROUP_2].HwId;

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