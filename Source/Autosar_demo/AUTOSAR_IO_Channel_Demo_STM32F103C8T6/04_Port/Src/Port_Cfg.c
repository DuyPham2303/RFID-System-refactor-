/*
 * Merge this pattern into your existing Port_Cfg.c.
 *
 * The important point is that Port configuration owns:
 *     physical pin + electrical mode
 *
 * It does NOT own the IoHwAb logical channel.
 */

#include "Port_Cfg.h"
#include "Port_PinId_Cfg.h"
#include "Port_PinMode_Cfg.h"

static const Port_ConfigType Port_PinCfgGroup[PORT_CFG_COUNT] =
{
    [LED_CFG_C13_PIN] =
    {
        .pin       = PORT_PIN_PC13,
        .ModeType  = PORT_MODE_OUTPUT_PUSH_PULL,
        .SpeedType = PORT_SPEED_10MHZ,
        .HwPortId  = PORT_C
    },

    /*
     * TIM1_CH1 output:
     *   logical PWM channel: PWM_CHANNEL_TIM1_CH1
     *   physical Port pin:  PORT_PIN_PA8
     *   Port mode:           TIM1_CH1 alternate function
     */
    [MOTOR_PWM_TIM1_CH1_PIN] =
    {
        .pin       = PORT_PIN_PA8,
        .ModeType  = PORT_MODE_AF_PUSH_PULL,
        .SpeedType = PORT_SPEED_10MHZ,
        .HwPortId  = PORT_A
    }
};

const Port_ConfigSetType Port_Config =
{
    .PinCfgGroup = Port_PinCfgGroup,
    .CfgID_Count = (uint32)(sizeof(Port_PinCfgGroup) /
                            sizeof(Port_PinCfgGroup[0]))
};
