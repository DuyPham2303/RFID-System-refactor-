/**
 * @file Adc_Config.h
 * @brief Định nghĩa các kiểu dữ liệu cấu hình cho mô-đun ADC của MCAL.
 * @details Khai báo cấu trúc cấu hình nhóm và cấu hình mô-đun được sử dụng
 *          bởi dịch vụ khởi tạo ADC.
 * @req Khái niệm cấu hình ADC của AUTOSAR MCAL và phần cấu hình riêng của dự án.
 */
#ifndef ADC_CONFIG_H
#define ADC_CONFIG_H
#include "Adc_Types.h"
#define MAX_ADC_CHANNEL_F103 16
/********************************************************
 * @Typedef Adc_GroupDefType
 * @brief kieu du lieu cau hinh tung nhom adc
 * @detail duoc su dung de cau hinh cho tung nhom chuyen doi
 *****************/
typedef struct
{
	Adc_ChannelType listChannel[MAX_ADC_CHANNEL_F103];
	Adc_TriggerSourceType TriggerSourceType;
	Adc_HwTriggerSignalType TriggerSourceHw;
	Adc_GroupConvModeType ConvMode;
	Adc_GroupType GroupType;
	Adc_GroupPriorityType Rank;
} Adc_GroupDefType;

/********************************************************
 * @Typedef Adc_ConfigType
 * @brief kieu du lieu chua cac cau hinh chung cho toan bo module ADC
 * @detail duoc su dung de cau hinh cho bo ADC
 *****************/
typedef struct
{
	// cau hinh cac thong so
	Adc_ConversionTimeType ConvTimeType;
	Adc_ResultAlignmentType AlignmentType;
	Adc_SamplingTimeType SampTime;
	Adc_PrescaleType prescaler;
	bool IsInjected;
	// cau hinh kenh va so kenh
	Adc_GroupDefType *GroupConfig;
} Adc_ConfigType;

typedef struct
{
	Adc_ValueGroupType *resultBuff;
	bool IsConfig;
} Adc_GroupConfigResultBuff;

#endif