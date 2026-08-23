#include "Adc.h"
#include "Adc_mapping.h"
#include <stdlib.h>
#include <stdbool.h>

#define MAX_GROUP 3
static ADC_InitTypeDef Adc_Config;
static Adc_GroupConfigResultBuff Adc_GroupResultList[MAX_GROUP];

void Adc_Init(const Adc_ConfigType *configPtr)
{
	if (configPtr == NULL)
	{
		return;
	}
	uint8 NumOfGroup = sizeof(configPtr->GroupConfig) / sizeof(configPtr->GroupConfig[0]);
	uint8 NumOfChannel = sizeof(configPtr->GroupConfig->listChannel) /
						 sizeof(configPtr->GroupConfig->listChannel[0]);
	ADC_TypeDef *Adc_Group = GetGroupAdc(configPtr->GroupConfig->GroupType);
	// call mapping function to return configured ADC group
	Adc_Config.ADC_Mode = GetAdcMode(NumOfGroup, configPtr->IsInjected);
	Adc_Config.ADC_ContinuousConvMode = GetGroupConvMode(configPtr->GroupConfig->ConvMode);
	Adc_Config.ADC_DataAlign = GetDataAlign(configPtr->AlignmentType);
	Adc_Config.ADC_ExternalTrigConv = GetExternalTrigConv(configPtr->GroupConfig->TriggerSourceType,
														  configPtr->GroupConfig->TriggerSourceHw);
	Adc_Config.ADC_ScanConvMode = NumOfChannel > 1 ? ENABLE : DISABLE;
	Adc_Config.ADC_NbrOfChannel = NumOfChannel;

	// initialize parameter of configPtr to Adc_Config
	ADC_Init(Adc_Group, &Adc_Config);

	// initialze prescaler by calling RCC_ADCCLKConfig
	if (configPtr->prescaler != ADC_AUTOSAR_PRESCALE_DIV1)
	{
		RCC_ADCCLKConfig(GetAdcClock(configPtr->prescaler));
	}
	// initialize channel and sampletime by calling ADC_RegularChannelConfig
	if (NumOfChannel == 1)
	{
		uint8 Channel = GetChannelAdc(configPtr->GroupConfig->listChannel[0]);
		uint8 T_sample = GetSamplingTime(configPtr->SampTime);
		uint8 Rank = configPtr->GroupConfig->Rank;
		ADC_RegularChannelConfig(Adc_Group, Rank, Channel, T_sample);
	}
	if (configPtr->GroupConfig->TriggerSourceType != ADC_TRIGG_SRC_SW)
	{
		uint32 TriggerSourceHw = GetExternalTrigConv(configPtr->GroupConfig->TriggerSourceType,
													 configPtr->GroupConfig->TriggerSourceHw);
		ADC_ExternalTrigInjectedConvConfig(Adc_Group, TriggerSourceHw);
	}
	// write in Adc reg by calling ADC_Init
	ADC_Init(Adc_Group, &Adc_Config);
	// enable Adc by calling ADC_Cmd
	ADC_Cmd(Adc_Group, ENABLE);
}
Std_ReturnType Adc_SetupResultBuffer(
	Adc_GroupType Group,
	Adc_ValueGroupType *DataBufferPtr)
{
	if (Group > MAX_GROUP || DataBufferPtr == NULL)
	{
		return E_NOT_OK;
	}
	Adc_GroupResultList[Group].resultBuff = DataBufferPtr;
	Adc_GroupResultList[Group].IsConfig = true;

	return E_OK;
}
void Adc_StartGroupConversion(Adc_GroupType Group)
{
	ADC_TypeDef *Adc_Group = GetGroupAdc(Group);
	ADC_SoftwareStartConvCmd(Adc_Group, ENABLE);
	// check if the conversion if completed
	if (ADC_GetSoftwareStartConvStatus(Adc_Group) == SET)
	{
		uint16 value = ADC_GetConversionValue(Adc_Group);
		Adc_SetupResultBuffer;
	}
}
