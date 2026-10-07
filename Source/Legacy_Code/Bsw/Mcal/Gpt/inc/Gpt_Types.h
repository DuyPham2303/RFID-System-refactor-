/**
 * @file Gpt_Types.h
 * @brief Giao diện kiểu dữ liệu cấu hình GPT của MCAL.
 * @details Khai báo các kiểu dữ liệu GPT được sử dụng bởi
 *          tầng ứng dụng và tầng dịch vụ.
 * @req Mô-đun TIM của AUTOSAR MCAL; cấu hình phụ thuộc vào từng project.
 */
#ifndef Gpt_Types_H
#define Gpt_Types_H
#include "./Bsw/Services/Common/Std_Types.h"

/* Kiểu định danh cho nhóm general purpose timer được ánh xạ tới phần cứng */
typedef enum Gpt_GroupId
{
    GPT_GROUP_1 = 1U,
    GPT_GROUP_2,
    GPT_GROUP_3,
} Gpt_GroupId_Type;

/********************************************************
 * @brief kiểu dữ liệu mô tả giá trị tần số chia timer
 * @detail được sử dụng để xác định thời gian của 1 tick count, nhằm
 *         chia nhỏ tần số Clock đầu vào của bộ APB timer, phù hợp với
 *         nhu cầu xử lý của hệ thống. Giá trị nhập trong khoảng
 *         0x0000 - 0xffff (16-bit timer)
 *****************/
typedef uint16 Gpt_PrescalerValue;

/********************************************************
 * @brief kiểu dữ liệu mô tả chế độ đếm của timer
 * @detail được sử dụng để xác định cách thức mà timer sẽ đếm ra sao
 *****************/
typedef enum Gpt_CounterModeType
{
    GPT_COUNTER_MODE_UP = 0U,
    GPT_COUNTER_MODE_DOWN
} Gpt_CounterModeType;

/********************************************************
 * @brief kiểu dữ liệu mô tả giá trị đếm tràn của chu kỳ timer
 * @detail được sử dụng để xác định thời điểm reset giá trị đếm trong thanh ghi ARR
 *         giá trị nhập trong khoảng 0x0000 - 0xffff (16-bit timer)
 *****************/
typedef uint16 Gpt_PeriodValue;

/********************************************************
 * @brief kiểu dữ liệu mô tả giá trị clock chia nhỏ không ảnh hưởng hardware
 * @details được sử dụng để kiểm tra giá trị clock gốc có đúng không dựa trên lý thuyết
 *         ,có thể cấu hình mặc định là không chia
 *****************/

typedef enum Gpt_ClockDivType
{
    GPT_CLOCK_DIV_1 = 0U,
    GPT_CLOCK_DIV_2,
    GPT_CLOCK_DIV_4
} Gpt_ClockDivType;

/********************************************************
 * @brief kiểu dữ liệu mô tả giá trị đếm lại của timer
 * @details được sử dụng để xác định giá trị mốc trong khoảng từ 0x00 - 0xff
 *         sẽ được bắt đầu đếm lại mỗi khi có 1 update event được kích hoạt
 *         giá trị này chỉ có hiệu lực trên TIM1 và TIM8. Có thể được cài đặt
 *         mặc định là 0 cho các timer còn lại
 *****************/
typedef uint16 Gpt_RepetitionCnt;

/**
 * @brief Enum định danh các nguồn ngắt logic cho module GPT (TIM2 -> TIM5)
 */
typedef enum
{
    GPT_IRQ_SOURCE_UPDATE = 0U, // Ngắt tràn định kỳ (Update)
    GPT_IRQ_SOURCE_CC1,         // Ngắt Capture/Compare kênh 1
    GPT_IRQ_SOURCE_CC2,         // Ngắt Capture/Compare kênh 2
    GPT_IRQ_SOURCE_CC3,         // Ngắt Capture/Compare kênh 3
    GPT_IRQ_SOURCE_CC4,         // Ngắt Capture/Compare kênh 4
    GPT_IRQ_SOURCE_NOTSET
} Gpt_IrqSourceType;

/*Định nghĩa kiểu cho hàm callback của GPT*/
typedef void (*Gpt_notificationPtr)(void);

/**
 * @brief kiểu sữ liệu chuẩn hóa cấu hình cho các publish API
 * @details dữ liệu sử dụng cục bộ trong module mcal khi xử lý các tác vụ
 *          liên quan tới ngắt
 * @param [HwId] Id ánh xạ tới địa chỉ phần cứng timer dùng chung cho các publish API
 * @param [NotiId] Id định danh cho callback tương ứng
 * @param [Noti]   hàm callback đăng ký
 * @param [cmd]  cho phép/vô hiệu hóa cơ chế ngắt
 *
 */
typedef struct Gpt_CallbackConfig
{
    Gpt_notificationPtr Noti;
    Gpt_IrqSourceType flag;
} Gpt_CallbackConfigType;

#endif /* Gpt_Types_H */