/**
 * @file Adc_Types.h
 * @brief Định nghĩa các kiểu dữ liệu sử dụng bởi mô-đun ADC.
 * @details Khai báo các kiểu dữ liệu cho kênh ADC, nhóm ADC,
 *          chế độ chuyển đổi, nguồn kích hoạt, thời gian lấy mẫu,
 *          căn chỉnh kết quả và bộ chia xung ADC.
 * @req Các kiểu dữ liệu được xây dựng dựa trên khái niệm ADC của
 *      AUTOSAR MCAL và yêu cầu riêng của dự án.
 */
#ifndef ADC_AUTOSARTYPE_H
#define ADC_AUTOSARTYPE_H
#include "Std_Types.h"

/********************************************************
 * @brief kieu du lieu dinh nghia ID cua cac kenh ADC
 * @detail duoc su dung de danh so cho cac kenh ADC
 * @note pham vi gia tri 0 - 17 kenh ADC tren stm32f103
 *****************/
typedef uint8 Adc_ChannelType;

#define ADC_AUTOSAR_CHANNEL_0 (Adc_ChannelType)(0)
#define ADC_AUTOSAR_CHANNEL_1 (Adc_ChannelType)(1)
#define ADC_AUTOSAR_CHANNEL_2 (Adc_ChannelType)(2)
#define ADC_AUTOSAR_CHANNEL_3 (Adc_ChannelType)(3)
#define ADC_AUTOSAR_CHANNEL_4 (Adc_ChannelType)(4)
#define ADC_AUTOSAR_CHANNEL_5 (Adc_ChannelType)(5)
#define ADC_AUTOSAR_CHANNEL_6 (Adc_ChannelType)(6)
#define ADC_AUTOSAR_CHANNEL_7 (Adc_ChannelType)(7)
#define ADC_AUTOSAR_CHANNEL_8 (Adc_ChannelType)(8)
#define ADC_AUTOSAR_CHANNEL_9 (Adc_ChannelType)(9)
#define ADC_AUTOSAR_CHANNEL_10 (Adc_ChannelType)(10)
#define ADC_AUTOSAR_CHANNEL_11 (Adc_ChannelType)(11)
#define ADC_AUTOSAR_CHANNEL_12 (Adc_ChannelType)(12)
#define ADC_AUTOSAR_CHANNEL_13 (Adc_ChannelType)(13)
#define ADC_AUTOSAR_CHANNEL_14 (Adc_ChannelType)(14)
#define ADC_AUTOSAR_CHANNEL_15 (Adc_ChannelType)(15)
#define ADC_AUTOSAR_CHANNEL_16 (Adc_ChannelType)(16)
#define ADC_AUTOSAR_CHANNEL_17 (Adc_ChannelType)(17)

/********************************************************
 * @brief kieu du lieu dinh nghia ID cua cac group ADC
 * @detail duoc su dung de danh so cho cac group ADC
 * @note so luong toi da la 3 group ADC
 *****************/
typedef uint8 Adc_GroupType;

#define ADC_GROUP_1 (Adc_GroupType)(1)
#define ADC_GROUP_2 (Adc_GroupType)(2)
#define ADC_GROUP_3 (Adc_GroupType)(3)

/********************************************************
 * @brief kieu du lieu mo ta tra tri chuyen doi adc
 * @detail duoc su dung de doc gia tri chuyen doi tu group ADC
 *****************/
typedef sint16 Adc_ValueGroupType;

/********************************************************
 * @brief kieu du lieu mo ta tang so chia clock cua ngoai vi ADC
 * @detail duoc su dung de chon gia tri bo chia tan so clock cap cho ADC
 *****************/
typedef uint8 Adc_PrescaleType;

#define ADC_AUTOSAR_PRESCALE_DIV1 ((Adc_PrescaleType)0)
#define ADC_AUTOSAR_PRESCALE_DIV2 ((Adc_PrescaleType)1)
#define ADC_AUTOSAR_PRESCALE_DIV4 ((Adc_PrescaleType)2)
#define ADC_AUTOSAR_PRESCALE_DIV6 ((Adc_PrescaleType)3)
#define ADC_AUTOSAR_PRESCALE_DIV8 ((Adc_PrescaleType)4)

