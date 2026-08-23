/**
 * @file Adc_Cfg.h
 * @brief Configuration interface for the MCAL ADC module.
 * @details Defines the configuration point for ADC groups, channels, and
 *          conversion parameters used during ECU initialization.
 * @req AUTOSAR MCAL ADC configuration concept.
 */
#ifndef ADC_CFG_H
#define ADC_CFG_H

// LÝ THUYẾT - GỢI Ý CODE (cfg/mcal/Adc_Cfg.h)
// Định nghĩa các hằng số cấu hình
#define ADC_RESOLUTION_12BIT (12U)
#define ADC_ALIGNMENT_RIGHT (0U)

// Cấu trúc dữ liệu chứa tham số cấu hình
typedef struct
{
    uint8 Resolution;
    uint8 Alignment;
    uint32 SampleTime;
} Adc_ConfigType;

// Khai báo extern để EcuM hoặc Main có thể nhìn thấy biến cấu hình này
extern const Adc_ConfigType AdcConfig_Default;

// vị trí : (cfg/mcal/Adc_PBcfg.c)
// Gắn giá trị thực tế cho biến cấu hình
const Adc_ConfigType AdcConfig_Default = {
    .Resolution = ADC_RESOLUTION_12BIT,
    .Alignment = ADC_ALIGNMENT_RIGHT,
    .SampleTime = 239 // Ví dụ: 239.5 cycles
};

// ví trị : Bsw/Mcal/Adc.c
#include "Adc.h"
#include "Adc_Cfg.h" // Nhúng file cấu hình

void Adc_Init(const Adc_ConfigType *ConfigPtr)
{
    if (ConfigPtr == NULL_PTR)
        return; // Bảo vệ con trỏ

    // Đọc thông số từ ConfigPtr để cấu hình thanh ghi STM32
    // Ví dụ:
    // Nếu (ConfigPtr->Resolution == ADC_RESOLUTION_12BIT)
    //      -> Ghi bit vào thanh ghi ADC_CR1

    // Cấu hình SampleTime, Alignment dựa theo biến con trỏ truyền vào
}
#endif
