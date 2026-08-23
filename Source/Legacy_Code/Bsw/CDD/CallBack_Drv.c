#include "CallBack_Drv.h"

#define MAX_INSTANCE_ADC 2

/* mảng chứa các con trỏ hàm tương ứng với số lượng các bộ adc có sẵn*/
static void (*adc_pCb[MAX_INSTANCE_ADC])(void) = {NULL_PTR, NULL_PTR};
static void (*timer_pCb[MAX_INSTANCE_ADC])(void) = {NULL_PTR, NULL_PTR, NULL_PTR};

/* Publish API --> Service sử dụng */
Std_ReturnType Brake_RegisterCallBack(uint8 instance_id, CallBack pCb)
{
    if (pCb != NULL_PTR)
    {
        // kiểm tra id có trong ngưỡng
        if (instance_id <= MAX_INSTANCE_ADC)
        {
            adc_pCb[instance_id] = pCb; /* ánh xạ đến hàm xử lý tương ứng dựa trên macro ID*/
        }
        printf("CallBack_Drv đã đăng ký thành công hàm callback cho Brake\n");
        return E_OK;
    }
    else
        return E_NOT_OK;
}

Std_ReturnType Dht11_RegisterCallBack(uint8 instance_id, CallBack pCb)
{
    if (pCb != NULL_PTR)
    {
        if (instance_id <= MAX_INSTANCE_ADC)
        {
            adc_pCb[instance_id] = pCb; /* ánh xạ đến hàm xử lý tương ứng dựa trên macro ID*/
        }
        return E_OK;
    }
    else
        return E_NOT_OK;
}

/* Private API --> tự động thực thi khi có sự kiện ngắt xảy ra */
void ADC1_2_IRQHandler()
{
    // 1.xóa cờ ngắt
    // 2.gọi callbaCK tương ứng đã đăng ký (nếu có)
    if (adc_pCb[BRAKE_ADC_ID_CB] != NULL_PTR)
        adc_pCb[BRAKE_ADC_ID_CB]();
    else if (adc_pCb[PEDAL_ADC_ID_CB] != NULL_PTR)
        adc_pCb[PEDAL_ADC_ID_CB]();
    else
        return;
}
void TIM1_UP_IRQHandler()
{
    if (timer_pCb[DHT11_TIMER_ID_CB] != NULL_PTR)
        timer_pCb[DHT11_TIMER_ID_CB]();
    else
        return;
}
void TIM2_IRQHandler()
{
    if (timer_pCb[SERVO_TIMER_ID_CB] != NULL_PTR)
        timer_pCb[SERVO_TIMER_ID_CB]();
    else
        return;
}
void TIM3_IRQHandler()
{
    if (timer_pCb[LED_TIMER_ID_CB] != NULL_PTR)
        timer_pCb[LED_TIMER_ID_CB]();
    else
        return;
}
