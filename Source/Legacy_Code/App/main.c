#include "ClkCfg.h"
#include "Systick.h"
#include "PinCfg.h"
#include "display.h"
#include "control_output.h"
#include "RFID_Scan.h"
#include "SdCard_Read.h"

int main()
{
	RCC_Cfg();		/* Kich hoat clock gpio,peripherals */
	SysTick_Init(); /* Kich hoat ngat system 1ms va khoi tao timer delay*/
	/* Khoi tao cac ngoai vi */
	SignalOutput_ModuleInit(); /* Khoi tao servo va led */
	LCD_ModuleInit();		   /* Khoi tao man hinh hien thi */
	RFID_ModuleInit();		   /* Khoi tao cam bien doc the RFID */
	SDcard_ModuleInit();	   /* Khoi tao module doc the nho doc du lieu luu trong the vao RAM */
	while (1)
	{

#ifndef CODE
		/* Flow hoat dong cua chuong trinh */
		/* Khoi dong he thong */
		/*
		 * + doc du lieu tu sd card
		 * + thong bao den lcd
		 * -> hien thi trang thai doc thanh cong neu data available
		 * -> hien thi trang thai doc that bai neu data empty -> shutdown program (thoat loop)
		 */
		/* Cho user quet the */
		/*
		 * + kiem tra lien tuc viec quet the
		 * + doc ra noi dung the
		 * + kiem tra noi dung the == noi dung luu trong database
		 */
		/* Xu ly valid rfid card */
		/*
		 * + hien thi trang thai thanh cong va thong tin cua the -> ten + bien so xe
		 * + OPENING barrier + blink led (hien thi trang thai tren lcd)
		 * + suspend viec quet the
		 * + wait 5s cho xe di qua
		 * +CLOSING barrier + blink led (hien thi trang thai tren lcd)
		 */
		/* Xu ly invalid rfid card */
		/*
		 * + thong bao the khong co trong he thong
		 */
		switch (AppModeUpdate())
		{
		case INIT_MODE:
			display_Init();
			break;
		case IDLE_MODE:
			/* Function display idle status -> no card found */
			display_Idle();
			/* Function scan rfid (interrupt) */
			/*
			 * - read card then check
			 *   + content == database -> jump to RUN_MODE
			 *   + content != databse -> jump to ERROR_MODE
			 */
			RFID_Scan();
			break;
		case RUN_MODE:
			/* Function display barrier is opening / closing */
			display_Run();
			/* Function OPENING /CLOSING barrier */
			/*
			 * - OPENING for 3s
			 * - wait for 5s
			 * -CLOSING for 3s
			 * - when doneCLOSING -> jump to WAIT_MODE
			 */
			Barrier_Control();
			/* Function blink led (interrupt) */
			/*
			 * - only blink when barrier is closing/opening
			 * - off when barrier is idling for 5s after opening
			 */
			Led_Control();
			break;
		case ERROR_MODE:
			/* Function display error code when data in card is not registered */
			/*
			 * - display for 2s then jump to WAIT_MODE
			 */
			display_Error();
			break;
		default:
			display_Fail();
			break;
		}
		return 0;
#endif
	}
}
