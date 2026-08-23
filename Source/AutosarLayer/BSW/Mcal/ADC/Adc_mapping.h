/**
 * @file Adc_mapping.h
 * @brief MCU mapping helpers for the MCAL ADC implementation.
 * @details Declares conversions between project ADC configuration values and
 *          STM32 peripheral-library values.
 * @req AUTOSAR MCAL ADC implementation support; not an application API.
 */
#ifndef ADC_MAPPING_H
#define ADC_MAPPING_H
#include "stm32f10x_adc.h" // Keil::Device:StdPeriph Drivers:ADC
#include "Adc_Types.h"

inline ADC_TypeDef *GetGroupAdc(Adc_GroupType groupType);
inline uint8 GetChannelAdc(Adc_ChannelType ChannelId);
inline Adc_ConversionTimeType GetConversionTime(Adc_SamplingTimeType T_sampling, Adc_PrescaleType prescale_value);
inline uint8 GetSamplingTime(Adc_SamplingTimeType T_sampling);
inline uint32 GetExternalTrigConv(Adc_TriggerSourceType TriggerSource,
								  Adc_HwTriggerTimerType TriggerSourceHw);
inline uint32 GetDataAlign(Adc_ResultAlignmentType alignType);
inline FunctionalState GetGroupConvMode(Adc_GroupConvModeType Conversion_Mode);
inline uint32 GetAdcMode(uint8 NumOfGroup, bool IsInjected);
inline uint32 GetAdcClock(Adc_PrescaleType prescaler);
inline FlagStatus GetSoftwareStartConvStatus(Adc_StatusType status);
#endif