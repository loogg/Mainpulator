#include "mcu_timer.h"

void McuTimerBaseInit(TIM_TypeDef *Timx, uint16_t TimPrescaler, uint16_t TimCounterMode, uint16_t TimPeriod,
                      uint16_t TimClockDivision, uint8_t TimRepetitionCounter)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Prescaler = TimPrescaler;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TimCounterMode;
    TIM_TimeBaseInitStructure.TIM_Period = TimPeriod;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TimClockDivision;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = TimRepetitionCounter;
    TIM_TimeBaseInit(Timx, &TIM_TimeBaseInitStructure);
}

void McuTimerPwmInit(TIM_TypeDef *Timx, uint16_t tpc, uint16_t TimOCMode, uint16_t TimOutputState, uint16_t TimOutputNState, 
                     uint16_t *TimPulse, uint16_t TimOCPolarity, uint16_t TimOCNPolarity, uint16_t TimOCIdleState, uint16_t TimOCNIdleState)
{
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TimOCMode;
    TIM_OCInitStructure.TIM_OutputState = TimOutputState;
    TIM_OCInitStructure.TIM_OutputNState = TimOutputNState;
    TIM_OCInitStructure.TIM_OCPolarity = TimOCPolarity;
    TIM_OCInitStructure.TIM_OCNPolarity = TimOCNPolarity;
    TIM_OCInitStructure.TIM_OCIdleState = TimOCIdleState;
    TIM_OCInitStructure.TIM_OCNIdleState = TimOCNIdleState;

    if (tpc & 0x01)
    {
        TIM_OCInitStructure.TIM_Pulse = TimPulse[0];
        TIM_OC1Init(Timx, &TIM_OCInitStructure);
        TIM_OC1PreloadConfig(Timx, TIM_OCPreload_Enable); //使能TIMx在CCR1上的预装载寄存器
    }

    if (tpc & 0x02)
    {
        TIM_OCInitStructure.TIM_Pulse = TimPulse[1];
        TIM_OC2Init(Timx, &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(Timx, TIM_OCPreload_Enable); //使能TIMx在CCR2上的预装载寄存器
    }

    if (tpc & 0x04)
    {
        TIM_OCInitStructure.TIM_Pulse = TimPulse[2];
        TIM_OC3Init(Timx, &TIM_OCInitStructure);
        TIM_OC3PreloadConfig(Timx, TIM_OCPreload_Enable); //使能TIMx在CCR3上的预装载寄存器
    }

    if (tpc & 0x08)
    {
        TIM_OCInitStructure.TIM_Pulse = TimPulse[3];
        TIM_OC4Init(Timx, &TIM_OCInitStructure);
        TIM_OC4PreloadConfig(Timx, TIM_OCPreload_Enable); //使能TIMx在CCR4上的预装载寄存器
    }
    TIM_CtrlPWMOutputs(Timx, ENABLE);
    TIM_ARRPreloadConfig(Timx, ENABLE);
}
