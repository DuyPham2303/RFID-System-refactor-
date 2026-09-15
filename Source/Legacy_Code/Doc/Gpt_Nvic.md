Flow triển khai Interrupt demo cho Gpt module
__1. xác định trach nhiệm các API module__
Gpt_Init()
    cấu hình timer
    cấu hình NVIC (gọi API)
    enable interrupt

Gpt_Irq()
TIM2_IRQHandler()
    kiểm tra cờ ngắt
    xóa cờ ngắt
    gọi Notification tương ứng

__2. Xác định toàn cảnh luồng gọi API__
Application task
    |
CallbackM.c 
    |
Gpt_EnableNotification / Gpt_RegisterApiCallback
    |
GPT MCAL
    |
IRQ handler

**trách nhiệm tổng quát module CallbackM**
`đăng ký callback : `
Gpt_RegisterApiCallback(GPT_CFG_LED_ID,BlinkLed_Task);

__3. Xác định overall architecture__
Bsw/Mcal/Gpt
    Gpt_Init()
    Gpt_RegisterApiCallback()
    TIMx_IRQHandler()
    Gpt_EnableNotification()

Bsw/Mcal/Adc
    Adc_Init()
    Adc_RegisterNotification()
    ADC_IRQHandler()

Bsw/Services/Callback
    CallbackM_Init()
    đăng ký các callback application vào MCAL

Bsw/CDD
    chỉ dùng cho driver tùy biến không thuộc MCAL chuẩn

**3.1 trách nhiệm chi tiết module CallbackM**
`3.1.1 Định nghĩa service callback API`
static void BlinkLed_Task(void)
{
    /* Application behavior */
}

static void ControlServo_Task(void)
{
    /* Application behavior */
}
`3.1.2 Định nghĩ wrapper API để đăng ký chung`
void CallbackM_Init(void)
{
    /*định nghĩa chi tiết ở Mcal/Gpt_Cfg.c*/
    Gpt_RegisterApiCallback(
        GPT_CFG_LED_ID,
        BlinkLed_Task);

    Gpt_RegisterApiCallback(
        GPT_CFG_SERVO_ID,
        ControlServo_Task);
}
**3.2 Quá trình đăng ký và thực thi callback**

