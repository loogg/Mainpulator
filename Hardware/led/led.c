#include "led.h"
#include "mcu_gpio.h"

#define LED0(x)     GPIO_WriteBit(GPIOA,GPIO_Pin_15,(BitAction)x)
#define LED1(x)     GPIO_WriteBit(GPIOB,GPIO_Pin_3,(BitAction)x)
#define LED2(x)     GPIO_WriteBit(GPIOB,GPIO_Pin_4,(BitAction)x)

void LedInit(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    McuGpioSwjJtagDisable();

    McuGpioInit(GPIOA, GPIO_Pin_15, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
    McuGpioInit(GPIOB, GPIO_Pin_3, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
    McuGpioInit(GPIOB, GPIO_Pin_4, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);

    LED0(1);
    LED1(1);
    LED2(1);

}

void LedToggle(u8 LedNum)
{
    static u8 LedState[3] = {0};
    switch (LedNum)
    {
    case 0:
        LED0(LedState[0]);
        LedState[0]++;
        LedState[0] %= 2;
        break;
    case 1:
        LED1(LedState[1]);
        LedState[1]++;
        LedState[1] %= 2;
        break;
    case 2:
        LED2(LedState[2]);
        LedState[2]++;
        LedState[2] %= 2;
        break;
    default:
        break;
    }
}

//0:³É¹¦   1:Ê§°Ü
u8 AlterLedState(u8 LedNum, u8 LedState)
{
    u8 result = 0;
    switch (LedNum)
    {
    case 0:
        LED0(LedState);
        break;
    case 1:
        LED1(LedState);
        break;
    case 2:
        LED2(LedState);
        break;
    default:
        result = 1;
        break;
    }
    return result;
}
