/* Debug_Printf.c */
#include "Debug_Printf.h"
#include <stdio.h>

void Debug_WritePrintf(const char *Text)
{
    if (Text != NULL)
    {
        printf("%s", Text);
    }
}