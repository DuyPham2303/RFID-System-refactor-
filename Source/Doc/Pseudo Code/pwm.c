/*
 * PWM channel -> physical output pin.
 *
 * This is MCU/project configuration. The PWM driver does not need
 * to hard-code "PA8" in its algorithm.
 *
 * TIM1_CH1 default route on STM32F103C8T6:
 *     TIM1_CH1 -> PA8
 */
const Pwm_ChannelConfigType Pwm_ChannelConfig[] =
    {
        {.Channel = PWM_CHANNEL_TIM1_CH1,
         .OutputPin = PORT_PIN_PA8,
         .Remap = 0u},

        {.Channel = PWM_CHANNEL_TIM3_CH1,
         .OutputPin = PORT_PIN_PA6,
         .Remap = 0u}};

static const Port_ConfigType Port_PinCfgGroup[PORT_CFG_COUNT] =
    {
        [LED_CFG_C13_PIN] =
            {
                .pin = PORT_PIN_PC13,
                .ModeType = PORT_MODE_OUTPUT_PUSH_PULL,
                .SpeedType = PORT_SPEED_10MHZ,
                .HwPortId = PORT_C},

        /*
         * TIM1_CH1 output:
         *   logical PWM channel: PWM_CHANNEL_TIM1_CH1
         *   physical Port pin:  PORT_PIN_PA8
         *   Port mode:           TIM1_CH1 alternate function
         */
        [MOTOR_PWM_TIM1_CH1_PIN] =
            {
                .pin = PORT_PIN_PA8,
                .ModeType = PORT_MODE_AF_PUSH_PULL,
                .SpeedType = PORT_SPEED_10MHZ,
                .HwPortId = PORT_A}};

const Port_ConfigSetType Port_Config =
    {
        .PinCfgGroup = Port_PinCfgGroup,
        .CfgID_Count = (uint32)(sizeof(Port_PinCfgGroup) /
                                sizeof(Port_PinCfgGroup[0]))};

/*
 * This is the abstraction boundary:
 *
 * IOHWAB_CHANNEL_MOTOR_PWM
 *          |
 *          v
 * PWM_CHANNEL_TIM1_CH1
 *
 * No physical GPIO pin appears here.
 */
const IoHwAb_PwmChannelConfigType IoHwAb_PwmChannelConfig[] =
    {
        {.LogicalChannel = IOHWAB_CHANNEL_MOTOR_PWM,
         .PwmChannel = PWM_CHANNEL_TIM1_CH1},

        {.LogicalChannel = IOHWAB_CHANNEL_LED_PWM,
         .PwmChannel = PWM_CHANNEL_TIM3_CH1}};