/********************************************************
 * @brief kieu du lieu mo ta thoi gian chuyen doi gia tri adc
 * @detail duoc su dung de cai dat thoi gian chuyen doi tu gia tri adc sang digital
 *****************/
typedef uint16 Adc_ConversionTimeType;

/********************************************************
 * @brief kieu du lieu mo ta cac chu ky lay mau adc
 * @detail duoc su dung de cai dat thoi gian lay mau gia tri adcs
 *****************/

typedef uint8 Adc_SamplingTimeType;

#define ADC_AUTOSAR_SAMPLING_1CYCLE ((Adc_SamplingTimeType)1)
#define ADC_AUTOSAR_SAMPLING_7CYCLE ((Adc_SamplingTimeType)2)
#define ADC_AUTOSAR_SAMPLING_13CYCLE ((Adc_SamplingTimeType)3)
#define ADC_AUTOSAR_SAMPLING_28CYCLE ((Adc_SamplingTimeType)4)
#define ADC_AUTOSAR_SAMPLING_41CYCLE ((Adc_SamplingTimeType)5)
#define ADC_AUTOSAR_SAMPLING_55CYCLE ((Adc_SamplingTimeType)6)
#define ADC_AUTOSAR_SAMPLING_71CYCLE ((Adc_SamplingTimeType)7)
#define ADC_AUTOSAR_SAMPLING_239CYCLE ((Adc_SamplingTimeType)8)

/********************************************************
 * @brief kieu du lieu mo ta do phan giai cua bo adc
 * @detail duoc su dung de cai dat do phan giai cho bo adc
 *****************/
typedef uint8 Adc_ResolutionType;

#define ADC_RESOLUTION_12BIT ((Adc_ResolutionType)1)

/********************************************************
* @brief  kieu du lieu liet ke cac trang thai chuyen doi ADC
* @detail duoc su dung de xac dinh trang thai cua ADC, giup
				  kiem tra trang thai hoat dong hien tai
* @note  	trang thai ADC co the thay doi lan luot trong qua trinh chuyen doi
*****************/
typedef enum
{

	/********************************************************
	 * @brief  qua trinh chuyen doi cua ADC chua duoc bat dau
	 * @detail mo ta trang thai khong co gia tri adc nao duoc tra ve
	 *****************/
	ADC_IDLE = 0x00,
	/********************************************************
	 * @brief  qua trinh chuyen doi cua ADC dang duoc thuc thi
	 * @detail mo ta gia tri adc chua duoc tra ve cho den khi qua trinh chuyen doi hoan tat
	 *****************/
	ADC_BUSY = 0x01,
	/********************************************************
	 * @brief  gia tri chuyen doi adc da duoc lam tron
	 * @detail mo ta tat cac kenh adc co the doc ve gia tri chuyen doi
	 *****************/
	ADC_COMPLETED = 0x02,
	/********************************************************
	 * @brief  ket qua chuyen doi adc da duoc ghi vao buffer
	 * @detail mo ta so lan lay mau co the doc ve tren kenh adc tuong ung
	 *****************/
	ADC_STREAM_COMPLETED = 0x03
} Adc_StatusType;

/********************************************************
 * @typedef enum Adc_TriggerSourceType
 * @brief  dinh nghia cac gia tri enum xac dinh nguon kich hoat qua trinh chuyen doi adc
 * @detail ADC co the duoc kich hoat boi hardware /software tuy vao gia tri lua chon
 *****************/
typedef enum
{
	ADC_TRIGG_SRC_SW = 0x00,
	ADC_TRIGG_SRC_HW = 0x01
} Adc_TriggerSourceType;

/********************************************************
 * @typedef enum Adc_GroupConvModeType
 * @brief  dinh nghia cac gia tri enum xac dinh che do chuyen doi adc
 *****************/
