// Define FrameIDs
#define FRAME_LED_GREEN (0x01U)
#define FRAME_SENSOR_REQUEST (0x02U)
#define FRAME_LED_FREQ (0x03U)
#define FRAME_ERROR_NOTIF (0x04U)

// Function pointer type for frame handlers
typedef void (*FrameHandler)(Frame_t *frame);

// Frame handler functions
void handle_led_green(Frame_t *frame)
{
    printf("LED Green: %s\n", frame->payload[0] ? "ON" : "OFF");
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, frame->payload[0] ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void handle_sensor_request(Frame_t *frame)
{
    printf("Requesting Temp/Humidity\n");
    uint8_t response[] = {0x02, 25, 60, 87}; // FrameID, Temp, Humidity, Checksum
    HAL_UART_Transmit(&huart1, response, 4, 100);
}

void handle_led_freq(Frame_t *frame)
{
    printf("Set LED Frequency: %d Hz\n", frame->payload[0]);
}

void handle_error_notif(Frame_t *frame)
{
    printf("Error Notification: Code 0x%02X\n", frame->payload[0]);
}

// Frame handler mapping table
typedef struct
{
    uint8_t frame_id;
    FrameHandler handler;
} FrameHandlerMap_t;

FrameHandlerMap_t frame_handlers[] = {
    {FRAME_LED_GREEN, handle_led_green},
    {FRAME_SENSOR_REQUEST, handle_sensor_request},
    {FRAME_LED_FREQ, handle_led_freq},
    {FRAME_ERROR_NOTIF, handle_error_notif},
    {0, NULL} // End of table
};