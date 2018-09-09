#include "delay.h"
#include "FreeRTOSConfig.h"


static u8 fac_us = 0;

void DelayInit(void)
{
    fac_us = SystemCoreClock / configTICK_RATE_HZ / 1000;
}

void DelayUs(u32 nus)
{
    u32 ticks;
    u32 told, tnow, tcnt = 0;
    u32 reload = SysTick->LOAD; //LOAD的值
    ticks = nus * fac_us;       //需要的节拍数
    told = SysTick->VAL;        //刚进入时的计数器值
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow; //这里注意一下SYSTICK是一个递减的计数器就可以了.
            else
                tcnt += reload - tnow + told;
            told = tnow;
            if (tcnt >= ticks)
                break; //时间超过/等于要延迟的时间,则退出.
        }
    };
}

