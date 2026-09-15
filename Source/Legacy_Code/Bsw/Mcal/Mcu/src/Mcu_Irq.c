#include "Mcu_Irq.h"
#include "Mcu_IrqMap.h"

/*biến tĩnh lưu trữ danh sách cấu hình NVIC của các ngoại vi*/
static NVIC_InitTypeDef NVIC_InitCfgGroup_s[MCU_IRQ_MAX] = {0};

Std_ReturnType Mcu_IrqInit(const Mcu_IrqConfigType *ConfigPtr)
{
    if (ConfigPtr == NULL || ConfigPtr->IrqChannel >= MCU_IRQ_MAX)
    {
        return E_NOT_OK;
    }
    /*truy xuất IRQ channel của cấu hình NVIC tương ứng*/
    Mcu_IrqIdType Id_e = ConfigPtr->IrqChannel;

    /*Lưu trữ sang cấu hình cục bộ với IRQ channel ID ánh xạ tương ứng*/
    NVIC_InitCfgGroup_s[Id_e].NVIC_IRQChannel = Mcu_IrqTable[Id_e];
    NVIC_InitCfgGroup_s[Id_e].NVIC_IRQChannelPreemptionPriority = ConfigPtr->PreemptionPriority;
    NVIC_InitCfgGroup_s[Id_e].NVIC_IRQChannelSubPriority = ConfigPtr->SubPriority;
    NVIC_InitCfgGroup_s[Id_e].NVIC_IRQChannelCmd = ConfigPtr->Enable ? ENABLE : DISABLE;

    /*Ánh xạ toàn bộ cấu hình tương ứng của IRQ channel xuống thanh ghi cứng*/
    NVIC_Init(&NVIC_InitCfgGroup_s[Id_e]);
    return E_OK;
}
