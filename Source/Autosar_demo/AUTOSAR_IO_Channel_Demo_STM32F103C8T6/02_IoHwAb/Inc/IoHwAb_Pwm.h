#ifndef IOHWAB_PWM_H
#define IOHWAB_PWM_H

#include "Std_Types.h"
#include "IoHwAb_Pwm_Cfg.h"

void IoHwAb_PwmInit(void);
Std_ReturnType IoHwAb_SetMotorPwm(uint16 DutyCycle);

#endif /* IOHWAB_PWM_H */
