#ifndef IOHWAB_PWM_CFG_H
#define IOHWAB_PWM_CFG_H

#include "Std_Types.h"
#include "Pwm_Types.h"

/* Application/IoHwAb logical channel IDs. */
typedef uint8 IoHwAb_PwmChannelType;

#define IOHWAB_CHANNEL_MOTOR_PWM   ((IoHwAb_PwmChannelType)0u)
#define IOHWAB_CHANNEL_LED_PWM     ((IoHwAb_PwmChannelType)1u)
#define IOHWAB_CHANNEL_INVALID     ((IoHwAb_PwmChannelType)0xFFu)

/* Logical -> MCAL channel configuration. */
typedef struct
{
    IoHwAb_PwmChannelType LogicalChannel;
    Pwm_ChannelType       PwmChannel;
} IoHwAb_PwmChannelConfigType;

extern const IoHwAb_PwmChannelConfigType IoHwAb_PwmChannelConfig[];

#define IOHWAB_PWM_CHANNEL_COUNT \
    ((uint8)(sizeof(IoHwAb_PwmChannelConfig) / sizeof(IoHwAb_PwmChannelConfig[0])))

#endif /* IOHWAB_PWM_CFG_H */
