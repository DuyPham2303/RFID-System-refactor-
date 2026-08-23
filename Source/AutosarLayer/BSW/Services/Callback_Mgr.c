#include "Callback_Mgr.h"
#include "CallBack_Drv.h"
#include <stdio.h>
/* Application Callback API sẽ được HW gọi*/
static void BlinkLed_Task(void);                 /* IT timer 1*/
static void ControlServo_Task(void);             /* IT timer 2*/
static void AdcConvertToTemp_Task(void);         /* IT timer 3 */
static void AdcConvertToPercentBrake_Task(void); /* IT Adc 1 */
static void AdcConvertToPercentPedal_Task(void); /* IT Adc 2 */

static void BlinkLed_Task(void)
{
    printf("BlinkLed_Task: print-only sample\n");
}

static void ControlServo_Task(void)
{
    printf("ControlServo_Task: print-only sample\n");
}

static void AdcConvertToTemp_Task(void)
{
    printf("AdcConvertToTemp_Task: print-only sample\n");
}

static void AdcConvertToPercentBrake_Task(void)
{
    printf("AdcConvertToPercentBrake_Task: print-only sample\n");
}

static void AdcConvertToPercentPedal_Task(void)
{
    printf("AdcConvertToPercentPedal_Task: print-only sample\n");
}

/* App main gọi mà không cần biết cụ thể về HardWare Periph */
void Serice_Callback_Init()
{
    /* Đăng ký hàm sẽ thực thi bằng cách truyển đối số là tên hàm */
    if (Brake_RegisterCallBack(BRAKE_ADC_ID_CB, AdcConvertToPercentBrake_Task) == E_OK)
    {
        printf("Dịch vụ Callback cho Brake module đã đắng ký thành công\n");
    }
    return;
}