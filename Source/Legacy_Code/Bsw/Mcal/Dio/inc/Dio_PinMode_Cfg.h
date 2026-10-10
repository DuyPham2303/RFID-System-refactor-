#ifndef DIO_PINMODE_CFG_H
#define DIO_PINMODE_CFG_H

#include "Dio_Types.h"

typedef uint8 Dio_ChannelModeType;

#define DIO_CHANNEL_TIM2_CH1 ((Dio_ChannelModeType)DIO_CHANNEL_A0)
#define DIO_CHANNEL_TIM2_CH2 ((Dio_ChannelModeType)DIO_CHANNEL_A1)
#define DIO_CHANNEL_TIM2_CH3 ((Dio_ChannelModeType)DIO_CHANNEL_A2)
#define DIO_CHANNEL_TIM2_CH4 ((Dio_ChannelModeType)DIO_CHANNEL_A3)

#define DIO_CHANNEL_TIM1_CH1 ((Dio_ChannelModeType)DIO_CHANNEL_A8)
#define DIO_CHANNEL_TIM1_CH2 ((Dio_ChannelModeType)DIO_CHANNEL_A9)
#define DIO_CHANNEL_TIM1_CH3 ((Dio_ChannelModeType)DIO_CHANNEL_A10)
#define DIO_CHANNEL_TIM1_CH4 ((Dio_ChannelModeType)DIO_CHANNEL_A11)

#define DIO_CHANNEL_SPI1_NSS ((Dio_ChannelModeType)DIO_CHANNEL_A4)
#define DIO_CHANNEL_SPI1_SCK ((Dio_ChannelModeType)DIO_CHANNEL_A5)
#define DIO_CHANNEL_SPI1_MISO ((Dio_ChannelModeType)DIO_CHANNEL_A6)
#define DIO_CHANNEL_SPI1_MOSI ((Dio_ChannelModeType)DIO_CHANNEL_A7)

#define DIO_CHANNEL_SPI2_NSS ((Dio_ChannelModeType)DIO_CHANNEL_B12)
#define DIO_CHANNEL_SPI2_SCK ((Dio_ChannelModeType)DIO_CHANNEL_B13)
#define DIO_CHANNEL_SPI2_MISO ((Dio_ChannelModeType)DIO_CHANNEL_B14)
#define DIO_CHANNEL_SPI2_MOSI ((Dio_ChannelModeType)DIO_CHANNEL_B15)

#endif /*DIO_PINMODE_CFG_H */
