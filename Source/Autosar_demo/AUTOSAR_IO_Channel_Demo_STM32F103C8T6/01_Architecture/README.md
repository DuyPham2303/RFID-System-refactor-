# AUTOSAR-style I/O Channel Demo — STM32F103C8T6

Purpose: demonstrate the separation between:
1. Application logical signal
2. IoHwAb logical I/O channel
3. PWM/Timer MCAL channel
4. Port MCAL physical pin
5. STM32 hardware

Flow:
App -> IoHwAb_SetMotorPwm() -> Pwm_SetDutyCycle(PWM_CHANNEL_TIM1_CH1, duty)
                           |
                           +-> IoHwAb/Pwm configuration maps logical channel
Port initialization is independent:
EcuM/BswM startup -> Port_Init(&Port_Config)

For TIM1_CH1 on STM32F103C8T6:
PWM_CHANNEL_TIM1_CH1 -> PORT_PIN_PA8 -> TIM1_CH1

Important:
- PORT_PIN_PA8 is a physical Port_PinType symbolic ID.
- PWM_CHANNEL_TIM1_CH1 is a peripheral channel ID.
- IOHWAB_CHANNEL_MOTOR_PWM is an application/IoHwAb logical channel ID.
They are intentionally three different abstractions.

The exact Port_ConfigType field names in the user's existing project are preserved in the example
(.pin, .ModeType, .SpeedType, .HwPortId). If the local typedef differs, adapt only the designated
initializer field names.
