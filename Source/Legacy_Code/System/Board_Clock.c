#include "Board_Clock.h"
#include "stm32f10x.h"

/* macro defined for port GPIO */
#define PORT_A_RCC RCC_APB2Periph_GPIOA
#define PORT_B_RCC RCC_APB2Periph_GPIOB
#define PORT_C_RCC RCC_APB2Periph_GPIOC

/* macro defined for Peripherals */
#define APB2_PERIPHERAL_RCC (RCC_APB2Periph_SPI1 | RCC_APB2Periph_TIM1)
#define APB1_PERIPHERAL_RCC (RCC_APB1Periph_SPI2 | RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3)

/**
 * @brief  Cấp clock cho các ngoại vi phần cứng trên bus (GPIO, SPI, I2C, UART, TIM,...).
 */
void Board_PeripheralsClock_Init(void)
{
    RCC_APB2PeriphClockCmd(PORT_A_RCC | PORT_B_RCC | PORT_C_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(APB2_PERIPHERAL_RCC, ENABLE);
    RCC_APB1PeriphClockCmd(APB1_PERIPHERAL_RCC, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
}