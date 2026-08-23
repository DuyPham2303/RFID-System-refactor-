/**
 * @file IoHwAb_Adc_Cfg.h
 * @brief Configuration interface for ADC IoHwAb channels.
 * @details Provides board-independent configuration points for analog signals
 *          while hiding MCU ADC channel assignments from upper layers.
 * @req AUTOSAR ECU Abstraction Layer configuration concept.
 */
#ifndef IOHWAB_ADC_CFG_H
#define IOHWAB_ADC_CFG_H

#include "Std_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /* =====================================================================
     * 1) Định danh các kênh ADC ở mức Ứng dụng
     * ===================================================================== */
    typedef enum
    {
        IOHWAB_ADC_CH_SYSTEM_VOLTAGE = 0, /* Đo điện áp nguồn nuôi hệ thống */
        IOHWAB_ADC_CHANNEL_COUNT
    } IoHwAb_AdcChannelType;

    /* =====================================================================
     * 2) Cấu trúc tham số cấu hình cho kênh ADC
     * ===================================================================== */
    typedef struct
    {
        IoHwAb_AdcChannelType ChannelId;
        uint8 AdcHardwareUnit; /* Chọn cụm phần cứng ADC1, ADC2... */
        uint8 AdcGroupChannel; /* Kênh vật lý tương ứng (ví dụ: Channel 0 tương ứng PA0) */
    } IoHwAb_AdcConfigType;

    /* Khai báo bảng cấu hình ADC */
    extern const IoHwAb_AdcConfigType IoHwAb_AdcConfigList[IOHWAB_ADC_CHANNEL_COUNT];

#ifdef __cplusplus
}
#endif

#endif /* IOHWAB_ADC_CFG_H */