/**
 * @file Pwm_Cfg.h
 * @brief Giao diện cấu hình trình điều khiển PWM của MCAL.
 * @details Khai báo số lượng kênh PWM, tên tượng trưng và cấu hình
 *          được sử dụng bởi tầng ứng dụng và tầng dịch vụ.
 * @req Mô-đun PWM của AUTOSAR MCAL; cấu hình phụ thuộc vào từng project.
 */
#ifndef PWM_CFG_H
#define PWM_CFG_H

#include "Pwm_Types.h"

/* 1. Cấu hình phần NGỌN (Riêng cho từng Kênh) */
typedef struct
{
    Pwm_ChannelType ChannelId;       /* ID kênh (CH1, CH2...) */
    Pwm_OcModeType OcMode;           /* Khác nhau giữa các kênh */
    Pwm_OutputStateType OutputState; /* Khác nhau giữa các kênh */
    Pwm_PolarityType Polarity;       /* Khác nhau giữa các kênh */
    Pwm_PeriodValue DefaultDuty;     /* Giá trị xung CCRx ban đầu */
} Pwm_ChannelConfigType;

/* 2. Cấu hình phần NỀN (Chung cho cả bộ Timer) */
typedef struct
{
    Hw_TimerGroupIdType HwTimerGroup; /* Định danh bộ Timer (TIM1, TIM2...) */
    Pwm_PrescalerValue PresVal;       /* Chung cho các kênh */
    Pwm_ClockDivType ClkDiv;          /* Chung cho các kênh */
    Pwm_CounterModeType ModeCntType;  /* Chung cho các kênh */
    Pwm_PeriodValue PeriodVal;        /* Chung cho các kênh */
    Pwm_RepetitionCnt RepCntVal;      /* Chung cho các kênh */

    /* Con trỏ trỏ tới danh sách các kênh thuộc bộ Timer này */
    const Pwm_ChannelConfigType *ChannelsArray;
    uint8 ChannelCount;
} Pwm_TimerConfigType;

extern const Pwm_TimerConfigType g_Pwm_Config;
#endif /* PWM_CFG_H */
