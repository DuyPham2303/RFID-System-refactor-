#include "display.h"
#include "lcd.h"
#include "usbttl.h"
#include "Systick.h"

ModeStatusCfg_s ModeStatusCfg = {.apptype = INIT_FAIL,
								 .errortype = NORMAL,
								 .IsLcdModeUpdate = false};
static LCD lcd;
AppMode_e AppModeUpdate()
{
	return ModeStatusCfg.apptype;
}
void LCD_ModuleInit()
{
	LCD_I2C_Init(&lcd, 0x61, 16, 2);
}
void display_Init()
{
	// update lcd display
	LCD_I2C_SetCursor(&lcd, 4, 0);
	LCD_I2C_WriteString(&lcd, "KHOI TAO");
	LCD_I2C_SetCursor(&lcd, 3, 1);
	LCD_I2C_WriteString(&lcd, "THANH CONG");

	// Update lcd mode
	ModeStatusCfg.apptype = IDLE_MODE;
	ModeStatusCfg.IsLcdModeUpdate = true;
	delay_ms(1000);

	// update terminal
	Terminal_InitDebug(count,
					   list_RfidUserData[RfidUserData_Index].name,
					   list_RfidUserData[RfidUserData_Index].license_plate);
}
void display_Idle()
{
	// khi phat hien RFID card
	if (ModeStatusCfg.IsLcdModeUpdate)
	{
		// Update lcd display
		LCD_I2C_Clear(&lcd); /* xoa lcd */
		ModeStatusCfg.IsLcdModeUpdate = false;
		delay_ms(2);

		// update terminal
		Terminal_IdleDebug();
	}

	// display lcd lien tuc khi chua phat hien RFID card
	else
	{
		LCD_I2C_SetCursor(&lcd, 4, 0);
		LCD_I2C_WriteString(&lcd, "VUI LONG");
		LCD_I2C_SetCursor(&lcd, 4, 1);
		LCD_I2C_WriteString(&lcd, "QUET THE");
	}
}
void display_Run()
{
	if (ModeStatusCfg.IsLcdModeUpdate)
	{
		// update lcd display
		LCD_I2C_Clear(&lcd);
		ModeStatusCfg.IsLcdModeUpdate = false;
		delay_ms(2);
		LCD_I2C_SetCursor(&lcd, 0, 0);

		// update terminal
		Terminal_RunDebug(list_RfidUserData[RfidUserData_Index].name,
						  list_RfidUserData[RfidUserData_Index].license_plate);
	}

	if (DeviceStatusCfg.barrierSignal == CLOSING)
	{

		if (ModeStatusCfg.IsLcdModeUpdate)
		{
			LCD_I2C_Clear(&lcd);
			delay_ms(2);
			ModeStatusCfg.IsLcdModeUpdate = false;
		}

		else
		{
			if (DeviceStatusCfg.LedSignal == ON)
			{
				LCD_I2C_WriteString(&lcd, "BARRIER IS");
				LCD_I2C_SetCursor(&lcd, 0, 1);
				LCD_I2C_WriteString(&lcd, "OPENING");
			}
			else
			{
				LCD_I2C_WriteString(&lcd, "BARRIER CLOSED");
				LCD_I2C_SetCursor(&lcd, 0, 1);
				LCD_I2C_WriteString(&lcd, "SUCCESSFULLY");
				delay_ms(1000);
			}
		}
	}
	else
	{
		if (ModeStatusCfg.IsLcdModeUpdate)
		{
			LCD_I2C_Clear(&lcd);
			delay_ms(1);
			ModeStatusCfg.IsLcdModeUpdate = false;
		}

		if (DeviceStatusCfg.LedSignal == ON)
		{
			LCD_I2C_WriteString(&lcd, "BARRIER IS");
			LCD_I2C_SetCursor(&lcd, 0, 1);
			LCD_I2C_WriteString(&lcd, "CLOSING");
		}
		else
		{
			LCD_I2C_WriteString(&lcd, "BARRIER OPENED");
			LCD_I2C_SetCursor(&lcd, 0, 1);
			LCD_I2C_WriteString(&lcd, "SUCCESSFULLY");
		}
	}
}
void display_Error()
{
	if (ModeStatusCfg.IsLcdModeUpdate)
	{
		// update lcd display
		LCD_I2C_Clear(&lcd);
		delay_ms(1);
		ModeStatusCfg.IsLcdModeUpdate = false;

		// update terminal
		Terminal_ErrorDebug();
	}
	else
	{
		switch ((uint8_t)ModeStatusCfg.errortype)
		{
		case UID_NOTFOUND:
			LCD_I2C_SetCursor(&lcd, 0, 0);
			LCD_I2C_WriteString(&lcd, "KHONG CO THE");
			LCD_I2C_SetCursor(&lcd, 5, 1);
			LCD_I2C_WriteString(&lcd, "RFID");
			break;
		case RFID_SCAN_EMPTY:
			LCD_I2C_SetCursor(&lcd, 0, 0);
			LCD_I2C_WriteString(&lcd, "THE RFID");
			LCD_I2C_SetCursor(&lcd, 0, 1);
			LCD_I2C_WriteString(&lcd, "KHONG CO DATA");
			break;
		default:
			break;
		}
		// update lcd mode

		ModeStatusCfg.apptype = IDLE_MODE;
		ModeStatusCfg.IsLcdModeUpdate = true;
		delay_ms(2000);
	}
}
void display_Fail()
{
	LCD_I2C_SetCursor(&lcd, 4, 0);
	LCD_I2C_WriteString(&lcd, "HE THONG ");
	LCD_I2C_SetCursor(&lcd, 3, 1);
	LCD_I2C_WriteString(&lcd, "KHONG CO DATA");
}