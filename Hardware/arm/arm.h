#ifndef __ARM_H
#define __ARM_H
#include "stm32f10x.h"

#define ENGINE1     TIM4->CCR1
#define ENGINE2     TIM4->CCR2
#define ENGINE3     TIM4->CCR3
#define ENGINE4     TIM4->CCR4

void ArmInit(void);
u8 AlterEngineAngle(u8 EngineNum, float Angle);
#endif