typedef enum
{
	/********************************************************
	 * @brief adc chi chuyen doi 1 lan khi co su kien kich hoat
	 * @note 	- doi voi kich hoAUTOSAR adc bang software,
	 *				  	viec chuyen doi 1 lan co the stop bang cach goi softwAUTOSARe API
	 *					- doi voi kich hoat adc bang hardware,
	 *						viec chuyen doi 1 lan co the stop bang cach disable su kien kich hoat boi hAUTOSARdwAUTOSARe
	 *****************/
	ADC_CONV_MODE_ONESHOT = 0x00,

	/********************************************************
	* @brief chuyen doi lien tuc gia tri adc
	* @note  - che do nay chi co san doi voi viec kich hoat group adc bang softwAUTOSARe
					 - qua trinh chuyen doi stop bang cah goi softwAUTOSARe API
*****************/
	ADC_CONV_MODE_CONTINUOUS = 0x01
} Adc_GroupConvModeType;

/********************************************************
	* @brief kieu du lieu dinh nghia muc uu tien cua cac kenh adc
	* @detai muc uu tien chuyen doi adc cua cac kenh adc
					 co the duoc chon trong khoang tu 0 - 255
*****************/
typedef uint8 Adc_GroupPriorityType;

/********************************************************
 * @brief  kieu du lieu xac dinh so luong mau trong 1 chuoi chuyen doi adc
 * @detail gia tri phu thuoc vao so luong kenh adc quet duoc trong 1 chu ky chuyen doi
 * @note   - gia tri cho phep : 1 - 16 (so luong adc su dung trong scan mode)
 *					- neu su dung single mode, gia tri la 1
 * @wAUTOSARning ghi gia tri lon hon 16 co the gay loi phan cung
 *****************/
typedef uint8 Adc_StreamNumSampleType;

/********************************************************
 * @brief kieu du lieu xac dinh che do luu tru du lieu cua adc khi su dung stream buffer
 * @detai quyet dinh cach du lieu cua nhieu mau adc duoc luu vao buffer
 *				 khi co nhieu lan chuyen doi lien tiep
 * @note - ADC_STREAM_BUFFER_LINEAUTOSAR: luu tru du lieu theo tuyen tinh, dung luu tru khi day
 *				- ADC_STREAM_BUFFER_CIRCULAUTOSAR: Luu tru du lieu theo vong tron, ghi de du lieu cu khi day
 * @wAUTOSARning chon gia tri khong phu hop co the dan den mat du lieu neu buffer bi ghi de
 *****************/
typedef enum
{
	ADC_STREAM_BUFFER_LINEAUTOSAR,
	ADC_STREAM_BUFFER_CIRCULAUTOSAR
} Adc_StreamBufferModeType;

/********************************************************
	* @brief kieu du lieu xac dinh cach truy cap gia tri adc khi lam viec voi group adc
	* @detai quyet dinh cach doc gia tri adc sau khi hoan tat chuyen doi
	* @note  - ADC_ACCESS_MODE_SINGLE: doc tung gia tri adc rieng le
					 - ADC_ACCESS_MODE_STREAMING: doc tat ca gia tri adc cua group cung luc
	*@wAUTOSARning chon gia tri khong phu hop co the dan den mat du lieu neu buffer bi ghi de
*****************/
typedef enum
{
	ADC_ACCESS_MODE_SINGLE,
	ADC_ACCESS_MODE_STREAMING
} Adc_GroupAccessModeType;

/**
 * @brief   dinh nghia nguon kich hoat phan cung cho adc
 * @details xac dich nguon tin hieu ben ngoai hoac ben trong duoc dung de kich hoat qua trinh chuyen doi adc
 * @note    - ADC_HW_TRIGGER_RISING_EDGE: kich hoat khi co tin hieu canh len
 *          - ADC_HW_TRIGGER_FALLING_EDGE: kich hoat khi co tin hieu canh xuong
 *          - ADC_HW_TRIGGER_BOTH_EDGES: kich hoat khi tin hieu that doi o 2 canh
 * @wAUTOSARning cau hinh khong dung co the dan den adc khong hoat dong hoac do sai gia tri
 */
