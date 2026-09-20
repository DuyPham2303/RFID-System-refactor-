/**
 * @file Gpt_dtypes.h
 * @brief Giao diện kiểu dữ liệu cấu hình GPT của MCAL.
 * @details Khai báo các kiểu dữ liệu GPT được sử dụng bởi
 *          tầng ứng dụng và tầng dịch vụ.
 * @req Mô-đun TIM của AUTOSAR MCAL; cấu hình phụ thuộc vào từng project.
 */
#ifndef GPT_DTYPES_H
#define GPT_DTYPES_H
#include "./Bsw/Services/Common/Std_Types.h"

/* Kiểu định danh cho nhóm general purpose timer được ánh xạ tới phần cứng */
typedef enum Gpt_GroupId
{
    GPT_GROUP_1 = 0U,
    GPT_GROUP_2,
    GPT_GROUP_3,
    GPT_GROUP_4,
    GPT_MAX_GROUP
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
 * @detail được sử dụng để kiểm tra giá trị clock gốc có đúng không dựa trên lý thuyết
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
 * @detail được sử dụng để xác định giá trị mốc trong khoảng từ 0x00 - 0xff
 *         sẽ được bắt đầu đếm lại mỗi khi có 1 update event được kích hoạt
 *         giá trị này chỉ có hiệu lực trên TIM1 và TIM8. Có thể được cài đặt
 *         mặc định là 0 cho các timer còn lại
 *****************/
typedef uint16 Gpt_RepetitionCnt;

/**
 * @brief danh sách Id ánh xạ tới loại Callback API, được tầng service sử dụng
 *
 */
typedef enum Gpt_Idnoti
{
    GPT_UPDATE_1S = 0U,
    GPT_MAX_NOTI
} Gpt_IdnotiType;

/*Định nghĩa kiểu cho hàm callback của GPT*/
typedef void (*Gpt_notificationPtr)(void);

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
    GPT_IRQ_SOURCE_MAX
} Gpt_IrqSourceType;

/*kiểu dữ liệu cấu hình con trỏ callback và eventflag*/
typedef struct
{
    Gpt_notificationPtr cb; /*callback function*/
    Gpt_IrqSourceType flag; /*cờ chỉ định nguồn xử lý ngắt*/
} Gpt_IrqConfigNotiType;

#endif /* GPT_DTYPES_H */