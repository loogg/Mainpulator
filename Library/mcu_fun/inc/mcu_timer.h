#ifndef __MCU_TIMER_H
#define __MCU_TIMER_H
#include "stm32f10x.h"

typedef enum
{

	TPC_1 = 1,
	TPC_2 = 2,
	TPC_3 = 4,
	TPC_4 = 8,

} TIMER_PWM_CHANNEL;


void McuTimerBaseInit(TIM_TypeDef *Timx, uint16_t TimPrescaler, uint16_t TimCounterMode, uint16_t TimPeriod,
                      uint16_t TimClockDivision, uint8_t TimRepetitionCounter);
void McuTimerPwmInit(TIM_TypeDef *Timx, uint16_t tpc, uint16_t TimOCMode, uint16_t TimOutputState, uint16_t TimOutputNState, 
                     uint16_t *TimPulse, uint16_t TimOCPolarity, uint16_t TimOCNPolarity, uint16_t TimOCIdleState, uint16_t TimOCNIdleState);

#endif
