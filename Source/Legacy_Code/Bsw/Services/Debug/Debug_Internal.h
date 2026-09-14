/**
 * @file Debug_Internal.h
 * @brief Giao diện nội bộ giữa các thành phần của dịch vụ Debug.
 * @details Khai báo các hàm dùng chung bên trong module Debug, không dành
 *          cho application hoặc các module MCAL gọi trực tiếp.
 */
#ifndef DEBUG_INTERNAL_H
#define DEBUG_INTERNAL_H

#include "Debug.h"

/**
 * @brief Gửi một chuỗi tới callback xuất dữ liệu đã đăng ký.
 * @param Text Chuỗi cần xuất.
 */
void Debug_WriteText(const char *Text);

#endif /* DEBUG_INTERNAL_H */