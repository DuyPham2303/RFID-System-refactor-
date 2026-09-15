#include "CallbackM.h"
#include "Gpt_Irq.h"
#include "Dio.h"

static void BlinkLed_Task(void)
{
    /* Application behavior */
    Dio_FlipChannel(DIO_CHANNEL_C13);
}

void CallbackM_Init(void)
{
    /*đăng ký hàm chức năng sẽ thực thi khi ngắt xảy*/
    Gpt_RegisterApiCallback(
        GPT_BLINKLED_GROUP1,
        BlinkLed_Task);
}