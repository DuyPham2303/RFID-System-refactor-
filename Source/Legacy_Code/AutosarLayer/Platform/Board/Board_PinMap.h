/**
 * @file Board_PinMap.h
 * @brief Board-support physical pin mapping interface.
 * @details Defines board-level mapping between ECU signals and MCU pins used
 *          by IoHwAb and MCAL configuration.
 * @req AUTOSAR-adjacent BSP support; outside standardized BSW modules.
 */
#ifndef BOARD_PINMAP_H
#define BOARD_PINMAP_H

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief  Khởi tạo hệ thống xung nhịp cơ bản (RCC, PLL, Flash Latency, Bus Clocks).
     * @note   Chạy đầu tiên trong EcuM_Init() để đảm bảo chip chạy đúng tần số mong muốn.
     */
    void Board_InitClock(void);

    /**
     * @brief  Cấp clock cho các ngoại vi phần cứng trên bus (GPIO, SPI, I2C, UART, TIM,...).
     */
    void Board_InitPeripheralsClock(void);

    /**
     * @brief  Cấu hình chế độ chân (Pin Mode): Alternate Function, Push-Pull, Pull-up/Down, Speed.
     * @note   Thiết lập các chân phần cứng thô (ví dụ: PA5, PA6, PA7 thành chân SPI)
     *         nhưng chưa định nghĩa thiết bị ngoại vi nào cắm vào đó.
     */
    void Board_InitPinMapping(void);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_PINMAP_H */
