#include "Port_Cfg.h"
#include "Port_PinMode_Cfg.h"

static const Port_ConfigType Port_PinCfgGroup[] = {
    {.HwPortId = PORT_PIN_SPI2,
     .ModeType = PORT_MODE_ALTERNATE_PUSH_PULL,
     .pin = PORT_PIN_AFMODE_SPI2_MOSI | PORT_PIN_AFMODE_SPI2_SCK,
     .SpeedType = PORT_SPEED_50MHZ},
    {.HwPortId = PORT_PIN_SPI2,
     .ModeType = PORT_MODE_OUTPUT_PUSH_PULL,
     .pin = PORT_PIN_AFMODE_SPI2_NSS,
     .SpeedType = PORT_SPEED_50MHZ},
    {.HwPortId = PORT_PIN_SPI2,
     .ModeType = PORT_MODE_INPUT_PULL_UP,
     .pin = PORT_PIN_AFMODE_SPI2_MISO,
     .SpeedType = PORT_SPEED_50MHZ},
    {.HwPortId = PORT_C,
     .ModeType = PORT_MODE_OUTPUT_PUSH_PULL,
     .pin = PORT_PIN_C13,
     .SpeedType = PORT_SPEED_50MHZ}};

const Port_ConfigSetType g_Port_ConfigGroup = {
    .PinCfgGroup = Port_PinCfgGroup,                                      /* con trỏ đến mảng cấu hình pin */
    .CfgID_Count = sizeof(Port_PinCfgGroup) / sizeof(Port_PinCfgGroup[0]) /* Số lượng ID logic cấu hình cho các tác vụ */
};