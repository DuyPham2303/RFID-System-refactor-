#include "control_output.h"
#include "servo.h"
#include "AppTypes.h"
#include "Systick.h"

uint32_t wait_time = 0;	   // measure the idle duration between OPENING/close
uint32_t servo_updateTime; // thoi gian cap nhat goc quay servo
DeviceStatusCfg_s DeviceStatusCfg = {.barrierSignal = CLOSING,
									 .LedSignal = OFF};

void SignalOutput_ModuleInit()
{
	Servo_Init();
	Led_Init();
}
void Led_Control()
{
	// if servo does not enter idle duration and current system status is EXECUTE_MODE -> start blinking led
	if (wait_time == 0 && ModeStatusCfg.apptype == RUN_MODE)
	{
		if (DeviceStatusCfg.LedSignal == OFF)
		{
			LedTimerInterruptEnable(); // enable interrupt timer when servo start rotating
			DeviceStatusCfg.LedSignal = ON;
			ModeStatusCfg.IsLcdModeUpdate = true;
		}
	}
	// if wait time start measuring or barrier has already closed -> stop blinking led
	else if (wait_time > 0 || DeviceStatusCfg.barrierSignal == CLOSING)
	{
		if (DeviceStatusCfg.LedSignal == ON)
		{
			LedTimerInterruptDisable(); // disable interrupt timer when servo stop rorating
			DeviceStatusCfg.LedSignal = OFF;
			ModeStatusCfg.IsLcdModeUpdate = true;
		}
	}
}
void Barrier_Control()
{
	static ServoStatus_e st;
	// OPENING barrier
	if (DeviceStatusCfg.barrierSignal == CLOSING)
	{
		// start opening the barrier and keep checking until servo has done rotating
		if (st == FULL_RANGE)
		{
			DeviceStatusCfg.barrierSignal = OPENING; // update servo's status
			wait_time = getTick();					 // started counting wait time -> servo enter idle state
		}
		else
		{
			st = Increase_ServoPos();
		}
	}
	// check whether 5s has passed or not
	if (DeviceStatusCfg.barrierSignal == OPENING && getTick() - wait_time > 3000)
	{
		if (wait_time > 0)
		{
			wait_time = 0; // reset wait duration
		}
		// start closing the barrier and keep checking until servo has done rotating
		if (st == FULL_RANGE)
		{
			DeviceStatusCfg.barrierSignal = CLOSING;
			// switch to other system's status
			ModeStatusCfg.apptype = IDLE_MODE;
		}
		else
		{
			st = Decrease_ServoPos();
		}
	}
}