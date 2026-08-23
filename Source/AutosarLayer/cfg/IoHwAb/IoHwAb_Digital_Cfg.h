/**
 * @file IoHwAb_Digital_Cfg.h
 * @brief Configuration interface for digital IoHwAb channels.
 * @details Provides board-independent configuration points for digital
 *          signals such as LEDs and switches.
 * @req AUTOSAR ECU Abstraction Layer configuration concept.
 */
#ifndef IOHWAB_DIGITAL_CFG_H
#define IOHWAB_DIGITAL_CFG_H

#include "Std_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /* =====================================================================
     * 1) Định danh các kênh Digital ở mức Ứng dụng (Logical Channels)
     * ===================================================================== */
    typedef enum
    {
        IOHWAB_CH_LED_STATUS = 0, /* Đèn LED báo trạng thái hệ thống */
        IOHWAB_CH_BARRIER_SERVO,  /* Tín hiệu điều khiển Servo/Barrier */
        IOHWAB_CH_RC522_RST,      /* Chân Reset của module RC522 */
        IOHWAB_DIGITAL_CHANNEL_COUNT
    } IoHwAb_DigitalChannelType;

    /* =====================================================================
     * 2) Cấu trúc tham số cấu hình cho từng kênh Digital
     * ===================================================================== */
    typedef struct
    {
        IoHwAb_DigitalChannelType ChannelId;
        uint8 HwChannelId;  /* Ánh xạ xuống kênh Dio/Port thực tế (ví dụ: Dio_Channel_0) */
        uint8 DefaultState; /* Trạng thái mặc định khi khởi tạo (STD_HIGH / STD_LOW) */
    } IoHwAb_DigitalConfigType;

    /* Khai báo bảng cấu hình mẫu để IoHwAb sử dụng */
    extern const IoHwAb_DigitalConfigType IoHwAb_DigitalConfigList[IOHWAB_DIGITAL_CHANNEL_COUNT];

#ifdef __cplusplus
}
#endif

#endif /* IOHWAB_DIGITAL_CFG_H */
