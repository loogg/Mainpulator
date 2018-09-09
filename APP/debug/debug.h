#ifndef __DEBUG_H
#define __DEBUG_H
#include "stm32f10x.h"

extern u8 DebugLedFlag;


void DebugInit(void);
void DebugMenu(void);
void DebugPrintf(const char *fmt, ...);
void DebugSendData(u8 *Data, u8 Len);
#endif
