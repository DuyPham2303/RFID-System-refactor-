#include "Port_Mapping.h"

GPIO_TypeDef *GetPortGroup(Port_PortType GPIOx)
{
    switch (GPIOx)
    {
    case PORT_A:
        return GPIOA;
    case PORT_B:
        return GPIOB;
    case PORT_C:
        return GPIOC;
    case PORT_D:
        return GPIOD;
    default:
        return NULL_PTR;
    }
}
GPIOMode_TypeDef Port_GetPinMode(Port_PinModeType mode)
{
    switch (mode)
    {
    case PORT_MODE_ANALOG:
        return GPIO_Mode_AIN;
    case PORT_MODE_INPUT_FLOATING:
        return GPIO_Mode_IN_FLOATING;
    case PORT_MODE_INPUT_PULL_DOWN:
        return GPIO_Mode_IPD;
    case PORT_MODE_INPUT_PULL_UP:
        return GPIO_Mode_IPU;
    case PORT_MODE_OUTPUT_OPEN_DRAIN:
        return GPIO_Mode_Out_OD;
    case PORT_MODE_OUTPUT_PUSH_PULL:
        return GPIO_Mode_Out_PP;
    case PORT_MODE_ALTERNATE_OPEN_DRAIN:
        return GPIO_Mode_AF_OD;
    case PORT_MODE_ALTERNATE_PUSH_PULL:
        return GPIO_Mode_AF_PP;
    default:
        return GPIO_Mode_AIN;
    }
}
GPIOSpeed_TypeDef Port_GetPinSpeed(Port_PinSpeedType speed)
{
    switch (speed)
    {
    case PORT_SPEED_2MHZ:
        return GPIO_Speed_2MHz;
    case PORT_SPEED_10MHZ:
        return GPIO_Speed_10MHz;
    case PORT_SPEED_50MHZ:
        return GPIO_Speed_50MHz;
    default:
        return GPIO_Speed_2MHz;
    }
}