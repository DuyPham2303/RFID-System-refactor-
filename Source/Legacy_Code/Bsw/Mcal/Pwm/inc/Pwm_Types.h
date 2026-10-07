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
#include "./Bsw/Services/Common/Std_Types.h"

/********************************************************
 * @brief Kiểu dữ liệu định danh kênh PWM được tầng phía trên sử dụng.
 * @details Dùng để xác định kênh PWM cần thao tác trong các API của driver,
 *          mỗi kênh được ánh xạ với I/O Pin tương ứng
 *          ví dụ như bật/tắt, đặt duty cycle, hoặc đặt trạng thái Idle.
 ******************/
typedef enum Pwm_ChannelType
{
    PWM_CHANNEL_CH1 = 1U,
    PWM_CHANNEL_CH2,
    PWM_CHANNEL_CH3,
    PWM_CHANNEL_CH4,
    PWM_MAX_CHANNELS
} Pwm_ChannelType;

typedef enum Hw_TimerGroupId
{
    HW_TIMER_GROUP_1 = 1U,
    HW_TIMER_GROUP_2,
    HW_TIMER_GROUP_3,
} Hw_TimerGroupIdType;
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

/********************************************************
 * @brief kiểu dữ liệu mô tả giá trị tần số chia timer
 * @detail được sử dụng để xác định thời gian của 1 tick count, nhằm
 *         chia nhỏ tần số Clock đầu vào của bộ APB timer, phù hợp với
 *         nhu cầu xử lý của hệ thống. Giá trị nhập trong khoảng
 *         0x0000 - 0xffff (16-bit timer)
 *****************/
typedef uint16 Pwm_PrescalerValue;

/********************************************************
 * @brief kiểu dữ liệu mô tả chế độ đếm của timer
 * @detail được sử dụng để xác định cách thức mà timer sẽ đếm ra sao
 *****************/
typedef enum Pwm_CounterModeType
{
    PWM_COUNTER_MODE_UP = 0U,
    PWM_COUNTER_MODE_DOWN
} Pwm_CounterModeType;

/********************************************************
 * @brief kiểu dữ liệu mô tả giá trị đếm tràn của chu kỳ timer
 * @detail được sử dụng để xác định thời điểm reset giá trị đếm trong thanh ghi ARR
 *         giá trị nhập trong khoảng 0x0000 - 0xffff (16-bit timer)
 *****************/
typedef uint16 Pwm_PeriodValue;

/********************************************************
 * @brief kiểu dữ liệu mô tả giá trị clock chia nhỏ không ảnh hưởng hardware
 * @details được sử dụng để kiểm tra giá trị clock gốc có đúng không dựa trên lý thuyết
 *         ,có thể cấu hình mặc định là không chia
 *****************/

typedef enum Pwm_ClockDivType
{
    PWM_CLOCK_DIV_1 = 0U,
    PWM_CLOCK_DIV_2,
    PWM_CLOCK_DIV_4
} Pwm_ClockDivType;

/********************************************************
 * @brief kiểu dữ liệu mô tả giá trị đếm lại của timer
 * @details được sử dụng để xác định giá trị mốc trong khoảng từ 0x00 - 0xff
 *         sẽ được bắt đầu đếm lại mỗi khi có 1 update event được kích hoạt
 *         giá trị này chỉ có hiệu lực trên TIM1 và TIM8. Có thể được cài đặt
 *         mặc định là 0 cho các timer còn lại
 *****************/
typedef uint16 Pwm_RepetitionCnt;

#endif