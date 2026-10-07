#include "IoHwAb_Pwm.h"
#include "Pwm.h"

/* Lưu trữ con trỏ cấu hình nội bộ sau khi Init thành công */
static const IoHwAb_PwmConfigType *IoHwAb_LocalConfigPtr = NULL_PTR;

/* Hàm Callback/Notification mẫu phục vụ NHIỆM VỤ 4 */
void IoHwAb_Pwm_Notification_Channel1(void)
{
    /* Thực hiện hành động nghiệp vụ khi một chu kỳ xung của Channel 1 kết thúc */
    /* Ví dụ: Đọc dữ liệu cảm biến đồng bộ với chu kỳ xung */
}

/* --- HÀM KHỞI TẠO TẦNG TRỪU TƯỢNG PHẦN CỨNG --- */
void IoHwAb_PwmInit(const IoHwAb_PwmConfigType *ConfigPtr)
{
    uint8 index;
    IoHwAb_LocalConfigPtr = ConfigPtr;

    /* Quét qua toàn bộ các kênh được cấu hình trong hệ thống */
    for (index = 0U; index < IoHwAb_LocalConfigPtr->CfgID_Count; index++)
    {
        const IoHwAb_PwmChannelConfigType *ChannelCfg = &(IoHwAb_LocalConfigPtr->ChannelConfigPtr[index]);

        /* NHIỆM VỤ 3: THIẾT LẬP TRẠNG THÁI AN TOÀN BAN ĐẦU (SAFE STATE) */
        /* Ghi trực tiếp giá trị an toàn xuống MCAL trước khi cho phép thiết bị bên ngoài chạy */
        Pwm_SetDutyCycle(ChannelCfg->McalChannelId, ChannelCfg->SafeStateDuty);

        /* NHIỆM VỤ 2: KHỞI TẠO LINH KIỆN NGOẠI VI KẾT NỐI VỚI CHÂN PWM */
        /* Nếu kênh có cấu hình chân kích hoạt (Enable Pin) cho IC Driver bên ngoài */
        /* NHIỆM VỤ 4: ĐĂNG KÝ CÁC HÀM CALLBACK / NOTIFICATION */
    }
}

/* --- API DÀNH CHO TẦNG ỨNG DỤNG (RTE) GỌI --- */
void IoHwAb_Pwm_SetDuty(IoHwAb_PwmIdType ChannelId, Pwm_DutyCycleType DutyValue)
{
    uint8 index;

    if (IoHwAb_LocalConfigPtr != NULL_PTR)
    {
        /* Tìm kiếm Kênh phần cứng MCAL tương ứng từ ID Kênh logic của Ứng dụng */
        for (index = 0U; index < IoHwAb_LocalConfigPtr->CfgID_Count; index++)
        {
            if (IoHwAb_LocalConfigPtr->ChannelConfigPtr[index].LogicChannelId == ChannelId)
            {
                Pwm_ChannelType mcalCh = IoHwAb_LocalConfigPtr->ChannelConfigPtr[index].McalChannelId;

                /*Ánh xạ giá trị Duty từ logic trong khoảng 0 đến 100 sang thô*/
                Pwm_PeriodValue rawDuty = (uint16)(((float)DutyValue / 100) * g_Pwm_Config.PeriodVal);

                /* Giới hạn giá trị Duty không vượt quá Period */
                if (rawDuty > g_Pwm_Config.PeriodVal)
                {
                    rawDuty = g_Pwm_Config.PeriodVal; // Giới hạn giá trị Duty không vượt quá Period
                }
                /* Chuyển tiếp giá trị Duty cần điều khiển xuống driver MCAL */
                Pwm_SetDutyCycle(mcalCh, rawDuty);
                break;
            }
        }
    }
}
