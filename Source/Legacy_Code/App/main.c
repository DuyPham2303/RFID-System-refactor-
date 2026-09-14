#include "Dio/Dio.h"
#include "Port/Port.h"
#include "Pwm/Pwm.h"
#include "Gpt/Gpt.h"
#include "Delay/Bsw_Delay.h"
#include "platform/Board_Clock.h"
#include "Testcase_Types.h"

/*chọn id của loại ngoại vi cần demo testcase*/
TC_IdType_e ty = TC_PWM_ID;

#define F_PWM_4HZ PERIOD_UPDATE_250MS

#define DUTY_25_PERCENT ((Pwm_DutyCycleType)(F_PWM_4HZ * 0.25))
#define DUTY_50_PERCENT ((Pwm_DutyCycleType)(F_PWM_4HZ * 0.5))
#define DUTY_75_PERCENT ((Pwm_DutyCycleType)(F_PWM_4HZ * 0.75))
#define DUTY_100_PERCENT ((Pwm_DutyCycleType)F_PWM_4HZ)

int main()
{
	Board_PeripheralsClock_Init();
	Port_Init(&PortCfg);
	Pwm_Init(&Pwm_Config);
	Gpt_Init(&Gpt_Config);

	/*đếm số lượng test case để khởi tạo bảng giám sát trạng thái xử lý*/
	uint8 keycnt = TestCaseTotalCount(ty);

	/*khởi tạo bảng đồ lưu trử TC và trạng thái xử lý*/
	TestCaseMgr_s TC_Mgr[keycnt]; // mảng chứa vừa đủ số lượng phần tử cần quản lý

	/*Gán dữ liệu mặc định cho từng phần tử*/
	TC_TableInitParam(TC_Mgr, keycnt);

	uint16 duty = 0;
	/*lặp qua từng testcase để xử lý và cập nhật keyname và trạng thái kết quả*/
	while (1)
	{
		if (duty < DUTY_25_PERCENT)
		{
			Pwm_SetDutyCycle(PWM_CHANNEL_1, duty);
			duty += 100;
		}
		else
		{
			duty = 0;
		}
		// uint8 index = 0;
		//  for (; index < keycnt; index++)
		//  {
#if TESTCASE_TYPE == TESTCASE_DIO
		if (index == 0)
		{
			Dio_WriteChannel(DIO_CHANNEL_A3, STD_HIGH);
			if (Dio_ReadChannel(DIO_CHANNEL_A3) == STD_HIGH)
				TC_Mgr[index].st = PASS;
			else
				TC_Mgr[index].st = FAIL;
			Dio_WriteChannel(DIO_CHANNEL_A3, STD_LOW);
		}
		else if (index == 1)
		{
			Dio_WritePort(DIO_PORT_A, 0xA5); // 0b 1010 0101
			if (Dio_ReadPort(DIO_PORT_A) == 0xA5)
				TC_Mgr[index].st = PASS;
			else
				TC_Mgr[index].st = FAIL;
			Dio_WritePort(DIO_PORT_A, 0x00);
		}
		else if (index == 2)
		{

			/*Cấu hình Group DIO gồm PA2-PA5 (4 bit) */
			const Dio_ChannelGroupType EngineCtrlGroup =
				{
					.mask = DIO_GET_PIN_MASK(DIO_CHANNEL_A2) |
							DIO_GET_PIN_MASK(DIO_CHANNEL_A3) |
							DIO_GET_PIN_MASK(DIO_CHANNEL_A4) |
							DIO_GET_PIN_MASK(DIO_CHANNEL_A5),
					.offset = DIO_OFFSET_POSITION(DIO_CHANNEL_A2),
					.HwPortId = DIO_PORT_A};

			Dio_WritePort(DIO_PORT_A, 0xF0);			  // 0b 1111 0000
			Dio_WriteChannelGroup(&EngineCtrlGroup, 0x5); // 0b 1111 0000 | (101 << 2)

			if (Dio_ReadPort(DIO_PORT_A) == 0xD4) // 0 b1111 0100
				TC_Mgr[index].st = PASS;
			else
				TC_Mgr[index].st = FAIL;
			Dio_WritePort(DIO_PORT_A, 0x00);
		}
		else
		{
			Dio_WriteChannel(DIO_CHANNEL_A1, STD_LOW);

			Dio_FlipChannel(DIO_CHANNEL_A1);

			uint8 val1 = Dio_ReadChannel(DIO_CHANNEL_A1);

			Dio_FlipChannel(DIO_CHANNEL_A1);

			uint8 val2 = Dio_ReadChannel(DIO_CHANNEL_A1);

			if (val1 == 1 && val2 == 0)
				TC_Mgr[index].st = PASS;
			else
				TC_Mgr[index].st = FAIL;
		}
#elif TESTCASE_TYPE == TESTCASE_PWM
		// if (index == 0)
		// {
		// 	Pwm_SetDutyCycle(PWM_CHANNEL_1, DUTY_25_PERCENT);
		// }
		// else if (index == 1)
		// {
		// 	Pwm_SetDutyCycle(PWM_CHANNEL_2, DUTY_50_PERCENT);
		// }
		// else if (index == 2)
		// {
		// 	Pwm_SetDutyCycle(PWM_CHANNEL_3, DUTY_75_PERCENT);
		// }
		// else
		// {
		// 	Pwm_SetDutyCycle(PWM_CHANNEL_4, DUTY_100_PERCENT);
		// }
#endif
		Dio_FlipChannel(DIO_CHANNEL_C13);
		Bsw_DelayMs(250);
	}
	return 0;
}
