#include "Adc_mapping.h"
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

inline ADC_TypeDef* GetGroupAdc(Adc_GroupType groupType){
	switch(groupType){
		case ADC_GROUP_1:		return ADC1;
		case ADC_GROUP_2:		return ADC2;
		case ADC_GROUP_3:		return ADC3;
		default:
			return ADC1;
	}
}
inline uint8 GetChannelAdc(Adc_ChannelType ChannelId){
	switch(ChannelId){
		case ADC_AUTOSAR_CHANNEL_0:	return ADC_Channel_0;
		case ADC_AUTOSAR_CHANNEL_1:	return ADC_Channel_1;
		case ADC_AUTOSAR_CHANNEL_2:	return ADC_Channel_2;
		case ADC_AUTOSAR_CHANNEL_3:	return ADC_Channel_3;
		case ADC_AUTOSAR_CHANNEL_4:	return ADC_Channel_4;
		//case ADC_AUTOSAR_CHANNEL_5:	return ADC_Channel_5;
		case ADC_AUTOSAR_CHANNEL_6:	return ADC_Channel_6;
		case ADC_AUTOSAR_CHANNEL_7:	return ADC_Channel_7;
		case ADC_AUTOSAR_CHANNEL_8:	return ADC_Channel_8;
		case ADC_AUTOSAR_CHANNEL_9:	return ADC_Channel_9;
		case ADC_AUTOSAR_CHANNEL_10:	return ADC_Channel_10;
		case ADC_AUTOSAR_CHANNEL_11:	return ADC_Channel_11;
		case ADC_AUTOSAR_CHANNEL_12:	return ADC_Channel_12;
		case ADC_AUTOSAR_CHANNEL_13:	return ADC_Channel_13;
		case ADC_AUTOSAR_CHANNEL_14:	return ADC_Channel_14;
		case ADC_AUTOSAR_CHANNEL_15:	return ADC_Channel_15;
		case ADC_AUTOSAR_CHANNEL_16:	return ADC_Channel_16;
		default:	
			return ADC_Channel_0;
		
	}
}
inline Adc_ConversionTimeType GetConversionTime(Adc_SamplingTimeType T_sampling,Adc_PrescaleType prescale_value){
	 Adc_ConversionTimeType T_conversion = ((T_sampling + 12) * (1000000/prescale_value));
		return T_conversion;
}
inline uint8 GetSamplingTime(Adc_SamplingTimeType T_sampling){
	switch(T_sampling){
		case ADC_AUTOSAR_SAMPLING_1CYCLE: return ADC_SampleTime_1Cycles5;
		case ADC_AUTOSAR_SAMPLING_7CYCLE: return ADC_SampleTime_7Cycles5;
		case ADC_AUTOSAR_SAMPLING_13CYCLE: return ADC_SampleTime_13Cycles5;
		case ADC_AUTOSAR_SAMPLING_28CYCLE: return ADC_SampleTime_28Cycles5;	
		case ADC_AUTOSAR_SAMPLING_41CYCLE: return ADC_SampleTime_41Cycles5;
		case ADC_AUTOSAR_SAMPLING_55CYCLE: return ADC_SampleTime_55Cycles5;
		case ADC_AUTOSAR_SAMPLING_71CYCLE: return ADC_SampleTime_71Cycles5;
		case ADC_AUTOSAR_SAMPLING_239CYCLE: return ADC_SampleTime_239Cycles5;
		default:
				return ADC_SampleTime_239Cycles5;
	}
}
inline uint32 GetExternalTrigConv(Adc_TriggerSourceType TriggerSourceType,
	Adc_HwTriggerTimerType TriggerSourceHw){
		if(TriggerSourceType == ADC_TRIGG_SRC_SW){ 
			 if(TriggerSourceHw == 0){
					return ADC_AUTOSAR_EXT_TRIG_CONV_None;
			 }
		}
		else if(TriggerSourceType == ADC_TRIGG_SRC_HW){
				if(TriggerSourceHw == ADC_AUTOSAR_EXT_TRIG_CONV_T1_CC1)	return ADC_ExternalTrigConv_T1_CC1;
				else if (TriggerSourceHw == ADC_AUTOSAR_EXT_TRIG_CONV_T1_CC2)	return ADC_ExternalTrigConv_T1_CC2;
				else if (TriggerSourceHw == ADC_AUTOSAR_EXT_TRIG_CONV_T1_CC2)	return ADC_ExternalTrigConv_T2_CC2;
				else return ADC_ExternalTrigConv_T3_TRGO;
		}		
}
inline uint32 GetDataAlign(Adc_ResultAlignmentType alignType){
	switch(alignType){
		case ADC_ALIGN_RIGHT: return ADC_DataAlign_Right;
		case ADC_ALIGN_LEFT:	return ADC_DataAlign_Left;
		default:
			return ADC_DataAlign_Right;
	}
}
inline FunctionalState GetGroupConvMode(Adc_GroupConvModeType Conversion_Mode){
	if(Conversion_Mode == ADC_CONV_MODE_ONESHOT){
			return DISABLE;
	}
	else if(Conversion_Mode == ADC_CONV_MODE_CONTINUOUS){
		return ENABLE;
	}
	else return ENABLE;
}
inline uint32	GetAdcMode(uint8 NumOfGroup,bool IsInjected){
	if(NumOfGroup > 1) return ADC_Mode_Independent;
	else{
		if(IsInjected) return ADC_Mode_RegSimult;
		else					 return ADC_Mode_InjecSimult;
	}
}
inline uint32 GetAdcClock(Adc_PrescaleType prescaler){
	switch(prescaler){
		case ADC_AUTOSAR_PRESCALE_DIV2: return RCC_PCLK2_Div2;
		case ADC_AUTOSAR_PRESCALE_DIV4: return RCC_PCLK2_Div4;
		case ADC_AUTOSAR_PRESCALE_DIV6: return RCC_PCLK2_Div6;
		case ADC_AUTOSAR_PRESCALE_DIV8: return RCC_PCLK2_Div8;
		default:
			return ADC_AUTOSAR_PRESCALE_DIV1;
	}
}
inline FlagStatus GetSoftwareStartConvStatus(Adc_StatusType status){
	switch(status){
		case: 
	}
}	

