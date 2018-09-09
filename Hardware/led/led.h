#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"


void LedInit(void);
u8 AlterLedState(u8 LedNum, u8 LedState);
void LedToggle(u8 LedNum);
#endif
