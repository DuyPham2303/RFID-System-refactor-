#include "Gpt_Irq.h"
#include "stm32f10x_tim.h"

// Mảng lưu con trỏ hàm callback cho từng kênh GPT (được gán khi gọi Gpt_RegisterApiCallback)
static Gpt_ApiCallbackPtr Gpt_CallbackMapTable[GPT_MAX_GROUP] = {NULL_PTR}; /*mặc định chưa lưu địa chỉ bất cứ hàm nào*/

// Hàm đăng ký do người dùng gọi lúc khởi tạo hệ thống
Std_ReturnType Gpt_RegisterApiCallback(Gpt_CbServiceType group, Gpt_ApiCallbackPtr Cb)
{
    /*giới hạn phạm vi số lượng nhóm phần cứng hợp lệ*/
    if (group < GPT_MAX_GROUP)
    {
        Gpt_CallbackMapTable[group] = Cb; /*gán địa chỉ ánh xạ cho Id của phần cứng tương ứng*/
        return E_OK;
    }
    return E_NOT_OK;
}

// Hàm ISR được CPU tự động gọi khi có ngắt
void TIM2_IRQHandler(void)
{
    /*đợi trigger event*/
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {

        /*kiểm tra hàm Callback tương ứng đã đăng ký chưa*/
        if (Gpt_CallbackMapTable[GPT_BLINKLED_GROUP1] != NULL_PTR)
        {
            Gpt_CallbackMapTable[GPT_BLINKLED_GROUP1](); // gọi hàm để thực thi
        }
    }
    /*xóa cơ ngắt để reset*/
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}