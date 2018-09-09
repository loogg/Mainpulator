#include "arm.h"
#include "mcu_gpio.h"
#include "mcu_timer.h"

#define ENGINE1     TIM4->CCR1
#define ENGINE2     TIM4->CCR2
#define ENGINE3     TIM4->CCR3
#define ENGINE4     TIM4->CCR4

#define ANGLE_PULSE_MIN         50
#define ANGLE_PULSE_MAX         200
#define ANGLE_PER_PULSE         0.9     

#define ENGINE1_INITAL_ANGLE    60
#define ENGINE2_INITAL_ANGLE    40
#define ENGINE3_INITAL_ANGLE    95
#define ENGINE4_INITAL_ANGLE    90

void ArmInit(void)
{
    u16 InitalPulse[4];
    InitalPulse[0]=ANGLE_PULSE_MIN + ENGINE1_INITAL_ANGLE / ANGLE_PER_PULSE;
    InitalPulse[1]=ANGLE_PULSE_MIN + ENGINE2_INITAL_ANGLE / ANGLE_PER_PULSE;
    InitalPulse[2]=ANGLE_PULSE_MIN + ENGINE3_INITAL_ANGLE / ANGLE_PER_PULSE;
    InitalPulse[3]=ANGLE_PULSE_MIN + ENGINE4_INITAL_ANGLE / ANGLE_PER_PULSE;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    McuGpioInit(GPIOB,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9,GPIO_Speed_50MHz,GPIO_Mode_AF_PP);
    McuTimerBaseInit(TIM4,720-1,TIM_CounterMode_Up,2000-1,TIM_CKD_DIV1,0); //72000000/720/2000=50Hz

    McuTimerPwmInit(TIM4,TPC_1|TPC_2|TPC_3|TPC_4,TIM_OCMode_PWM1,TIM_OutputState_Enable,TIM_OutputNState_Disable,InitalPulse,
                    TIM_OCPolarity_High,TIM_OCNPolarity_High,TIM_OCIdleState_Reset,TIM_OCNIdleState_Reset); 

    TIM_Cmd(TIM4,ENABLE);
}

//0:³É¹¦   1:Ê§°Ü
u8 AlterEngineAngle(u8 EngineNum, float Angle)
{
    u8 result = 0;
    switch (EngineNum)
    {
    case 1:
        ENGINE1 = ANGLE_PULSE_MIN + Angle / ANGLE_PER_PULSE;
        break;
    case 2:
        ENGINE2 = ANGLE_PULSE_MIN + Angle / ANGLE_PER_PULSE;
        break;
    case 3:
        ENGINE3 = ANGLE_PULSE_MIN + Angle / ANGLE_PER_PULSE;
        break;
    case 4:
        ENGINE4 = ANGLE_PULSE_MIN + Angle / ANGLE_PER_PULSE;
        break;
    default:
        result = 1;
        break;
    }
    return result;
}
