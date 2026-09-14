/**
 * @file Pwm_Types.h
 * @brief Định nghĩa các kiểu dữ liệu của mô-đun PWM.
 * @details Khai báo kiểu dữ liệu cho kênh PWM, chu kỳ, duty cycle,
 *          cực tính, timer phần cứng và cấu hình kênh PWM.
 * @req AUTOSAR_SWS_PWMDriver.
 * @note Các kiểu dữ liệu được định nghĩa nhằm tách biệt giao diện PWM
 *       khỏi phần cứng cụ thể của vi điều khiển.
 */

#ifndef PWM_TYPES_H
#define PWM_TYPES_H
#include "Std_Types.h"

/**
 * @brief Loại dữ liệu biểu diễn giá trị duty cycle được tầng phía trên truyền vào.
 * @details Giá trị này đại diện cho độ rộng xung của PWM, ví dụ 0x0000 = 0%,
 *          0x8000 = 100% theo kiểu biểu diễn nội bộ của driver.
 */
typedef uint16 Pwm_DutyCycleType;

/********************************************************
 * @brief Kiểu dữ liệu biểu diễn giá trị xung thô (Pulse Width Ticks)
 * @detail Dùng để thiết lập trực tiếp giá trị thanh ghi compare (CCR),
 *          giá trị hợp lệ từ 0x0000 - 0xffff
 ******************/
typedef uint16 Pwm_Pulse;

/********************************************************
 * @brief Kiểu dữ liệu định danh kênh PWM được tầng phía trên sử dụng.
 * @details Dùng để xác định kênh PWM cần thao tác trong các API của driver,
 *          mỗi kênh được ánh xạ với I/O Pin tương ứng
 *          ví dụ như bật/tắt, đặt duty cycle, hoặc đặt trạng thái Idle.
 ******************/
typedef enum Pwm_ChannelType
{
    PWM_CHANNEL_1 = 1U,
    PWM_CHANNEL_2,
    PWM_CHANNEL_3,
    PWM_CHANNEL_4
} Pwm_ChannelType;
/********************************************************
 * @brief Kiểu dữ liệu xác định chế độ so sánh ngõ ra (Output Compare Mode)
 * @detail Quy định cách thức hoạt động của kênh khi bộ đếm khớp với giá trị so sánh
 ******************/
typedef enum Pwm_OcModeType
{
    PWM_OC_MODE_TIMING = 0U, /* So sánh xung đơn giản, không tạo PWM */
    PWM_OC_MODE_ACTIVE,      /* Kích hoạt chân khi CNT khớp CCR */
    PWM_OC_MODE_INACTIVE,    /* Vô hiệu hóa chân khi CNT khớp CCR */
    PWM_OC_MODE_TOGGLE,      /* Chuyển đổi trạng thái mỗi lần khớp CCR */
    PWM_OC_MODE_PWM1,        /* Ngõ ra active khi CNT < CCR */
    PWM_OC_MODE_PWM2         /* Ngõ ra inactive khi CNT < CCR */
} Pwm_OcModeType;

/********************************************************
 * @brief Kiểu dữ liệu trạng thái ngõ ra chính của kênh PWM
 * @detail Xác định mức logic hiện tại của chân xuất xung chính
 ******************/
typedef enum Pwm_OutputStateType
{
    PWM_OUTPUT_DISABLED = 0U,
    PWM_OUTPUT_ENABLED
} Pwm_OutputStateType;
/********************************************************
 * @brief Kiểu dữ liệu trạng thái ngõ ra phụ/bổ sung (Complementary)
 * @detail Quản lý trạng thái của đường tín hiệu đảo pha (chân N),
 *          chỉ dành dành cho TIM1 và TIM8
 ******************/
typedef enum Pwm_OutputNStateType
{
    PWM_OUTPUT_N_DISABLED = 0U,
    PWM_OUTPUT_N_ENABLED
} Pwm_OutputNStateType;

/********************************************************
 * @brief Kiểu dữ liệu cấu hình cực tính cho ngõ ra chính
 * @detail Xác định xung là mức cao tích cực (active high) hay mức thấp tích cực (active low)
 ******************/
typedef enum Pwm_PolarityType
{
    PWM_POLARITY_HIGH = 0U,
    PWM_POLARITY_LOW
} Pwm_PolarityType;

/********************************************************
 * @brief Kiểu dữ liệu cấu hình cực tính cho ngõ ra bổ sung (Complementary)
 * @detail Đảm bảo đồng bộ cực tính đảo pha cho các ứng dụng cầu H hoặc motor,
           chỉ dành dành cho TIM1 và TIM8
 ******************/
typedef enum Pwm_PolarityNType
{
    PWM_POLARITY_N_HIGH = 0U,
    PWM_POLARITY_N_LOW
} Pwm_PolarityNType;

/********************************************************
 * @brief Kiểu dữ liệu trạng thái ngõ ra chính khi ở chế độ nghỉ (Idle State)
 * @detail Quy định mức điện áp an toàn của chân chính khi driver dừng hoạt động,
           chỉ dành dành cho TIM1 và TIM8
 ******************/
typedef enum Pwm_IdleStateType
{
    PWM_IDLE_SET = 0U,
    PWM_IDLE_RESET
} Pwm_IdleStateType;

/********************************************************
 * @brief Kiểu dữ liệu trạng thái ngõ ra bổ sung khi ở chế độ nghỉ (Idle State)
 * @detail Quy định mức điện áp an toàn của chân phụ (chân N) khi dừng hệ thống,
           chỉ dành dành cho TIM1 và TIM8
 ******************/
typedef enum Pwm_IdleStateNType
{
    PWM_IDLE_N_SET = 0U,
    PWM_IDLE_N_RESET
} Pwm_IdleStateNType;

#endif