`3.2.1 Khởi tạo kiểu dữ liệu con trỏ hàm callback và khai báo
Gpt_NotificationType Notification;
`3.2.2 Bổ sung vào hàm đăng ký `
Std_ReturnType Gpt_RegisterApiCallback(
    Gpt_ConfigIdType ConfigId,
    Gpt_NotificationType Notification);
`3.2.3 GPT sẽ lưu callback vào bảng runtime:`
static Gpt_NotificationType Gpt_NotificationTable[GPT_CFG_COUNT];
`3.2.4 IRQ_hanlder được gọi khi ngắt xảy ra`
void TIM2_IRQHandler(void)
{
    if (Gpt_NotificationTable[GPT_CFG_LED_ID] != NULL_PTR)
    {
        Gpt_NotificationTable[GPT_CFG_LED_ID]();
    }
}

__4. Xác định cấu hình NVIC__

**4.1 overall architecture**
Gpt_Cfg.h / Gpt_Cfg.c
    IRQ channel
    priority
    enable flag

Gpt.c
    NVIC_Init()
    TIM_ITConfig()
    IRQ dispatch
**4.2 Details architecture**
Gpt_Cfg.c
    cấu hình 
    - Timer, Prescaler, Period, TimerId
    - IRQ channel, NVIC priority, disable/enable IT

CallbackM.c
    đăng ký BlinkLed_Task, ControlServo_Task

Gpt.c / Gpt_Irq.c
    lưu cấu hình NVIC
    xử lý IRQ TIMx_IRQHandler()
    gọi callback đã đăng ký

`Ví dụ xử lý IRQ -> gọi callback đăng ký qua con trỏ hàm`
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

        if (Gpt_NotificationTable[GPT_CFG_LED_ID] != NULL_PTR)
        {
            Gpt_NotificationTable[GPT_CFG_LED_ID]();
        }
    }
}

__5.Tóm lược trách nhiệm module trước khi triển khai chi tiết__

MCAL:
    cấu hình peripheral
    cấu hình NVIC
    xử lý IRQ
    xóa cờ ngắt
    gọi notification

Callback service:
    đăng ký callback của application

__6. Thiết kế cấu hình NVIC và callback API__

**6.1 định nghĩa cấu hình và bổ sung vào cấu hình tổng thể**

`6.1.1 cấu hình NVIC`
typedef struct
{
    Gpt_IrqSourceType Source; /*triển khai chi tiết ở section 7 bên dưới*/
    uint8 PreemptionPriority;
    uint8 SubPriority;
    boolean Enable;
} Mcal_IrqConfigType;
`6.1.2 Bổ sung váo cấu hình chính`

typedef struct
{
    Gpt_GroupType HwTimerId;
    Gpt_PrescalerValue PresVal;
    Gpt_PeriodValue PeriodVal;
    const Mcal_IrqConfigType* IrqConfigPtr;
} Gpt_ChannelConfigType;

**6.2 Thiết kế callback Function pointer**

`6.2.1 Định nghĩa`
typedef void (*Gpt_NotificationType)(void);
static Gpt_NotificationType Gpt_NotificationTable[GPT_CFG_COUNT];

`6.2.2 đăng ký`
Std_ReturnType Gpt_RegisterApiCallback(
    Gpt_ConfigIdType ConfigId,
    Gpt_NotificationType Notification);

`6.2.3 Mô hính hóa luồng thực thi`
Gpt_Cfg.c
    cấu hình timer + NVIC

CallbackM.c
    đăng ký callback application

Gpt_Irq.c
    xử lý IRQ
    xóa cờ ngắt
    gọi Gpt_NotificationTable[]

**6.3 Hệ thống hóa toàn bộ kiến trúc**

`6.3.1 Trách nhiệm Ngắt của từng module`
Module	Cần IRQ config?
GPT	    Có
ADC	    Có nếu dùng conversion interrupt
USART	Có nếu dùng RX/TX interrupt
SPI	    Có nếu dùng interrupt
I2C	    Có nếu dùng event/error interrupt
PWM	    Không nhất thiết; chỉ cần nếu dùng update/compare interrupt
Port	Thường không; EXTI nên thuộc External Interrupt module
`6.3.2 Cấu trúc hoàn thiên Mcal`
Mcal/
    Common/
        Mcal_IrqTypes.h

    Gpt/
        Gpt_Cfg.h
        Gpt_Cfg.c
        Gpt.c
        Gpt.h
        Gpt_Irq.c
        Gpt_Irq.c
        Gpt_dtypes.h
__7. Quản lý ánh xạ các thông số cấu hình NVIC__
 **7.1 Dịnh nghĩe enum ánh xạ**
`7.1.1 chi tiết struct và enum`
/* Trong Mcu_IrqMapping.h hoặc Common */
typedef enum {
    MCAL_GPT_CH1_IRQ,
    MCAL_PWM_CH1_IRQ,
    // ... các nguồn ngắt logic toàn cục
} Mcu_IrqIdType;

// Bảng map tập trung toàn cục cho chip
static const IRQn_Type Mcu_IrqTable[] = {
    [MCAL_GPT_CH1_IRQ] = TIM2_IRQn,
    [MCAL_PWM_CH1_IRQ] = TIM3_IRQn,
};

typedef struct
{
    bool Enable;
    Mcu_IrqIdType IrqId;
    uint8 Priority;
    uint8 SubPriority;
} Gpt_IrqConfigType;
`7.1.2 Ap dụng thực té cấu hình`
[GPT_CFG_LED_ID] =
{
    .HwTimerId = GPT_GROUP_1,
    .PresVal = 36000U,
    .ClkDiv = GPT_CLOCK_DIV_1,
    .ModeCntType = GPT_COUNTER_MODE_UP,
    .PeriodVal = 500U,
    .RepCntVal = 0U,

    .IrqConfig =
    {
        .Enable = TRUE,
        .IrqId = MCAL_GPT_CH1_IRQ,
        .Priority = 0U,
        .SubPriority = 0U
    }
}
**Note** : Ở đây application Không biết TIM2_IRQn hay TIM1_UP_IRQn. (phụ thuộc vào thư viện phần cứng mcu)

 **7.2 Dịnh nghĩe API ánh xạ**
 `7.2.1 Logic ánh xạ`

Mapping trong Gpt_Mapping.c
---------------------------------------
#include "Gpt_Mapping.h"
#include "stm32f10x.h"

IRQn_Type Mcu_MapIrq(Mcu_IrqIdType Id)
{
        switch (Id)
        {
        case MCAL_GPT_CH1_IRQ:
            return TIM2_IRQn;

        case MCAL_GPT_CH2_IRQ:
            return TIM3_IRQn;
        default:
            return NULL;
            break;
        }
}
---------------------------------------
`7.2.2 Logic lưu trữ ánh xạ cấu hình`
Chuyển sang SPL trong Gpt.c
Chỉ Gpt.c mới biết NVIC_InitTypeDef:
---------------------------------------
static void Gpt_InitIrq(
    const Gpt_ChannelConfigType_s *Config)
{
    NVIC_InitTypeDef nvicConfig;

    if (Config->IrqConfig.Enable != TRUE ||
        Config->IrqConfig.Source == GPT_IRQ_NONE)
    {
        return;
    }

    nvicConfig.NVIC_IRQChannel =
        Gpt_MapIrq(
            Config->HwTimerId,
            Config->IrqConfig.Source);

    nvicConfig.NVIC_IRQChannelPreemptionPriority =
        Config->IrqConfig.Priority;

    nvicConfig.NVIC_IRQChannelSubPriority =
        Config->IrqConfig.SubPriority;

    nvicConfig.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&nvicConfig);
}
---------------------------------------
 **7.3 Quản lý cấu hình NVIC Group**

`7.3.1 Triển khai cấu hình chung`
Common/Mcal_IrqTypes.h
---------------------------------------
typedef enum
{
    MCAL_IRQ_GROUP_0 = 0U,
    MCAL_IRQ_GROUP_1,
    MCAL_IRQ_GROUP_2,
    MCAL_IRQ_GROUP_3,
    MCAL_IRQ_GROUP_4
} Mcal_IrqPriorityGroupType;
---------------------------------------
Common/Mcal_Irq.c
---------------------------------------
void Mcal_Irq_Init()
{
    NVIC_PriorityGroupConfig();
}
---------------------------------------
Mcal_MapPriorityGroup() là nơi duy nhất biết các macro SPL:
---------------------------------------
NVIC_PriorityGroup_0
NVIC_PriorityGroup_1
...
---------------------------------------
`7.3.2 vị trí gọi`
EcuM_Init()
    Mcal_Irq_Init()
        NVIC_PriorityGroupConfig(...)
    Port_Init()
    Gpt_Init()
    Adc_Init()
    ...



__8. Thứ tự triển khai demo__

**8.1 Module sequence**
1. Gpt_Init()
2. Gpt_RegisterApiCallback()
3. NVIC_Init()
4. TIM_ITConfig(..., ENABLE)
5. TIM_Cmd(..., ENABLE)

**8.2 API Sequence**
Gpt_Init()
    gọi TIM_TimeBaseInit()
    gọi NVIC_Init()
    gọi TIM_ITConfig()

Spi_Init()
    gọi SPI_Init()
    gọi NVIC_Init()
    gọi SPI_I2S_ITConfig()

Port_Init()
    gọi GPIO_Init()
    không cần NVIC nếu chỉ cấu hình GPIO









