/**
 * @file Debug_Info.c
 * @brief Định dạng và xuất thông tin cấu hình hệ thống.
 * @details Cung cấp phần triển khai cho các API đọc thông tin cấu hình
 *          của Port, GPT và PWM, sau đó gửi dữ liệu qua giao diện nội bộ
 *          của dịch vụ Debug.
 * @note File này không trực tiếp gọi printf hoặc phụ thuộc vào UART cụ thể;
 *       cơ chế xuất dữ liệu được cung cấp bởi Debug.c.
 */

#include "Debug.h"
#include "Debug_Internal.h"

#include "../Bsw/Mcal/Gpt/Gpt_Cfg.h"
#include "../Bsw/Mcal/Pwm/Pwm_Cfg.h"
#include "../Bsw/Mcal/Port/Port_Cfg.h"
/** @brief Xuất toàn bộ thông tin cấu hình hệ thống. */
void Debug_PrintAllConfig(void)
{
    Debug_PrintPortConfig();
    Debug_PrintGptConfig();
    Debug_PrintPwmConfig();
}

/** @brief Xuất riêng thông tin cấu hình Port. */
void Debug_PrintPortConfig(void)
{
    // đọc chuỗi cấu hình từ các ngoãi vi mcal
    /*
        - các module mcal/cfg tạo pair key - value độc lập
        - trả về chuỗi format cho moduel debug
    */
    // gửi chuỗi qua giao diện để xử lý
    Debug_WriteText("[DEBUG] PORT configuration\r\n");
}

/** @brief Xuất riêng thông tin cấu hình GPT. */
void Debug_PrintGptConfig(void)
{
    Debug_WriteText("[DEBUG] GPT configuration\r\n");
}

/** @brief Xuất riêng thông tin cấu hình PWM. */
void Debug_PrintPwmConfig(void)
{
    Debug_WriteText("[DEBUG] PWM configuration\r\n");
}