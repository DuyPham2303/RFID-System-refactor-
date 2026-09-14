/**
 * @file Port_Mapping.h
 * @brief Định nghĩa ánh xạ giữa logical pin và chân GPIO phần cứng.
 * @details Cung cấp các macro và ánh xạ để chuyển đổi giữa định danh pin trừu
 *          tượng của AUTOSAR và bitmask tương ứng của STM32 GPIO.
 * @req AUTOSAR_SWS_PortDriver
 * @note File này đóng vai trò lớp ánh xạ phần cứng, giúp tách API Port khỏi chi
 *       tiết chip cụ thể.
 */
#ifndef PORT_MAPPING_H
#define PORT_MAPPING_H

#include "Port_dtypes.h"
#include "stm32f10x_gpio.h"

GPIO_TypeDef *GetPortGroup(Port_PortType port);
GPIOMode_TypeDef Port_GetPinMode(Port_PinModeType mode);
GPIOSpeed_TypeDef Port_GetPinSpeed(Port_PinSpeedType speed);

#endif /* PORT_MAPPING_H */