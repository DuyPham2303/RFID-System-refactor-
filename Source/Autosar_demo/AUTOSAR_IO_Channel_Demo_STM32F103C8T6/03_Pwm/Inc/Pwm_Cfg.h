#ifndef PWM_CFG_H
#define PWM_CFG_H

#include "Pwm_Types.h"
#include "Port_PinId_Cfg.h"

typedef struct
{
    Pwm_ChannelType Channel;
    Port_PinType    OutputPin;
    uint8           Remap;
} Pwm_ChannelConfigType;

extern const Pwm_ChannelConfigType Pwm_ChannelConfig[];

#endif /* PWM_CFG_H */
