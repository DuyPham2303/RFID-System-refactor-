#include "Gpt_Irq.h"
#include "Gpt_Mapping.h"
#include "Dio.h"

static Gpt_IrqConfigNotiType Gpt_MapNotiTable[GPT_MAX_GROUP][GPT_MAX_NOTI] = {
    [GPT_GROUP_1] = {
        [GPT_UPDATE_1S] = {.cb = NULL_PTR, .flag = 0U}}};

// Hàm đăng ký do người dùng gọi lúc khởi tạo hệ thống
Std_ReturnType Gpt_RegisterNotification(
    Gpt_GroupId_Type group,
    Gpt_IdnotiType CbId,
    Gpt_notificationPtr Cb)
{
    /*giới hạn phạm vi số lượng nhóm phần cứng hợp lệ*/
    if (group < GPT_MAX_GROUP)
    {
        Gpt_MapNotiTable[group][CbId].cb = Cb; /*gán địa chỉ ánh xạ cho Id của phần cứng tương ứng*/
        return E_OK;
    }
    return E_NOT_OK;
}

Std_ReturnType Gpt_SetNotificationEnable(
    Gpt_GroupId_Type HwId,
    Gpt_IrqSourceType flag,
    bool cmd)
{
    if (flag >= GPT_IRQ_SOURCE_MAX)
    {
        return E_NOT_OK;
    }
    TIM_TypeDef *Gptx = GetTimerGroup(HwId);
    uint16 It_flag = Gpt_MapToHardwareItFlag(flag);
    // TIM_ITConfig(Gptx, It_flag, cmd);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    return E_OK;
}

// Hàm ISR được CPU tự động gọi khi có ngắt
void TIM2_IRQHandler(void)
{
    /*đợi trigger event*/
    if (TIM_GetITStatus(TIM2, Gpt_MapToHardwareItFlag(Gpt_MapNotiTable[GPT_GROUP_1][GPT_UPDATE_1S].flag)) != RESET)
    {
        /*kiểm tra hàm Callback tương ứng đã đăng ký chưa*/
        if (Gpt_MapNotiTable[GPT_GROUP_1][GPT_UPDATE_1S].cb != NULL_PTR)
        {
            Gpt_MapNotiTable[GPT_GROUP_1][GPT_UPDATE_1S].cb(); // gọi hàm để thực thi
        }
    }
    /*xóa cơ ngắt để reset*/
    TIM_ClearITPendingBit(TIM2, Gpt_MapToHardwareItFlag(Gpt_MapNotiTable[GPT_GROUP_1][GPT_UPDATE_1S].flag));
}