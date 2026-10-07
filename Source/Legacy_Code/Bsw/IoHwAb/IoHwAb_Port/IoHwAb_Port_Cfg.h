#ifndef IOHWAB_PORT_CFG_H
#define IOHWAB_PORT_CFG_H
#include "Port_Types.h"
/* --- 1. THIẾT LẬP ÁNH XẠ LOGIC (MAPPING ID CHO TẦNG ỨNG DỤNG) --- */

typedef uint16 IoHwAb_PortIdType;

#define IOHWAB_LED_BLINK ((IoHwAb_PortIdType)0U)
#define IOHWAB_LED_RED_PIN ((IoHwAb_PortIdType)1U)
#define IOHWAB_LED_GREEN_PIN ((IoHwAb_PortIdType)2U)
#define IOHWAB_LED_BLUE_PIN ((IoHwAb_PortIdType)3U)
#define IOHWAB_LED_YELLOW_PIN ((IoHwAb_PortIdType)4U)

#endif
