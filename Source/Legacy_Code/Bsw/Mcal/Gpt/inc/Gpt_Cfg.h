/**
 * @file Gpt_Cfg.h
 * @brief Giao diện cấu hình kênh GPT của MCAL.
 * @details Khai báo số lượng kênh TIM và tên tượng trưng được sử dụng bởi
 *          tầng ứng dụng và tầng dịch vụ.
 * @req Mô-đun TIM của AUTOSAR MCAL; cấu hình phụ thuộc vào từng project.
 */
#ifndef GPT_CFG_H
#define GPT_CFG_H
#include "Std_Types.h"
#include "Gpt_dtypes.h"
#include "Mcu_IrqCfg.h"

#define PERIOD_UPDATE_250MS 2500 /*tick count every 0.1 ms*/

/**
 * @brief ID logic của các chức năng GPT được application sử dụng.
 * @details Application chỉ sử dụng các ID này. Ánh xạ tới timer phần cứng
 *          được thực hiện trong bảng GptConfig của MCAL.
 */
typedef enum
{
    // GPT_CFG_PWM_ID,
    GPT_CFG_DELAY_ID = 0U,
    GPT_CFG_COUNT
} Gpt_ConfigIdType;

/* Cấu trúc cấu hình tĩnh cho một kênh TIM */
typedef struct
{
    Gpt_Cmd Cmd;                        /* enable/disable ngay khi khởi tạo timer */
    Gpt_GroupType HwTimerId;            /* Số hiệu Timer vật lý của MCU (Ví dụ: TIM2, TIM3) */
    Gpt_PrescalerValue PresVal;         /* Tần số chia xác định số tick count trong 1a */
    Gpt_ClockDivType ClkDiv;            /* hệ số chia clock dùng khi cần so sánh với clock ban đầu, không làm thay đổi clock thực tế */
    Gpt_CounterModeType ModeCntType;    /* chế độ đếm up/down */
    Gpt_PeriodValue PeriodVal;          /* chu kỳ đếm tràn (thời gian để thanh ghi ARR reset) */
    Gpt_RepetitionCnt RepCntVal;        /* Advanced Timer only (TIM1 cho dòng c8t6) */
    const Mcu_IrqConfigType *IrqCfgPtr; /* con trỏ đến cấu hình NVIC */
} Gpt_ChannelConfigType_s;

/* Cấu trúc cấu hình tổng thể của module Gpt */
typedef struct
{
    const Gpt_ChannelConfigType_s *ChannelConfigPtr; /* con trỏ lưu trữ mảng cấu hình theo ID logic */
    uint8 GptCount;                                  /* số entry GPT được cấu hình */
} Gpt_ConfigType_s;

extern const Gpt_ConfigType_s Gpt_Config;
#endif /* GPT_CFG_H */