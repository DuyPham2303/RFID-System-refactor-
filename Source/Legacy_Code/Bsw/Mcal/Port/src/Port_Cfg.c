#include "Port_Cfg.h"

static const Port_ConfigType Port_PinCfgGroup[PORT_CFG_COUNT] = {
    /*
    [SD_CARD_CFG_GENERAL_PIN] =
        {

            .pin = MISO_PIN | MOSI_PIN | SCK_PIN,
            .HwPortId = PORT_A,
            .SpeedType = PORT_SPEED_10MHZ,
            .ModeType = PORT_MODE_ALTERNATE_PUSH_PULL},
    [SD_CARD_CFG_CS_PIN] =
        {
            .pin = SD_CS_PIN,
            .HwPortId = PORT_A,
            .SpeedType = PORT_SPEED_10MHZ,
            .ModeType = PORT_MODE_OUTPUT_PUSH_PULL},
    [RC522_CFG_GENERAL_PIN] =
        {
            .pin = MOSI_PIN | MISO_PIN | SCK_PIN,
            .ModeType = PORT_MODE_ALTERNATE_PUSH_PULL,
            .SpeedType = PORT_SPEED_10MHZ,
            .HwPortId = PORT_A},
    [RC522_CFG_RST_PIN] =
        {
            .pin = RC522_RST_PIN,
            .ModeType = PORT_MODE_OUTPUT_PUSH_PULL,
            .SpeedType = PORT_SPEED_10MHZ,
            .HwPortId = PORT_B},
    [LCD_CFG_GENERAL_PIN] =
        {
            .pin = LCD_SCL_PIN | LCD_SDA_PIN,
            .HwPortId = PORT_B,
            .SpeedType = PORT_SPEED_10MHZ,
            .ModeType = PORT_MODE_ALTERNATE_OPEN_DRAIN},
    [USBTTL_CFG_TX_PIN] =
        {
            .pin = TX_PIN,
            .HwPortId = PORT_A,
            .SpeedType = PORT_SPEED_10MHZ,
            .ModeType = PORT_MODE_ALTERNATE_PUSH_PULL},
    [USBTTL_CFG_RX_PIN] =
        {
            .pin = RX_PIN,
            .HwPortId = PORT_A,
            .SpeedType = PORT_SPEED_10MHZ,
            .ModeType = PORT_MODE_INPUT_FLOATING},
    [RC522_CFG_CS_PIN] =
        {
            .pin = RC522_CS_PIN,
            .ModeType = PORT_MODE_OUTPUT_PUSH_PULL,
            .SpeedType = PORT_SPEED_10MHZ,
            .HwPortId = PORT_B},
    [SERVO_CFG_GENERAL_PIN] =
        {
            .pin = SERVO_PIN,
            .HwPortId = PORT_A,
            .SpeedType = PORT_SPEED_10MHZ,
            .ModeType = PORT_MODE_ALTERNATE_PUSH_PULL},
    */
    [LED_CFG_C13_PIN] =
        {
            .pin = PORT_PIN_C13,
            .ModeType = PORT_MODE_OUTPUT_PUSH_PULL,
            .SpeedType = PORT_SPEED_10MHZ,
            .HwPortId = PORT_C},
    [LED_CFG_A0_PIN] =
        {
            .pin = PORT_PIN_A0,
            .ModeType = PORT_MODE_OUTPUT_PUSH_PULL,
            .SpeedType = PORT_SPEED_10MHZ,
            .HwPortId = PORT_A}};

const Port_ConfigSetType Port_Config = {
    .PinCfgGroup = Port_PinCfgGroup,                                      /* con trỏ đến mảng cấu hình pin */
    .CfgID_Count = sizeof(Port_PinCfgGroup) / sizeof(Port_PinCfgGroup[0]) /* Số lượng ID logic cấu hình cho các tác vụ */
};