typedef enum
{
	ADC_HW_TRIGGER_RISING_EDGE,
	ADC_HW_TRIGGER_FALLING_EDGE,
	ADC_HW_TRIGGER_BOTH_EDGES
} Adc_HwTriggerSignalType;

/**
 * @brief   dinh nghia loai timer dung de kich haot phan cung cho adc
 * @details adc co the duoc kich hoat tu dong thong qua timer
 * @note    - ADC_HW_TRIGGER_TIMER1: su dung timer1 lam nguon kich hoat adc
 *          - ADC_HW_TRIGGER_TIMER2: su dung timer2 lam nguon kich hoat adc
 *          - ADC_HW_TRIGGER_TIMER3: su dung timer3 lam nguon kich hoat adc
 *          - ADC_HW_TRIGGER_TIMER4: su dung timer4 lam nguon kich hoat adc
 * @wAUTOSARning cau hinh khong chinh xac co lam adc khong lay mau dung chu ky
 */
typedef uint8 Adc_HwTriggerTimerType;

// Macro used for Regular channels
#define ADC_AUTOSAR_EXT_TRIG_CONV_T1_CC1 ((Adc_HwTriggerTimerType)0)			 /*!< For ADC1 and ADC2 */
#define ADC_AUTOSAR_EXT_TRIG_CONV_T1_CC2 ((Adc_HwTriggerTimerType)1)			 /*!< For ADC1 and ADC2 */
#define ADC_AUTOSAR_EXT_TRIG_CONV_T2_CC2 ((Adc_HwTriggerTimerType)2)			 /*!< For ADC1 and ADC2 */
#define ADC_AUTOSAR_EXT_TRIG_CONV_T3_TRGO ((Adc_HwTriggerTimerType)3)			 /*!< For ADC1 and ADC2 */
#define ADC_AUTOSAR_EXT_TRIG_CONV_T4_CC4 ((Adc_HwTriggerTimerType)4)			 /*!< For ADC1 and ADC2 */
#define ADC_AUTOSAR_EXT_TRIG_CONV_Ext_IT11_TIM8_TRGO ((Adc_HwTriggerTimerType)5) /*!< For ADC1 and ADC2 */

#define ADC_AUTOSAR_EXT_TRIG_CONV_T1_CC3 ((Adc_HwTriggerTimerType)6) /*!< For ADC1, ADC2 and ADC3 */
#define ADC_AUTOSAR_EXT_TRIG_CONV_None ((Adc_HwTriggerTimerType)7)	 /*!< For ADC1, ADC2 and ADC3 */

#define ADC_AUTOSAR_EXT_TRIG_CONV_T3_CC1 ((Adc_HwTriggerTimerType)8)   /*!< For ADC3 only */
#define ADC_AUTOSAR_EXT_TRIG_CONV_T2_CC3 ((Adc_HwTriggerTimerType)9)   /*!< For ADC3 only */
#define ADC_AUTOSAR_EXT_TRIG_CONV_T8_CC1 ((Adc_HwTriggerTimerType)10)  /*!< For ADC3 only */
#define ADC_AUTOSAR_EXT_TRIG_CONV_T8_TRGO ((Adc_HwTriggerTimerType)11) /*!< For ADC3 only */
#define ADC_AUTOSAR_EXT_TRIG_CONV_T5_CC1 ((Adc_HwTriggerTimerType)12)  /*!< For ADC3 only */
#define ADC_AUTOSAR_EXT_TRIG_CONV_T5_CC3 ((Adc_HwTriggerTimerType)13)  /*!< For ADC3 only */

/**
 * @brief   kieu du lieu xac dinh cach can chinh du kieu adc trong thanh ghi ket qua
 * @note    - ADC_ALIGN_RIGHT: can chinh cac bit ve ben phai
 *          - ADC_ALIGN_LEFT: can chinh cac bit ve ben trai
 * @wAUTOSARning viec chon sai cach can chinh co the lam du lieu adc bi sai khi doc gia tri tu thanh ghi
 */
typedef enum
{
	ADC_ALIGN_RIGHT,
	ADC_ALIGN_LEFT
} Adc_ResultAlignmentType;

#endif