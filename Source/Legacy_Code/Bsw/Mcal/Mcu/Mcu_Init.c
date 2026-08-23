#include "Mcu_Init.h"
#include "stm32f10x.h"

/* macro defined for port GPIO */
#define PORT_A_RCC RCC_APB2Periph_GPIOA
#define PORT_B_RCC RCC_APB2Periph_GPIOB
#define PORT_C_RCC RCC_APB2Periph_GPIOC

/* macro defined for Peripherals */
#define RCC_SPI1 RCC_APB2Periph_SPI1
#define RCC_I2C1 RCC_APB1Periph_I2C1
#define RCC_USART1 RCC_APB2Periph_USART1
#define RCC_TIM1 RCC_APB2Periph_TIM1
#define RCC_TIM2 RCC_APB1Periph_TIM2
#define RCC_TIM3 RCC_APB1Periph_TIM3
/**
 * @brief  Khởi tạo hệ thống xung nhịp cơ bản (RCC, PLL, Flash Latency, Bus Clocks).
 * @note   Chạy đầu tiên trong EcuM_Init() để đảm bảo chip chạy đúng tần số mong muốn.
 */
void Board_InitClock(void)
{
    /* tạm thời bỏ qua do đã dùng thư viện systemclock của hệ thống*/
}

/**
 * @brief  Cấp clock cho các ngoại vi phần cứng trên bus (GPIO, SPI, I2C, UART, TIM,...).
 */
void Board_InitPeripheralsClock(void)
{
    // cấp clock cho Các GPIO Port
    RCC_APB2PeriphClockCmd(PORT_A_RCC | PORT_B_RCC | PORT_C_RCC, ENABLE);
    // cấp clock cho ngoại vi Mcu
    RCC_APB1PeriphClockCmd(RCC_SPI1 | RCC_I2C1 | RCC_USART1 | RCC_TIM1 | RCC_TIM2 | RCC_TIM3, ENABLE);
}