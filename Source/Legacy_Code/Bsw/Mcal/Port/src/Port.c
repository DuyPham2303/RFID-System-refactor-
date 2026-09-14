#include "Port.h"
#include "Port_Mapping.h"

/*Biến tĩnh lưu trữ cấu hình cục bộ hàm*/
static const Port_ConfigSetType *Port_GroupConfig_s;

Std_ReturnType Port_Init(const Port_ConfigSetType *ConfigPtr)
{
	if (ConfigPtr == NULL_PTR)
	{
		return E_NOT_OK;
	}

	for (uint8 index = 0U; index < ConfigPtr->CfgID_Count; index++)
	{

		/*khai báo cấu hình luu trữ trên thanh ghi cứng*/
		GPIO_InitTypeDef Port_InitCfg_s;

		/*truy xuất group cấu hình hiện tại trong danh sách */
		const Port_ConfigType *PortGroupCfg = &ConfigPtr->PinCfgGroup[index];

		/*truy xuất địa chỉ cứng của Port*/
		GPIO_TypeDef *Port_Group = GetPortGroup(PortGroupCfg->HwPortId);

		if (Port_Group == NULL_PTR)
		{
			return E_NOT_OK;
		}

		Port_InitCfg_s.GPIO_Pin = PortGroupCfg->pin;
		Port_InitCfg_s.GPIO_Mode = Port_GetPinMode(PortGroupCfg->ModeType);
		Port_InitCfg_s.GPIO_Speed = Port_GetPinSpeed(PortGroupCfg->SpeedType);

		/*Ánh xạ cấu hình xuống địa chỉ thực tế*/
		GPIO_Init(Port_Group, &Port_InitCfg_s);
	}

	/*copy cấu hình lưu trữ sang biến tĩnh sử dụng cục bộ*/
	Port_GroupConfig_s = ConfigPtr;
	return E_OK;
}

// Std_ReturnType Port_SetPinDirection(
// 	Port_PinType Pin,
// 	Port_PinDirectionType Direction)
// {
// 	uint8 index;
// 	Port_PinModeType mode;

// 	mode = (Direction == PORT_PIN_IN) ? PORT_MODE_INPUT_FLOATING : PORT_MODE_OUTPUT_PUSH_PULL;
// 	for (index = 0U; index < Port_Config.CfgID_Count; index++)
// 	{
// 		if (Port_Config.PinCfgGroup[index].pin == Pin &&
// 			Port_ApplyConfig(&Port_Config.PinCfgGroup[index]) != E_OK)
// 		{
// 			return E_NOT_OK;
// 		}
// 	}

// 	return E_OK;
// }

// Std_ReturnType Port_RefreshPortDirection(void)
// {
// 	return Port_Init(&Port_Config);
// }

// Std_ReturnType Port_SetPinMode(
// 	Port_PinType Pin,
// 	Port_PinModeType Mode)
// {
// 	uint8 index;

// 	for (index = 0U; index < Port_Config.CfgID_Count; index++)
// 	{
// 		if (Port_Config.PinCfgGroup[index].pin == Pin &&
// 			Port_ApplyConfig(&Port_Config.PinCfgGroup[index]) != E_OK)
// 		{
// 			return E_NOT_OK;
// 		}
// 	}

// 	return E_OK;
// }
