#ifndef DISPLAY_H
#define DISPLAY_H
#include "AppTypes.h"

void LCD_ModuleInit();
AppMode_e AppModeUpdate();

void display_Init();
void display_Idle();
void display_Run();
void display_Error();
void display_Fail();

#endif