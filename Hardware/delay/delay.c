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
    u32 reload = SysTick->LOAD; //LOAD��ֵ
    ticks = nus * fac_us;       //��Ҫ�Ľ�����
    told = SysTick->VAL;        //�ս���ʱ�ļ�����ֵ
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow; //����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
            else
                tcnt += reload - tnow + told;
            told = tnow;
            if (tcnt >= ticks)
                break; //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
        }
    };
}

