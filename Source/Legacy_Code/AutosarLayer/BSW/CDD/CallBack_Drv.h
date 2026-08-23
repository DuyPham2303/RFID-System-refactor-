/**
 * @file CallBack_Drv.h
 * @brief Callback registration interface for CDD interrupt services.
 * @details Provides callback types, service identifiers, and registration APIs
 *          used by the interrupt-driven CDD layer.
 * @req AUTOSAR BSW service interface; implementation is hardware dependent.
 */
#ifndef CALLBACK_DRV_H
#define CALLBACK_DRV_H
#include "StdTypes.h"

/* ID định danh để xử lý logic lựa chọn hàm đăng ký : publish --> module serice  */
#define BRAKE_ADC_ID_CB 0
#define PEDAL_ADC_ID_CB 1

#define DHT11_TIMER_ID_CB 0
#define SERVO_TIMER_ID_CB 1
#define LED_TIMER_ID_CB 2

/* định nghĩa kiểu dữ liệu con trỏ hàm */
typedef void (*CallBack)(void);
/* API đăng ký xử lý task cụ thể mỗi khi có 1 ngắt tương ứng xảy ra  */
Std_ReturnType Brake_RegisterCallBack(uint8_t id, CallBack pCb);
Std_ReturnType Pedal_RegisterCallBack(uint8_t id, CallBack pCb);
Std_ReturnType Dht11_RegisterCallBack(uint8_t id, CallBack pCb);
Std_ReturnType Servo_RegisterCallBack(uint8_t id, CallBack pCb);
Std_ReturnType Timer_RegisterCallBack(uint8_t id, CallBack pCb);

#endif