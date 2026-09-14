/**
 * @file Debug.h
 * @brief Giao diện dịch vụ debug và báo cáo trạng thái hệ thống.
 * @details Cung cấp API để báo cáo lỗi khởi tạo MCAL và xuất thông tin cấu hình
 *          của các module Port, GPT và PWM. Phần triển khai có thể nối tới
 *          UART, USB-TTL hoặc terminal khác thông qua callback.
 */
#ifndef DEBUG_H
#define DEBUG_H

#include "Std_Types.h"

/*
 * @brief Kiểu dữ liệu xác định module MCAL hoặc BSW đang báo cáo trạng thái.
 * @details Dùng để phân biệt các module khi xuất thông tin debug.
 */
typedef enum
{
    DEBUG_MODULE_PORT = 0U,
    DEBUG_MODULE_GPT,
    DEBUG_MODULE_PWM,
    DEBUG_MODULE_ECUM,
    DEBUG_MODULE_COUNT
} Debug_ModuleType;

/*
 * @brief Kiểu dữ liệu xác định trạng thái của module khi báo cáo debug.
 * @details Dùng để phân biệt các trạng thái khác nhau khi xuất thông tin debug.
 */
typedef enum
{
    DEBUG_STATUS_OK = 0U,
    DEBUG_STATUS_INIT_FAILED,
    DEBUG_STATUS_INVALID_CONFIG,
    DEBUG_STATUS_RUNTIME_ERROR
} Debug_StatusType;

/*
 * @brief Kiểu dữ liệu xác định loại thông tin debug cần xuất.
 * @details Dùng để phân biệt các loại thông tin debug khác nhau.
 */
typedef enum
{
    DEBUG_INFO_ALL = 0U,
    DEBUG_INFO_PORT,
    DEBUG_INFO_GPT,
    DEBUG_INFO_PWM
} Debug_InfoType;
/*Debug.c implement*/
/*
 * @brief Kiểu dữ liệu hàm callback để xuất dữ liệu debug.
 * @details Ứng dụng cung cấp hàm này để Debug service gọi khi cần xuất thông tin.
 */

typedef void (*Debug_WriteType)(const char *text);

/** @brief Khởi tạo dịch vụ debug với hàm xuất dữ liệu do ứng dụng cung cấp. */
void Debug_Init(Debug_WriteType WriteFunction);

/** @brief Báo cáo trạng thái khởi tạo của một module. */
void Debug_ReportInitStatus(Debug_ModuleType Module, Std_ReturnType Result);

/** @brief Báo cáo mã lỗi của một module MCAL. */
void Debug_ReportError(Debug_ModuleType Module, uint8 ErrorCode);

/** @brief Xuất thông tin cấu hình của một hoặc toàn bộ module. */
void Debug_PrintInfo(Debug_InfoType Info);

/*Debug_info.c implement*/
/** @brief Xuất toàn bộ thông tin cấu hình hệ thống. */
void Debug_PrintAllConfig(void);

/** @brief Xuất riêng thông tin cấu hình Port. */
void Debug_PrintPortConfig(void);

/** @brief Xuất riêng thông tin cấu hình GPT. */
void Debug_PrintGptConfig(void);

/** @brief Xuất riêng thông tin cấu hình PWM. */
void Debug_PrintPwmConfig(void);
#endif /* DEBUG_H */
