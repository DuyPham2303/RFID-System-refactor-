#include "Adc.h"
#include "Adc_mapping.h"
#include <stdlib.h>
#include <stdbool.h>

#define MAX_GROUP 3
static ADC_InitTypeDef Adc_Config;								 /* lưu trữ cấu hình cục bộ sẽ ánh xạ vào thanh ghi phần cứng*/
static Adc_GroupConfigResultBuff Adc_GroupResultList[MAX_GROUP]; /* lưu trữ kết quả chuyển đổi adc của tất group channel*/

void Adc_Init(const Adc_ConfigType *configPtr)
{
	if (configPtr == NULL)
	{
		return;
	}
	// trả về số lượng group và channel đã chọn
	uint8 NumOfGroup = sizeof(configPtr->GroupConfig) / sizeof(configPtr->GroupConfig[0]);
	uint8 NumOfChannel = sizeof(configPtr->GroupConfig->listChannel) /
						 sizeof(configPtr->GroupConfig->listChannel[0]);
	ADC_TypeDef *Adc_Group = GetGroupAdc(configPtr->GroupConfig->GroupType);
	// ánh xạ thông số cấu hình cơ bản từ IoHwAb xuống đơn vị phần cứng tương ứng
	Adc_Config.ADC_Mode = GetAdcMode(NumOfGroup, configPtr->IsInjected);
	Adc_Config.ADC_ContinuousConvMode = GetGroupConvMode(configPtr->GroupConfig->ConvMode);
	Adc_Config.ADC_DataAlign = GetDataAlign(configPtr->AlignmentType);
	Adc_Config.ADC_ExternalTrigConv = GetExternalTrigConv(configPtr->GroupConfig->TriggerSourceType,
														  configPtr->GroupConfig->TriggerSourceHw);
	Adc_Config.ADC_ScanConvMode = NumOfChannel > 1 ? ENABLE : DISABLE;
	Adc_Config.ADC_NbrOfChannel = NumOfChannel;

	// ánh xạ xuống thanh ghi thực tế
	ADC_Init(Adc_Group, &Adc_Config);

	// cấu hình tần số chia cho bộ adc
	if (configPtr->prescaler != ADC_AUTOSAR_PRESCALE_DIV1)
	{
		RCC_ADCCLKConfig(GetAdcClock(configPtr->prescaler));
	}
	// cấu hình kênh adc và tần số lấy mẫu
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
	// ánh xạ bổ sung cài đặt bổ sung nâng cao
	ADC_Init(Adc_Group, &Adc_Config);
	// cho phép bộ adc hoạt động
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
