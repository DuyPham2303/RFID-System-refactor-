/**
 * @file Port.h
 * @brief Giao diện public của driver Port MCAL theo AUTOSAR.
 * @details Khai báo các API, kiểu cấu hình và dịch vụ được sử dụng để cấu hình
 *          các pin GPIO của MCU, bao gồm hướng vào/ra, mode chức năng và khởi tạo
 *          ban đầu của module.
 * @req AUTOSAR_SWS_PortDriver
 * @note File này định nghĩa giao diện trừu tượng cho tầng phía trên; chi tiết
 *       truy cập phần cứng được tách riêng vào các file thấp hơn.
 */
#ifndef PORT_H
#define PORT_H

#include "Port_Cfg.h"

/********************************************************
 * @brief Khởi tạo cấu hình ban đầu cho toàn bộ module Port.
 * @details Thiết lập trạng thái mặc định cho các pin theo bảng cấu hình,
 *          bao gồm hướng vào/ra, mode chức năng, tốc độ, và các tham số
 *          liên quan đến phần cứng. Hàm này thường được gọi một lần trong
 *          quá trình khởi động hệ thống.
 * @param ConfigPtr: Con trỏ tới cấu hình Port cần dùng để khởi tạo.
 * @return E_OK nếu khởi tạo thành công, E_NOT_OK nếu có lỗi cấu hình.
 *****************/
Std_ReturnType Port_Init(const Port_ConfigSetType *ConfigPtr);

/********************************************************
 * @brief Thay đổi hướng của một pin trong thời gian chạy.
 * @details Dùng để chuyển pin giữa chế độ input và output sau khi hệ thống
 *          đã khởi động, ví dụ khi pin được tái sử dụng cho nhiều chức năng
 *          khác nhau theo trạng thái ứng dụng.
 * @param Pin: Định danh pin cần thay đổi hướng.
 * @param Direction: Giá trị hướng mới của pin, ví dụ PORT_PIN_IN hoặc
 *        PORT_PIN_OUT.
 * @return E_OK nếu đổi hướng thành công, E_NOT_OK nếu pin không hợp lệ.
 *****************/
Std_ReturnType Port_SetPinDirection(
	Port_PinType Pin,
	Port_PinDirectionType Direction);

/********************************************************
 * @brief Cập nhật lại hướng pin theo bộ nhớ cấu hình hoặc trạng thái hiện tại.
 * @details Hàm này giúp đồng bộ trạng thái direction của các pin với cấu hình
 *          đã lưu hoặc áp lại các hướng mặc định sau khi có thay đổi cấu hình
 *          hệ thống. Nó được dùng khi cần reset/reapply chế độ I/O theo nhóm.
 * @return E_OK nếu cập nhật thành công, E_NOT_OK nếu có lỗi trong quá trình
 *         đồng bộ.
 *****************/
Std_ReturnType Port_RefreshPortDirection();

/********************************************************
 * @brief Thiết lập mode chức năng cho một pin cụ thể.
 * @details Điều chỉnh chế độ kỹ thuật của pin, ví dụ chức năng GPIO, UART,
 *          SPI, I2C, timer, ADC, v.v. Hàm này thường được dùng ở thời điểm
 *          runtime nếu pin thay đổi vai trò chức năng trong hệ thống.
 * @param Pin: Định danh pin cần cấu hình mode.
 * @param Mode: Giá trị mode tương ứng với chức năng cần gán cho pin.
 * @return E_OK nếu thiết lập mode thành công, E_NOT_OK nếu pin hoặc mode không
 *         hợp lệ.
 *****************/
Std_ReturnType Port_SetPinMode(
	Port_PinType Pin,
	Port_PinModeType Mode);
#endif