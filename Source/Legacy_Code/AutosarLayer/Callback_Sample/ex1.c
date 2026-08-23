#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#define IDLE 0
#define RECV_HEADER 1
#define RECV_PAYLOAD 2
#define RECV_CHECKSUM 3
#define PROCESSING 4
#define ERROR 5

#define FRAME_LED_GREEN 0x01
#define FRAME_SENSOR_REQUEST 0x02
#define FRAME_LED_FREQ 0x03
#define FRAME_ERROR_NOTIF 0x04

// USART_InitTypeDef huart1; // UART1 for ESP01
// GPIO_TypeDef *GPIOA;      // PA5

uint8_t process_frame(int state, uint8_t *buffer, int *buf_index, uint8_t new_byte, int *checksum, int *payload_len)
{
    switch (state)
    {
    case IDLE:
        printf("Idle: Waiting for FrameID\n");
        if (new_byte == FRAME_LED_GREEN || new_byte == FRAME_SENSOR_REQUEST ||
            new_byte == FRAME_LED_FREQ || new_byte == FRAME_ERROR_NOTIF)
        {
            buffer[0] = new_byte;
            *buf_index = 1;
            *checksum = new_byte;
            *payload_len = (new_byte == FRAME_SENSOR_REQUEST) ? 0 : 1; // Sensor request without payload
            return (*payload_len == 0) ? RECV_CHECKSUM : RECV_PAYLOAD;
        }
        return IDLE;
    case RECV_PAYLOAD:
        printf("Receiving Payload: byte %d\n", *buf_index);
        if (*buf_index < *payload_len + 1)
        {
            buffer[*buf_index] = new_byte;
            *checksum += new_byte;
            (*buf_index)++;
            return RECV_PAYLOAD;
        }
        return RECV_CHECKSUM;
    case RECV_CHECKSUM:
        printf("Receiving Checksum\n");
        if (new_byte == (uint8_t)(*checksum & 0xFF))
        {
            return PROCESSING;
        }
        return ERROR;
    case PROCESSING:
        printf("Processing FrameID: 0x%02X\n", buffer[0]);
        switch (buffer[2])
        {
        case FRAME_LED_GREEN:
            printf("LED Green: %s\n", buffer[1] ? "ON" : "OFF");
            // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, buffer[1] ? GPIO_PIN_SET : GPIO_PIN_RESET);
            break;
        case FRAME_SENSOR_REQUEST:
            printf("Requesting Temp/Humidity\n");
            // Simulate Sensor Data
            uint8_t response[] = {0x02, 25, 60, 87}; // FrameID, Temp, Humidity, Checksum
            // HAL_UART_Transmit(&huart1, response, 4, 100);
            break;
        case FRAME_LED_FREQ:
            printf("Set LED Frequency: %d Hz\n", buffer[1]);
            // Simulate LED Frequency
            break;
        case FRAME_ERROR_NOTIF:
            printf("Error Notification: Code 0x%02X\n", buffer[1]);
            break;
        }
        *buf_index = 0;
        *checksum = 0;
        return IDLE;
    case ERROR:
        printf("Error: Invalid packet\n");
        *buf_index = 0;
        *checksum = 0;
        return IDLE;
    default:
        printf("Invalid state\n");
        return IDLE;
    }
}

int main()
{
    uint8_t buffer[10] = {0};
    int buf_index = 0, checksum = 0, payload_len = 0, state = IDLE;
    // Simulate UART Data from ESP01
    uint8_t input[] = {0x01, 0x01, 0x02, 0x02, 0x02, 0x03, 0x02, 0x05, 0x04, 0xFF, 0x03};
    int input_index = 0;

    for (int i = 0; i < 11; i++)
    {
        state = process_frame(state, buffer, &buf_index, input[input_index++], &checksum, &payload_len);
        sleep(10);
    }
    printf("System stopped\n");
    return 0;
}
