#include "debug.h"
#include "usart.h"
#include "mcu_dma.h"
#include "led.h"
#include "arm.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


//任务优先级
#define DEBUG_TASK_PRIO 2
//任务堆栈大小
#define DEBUG_STK_SIZE 128
//任务句柄
TaskHandle_t DebugTaskHandler;
//任务函数
void DebugTask(void *p_arg);

u8 DebugLedFlag = 0;

void DebugInit(void)
{
    Usart1Init();
    xTaskCreate(DebugTask, "DebugTask", DEBUG_STK_SIZE, NULL, DEBUG_TASK_PRIO, &DebugTaskHandler);
}

void DebugPrintf(const char *fmt, ...)
{
    
    xSemaphoreTake(Usart1Info.SendBusySemaphore,portMAX_DELAY);
    va_list va_args;
    unsigned char *pStr = Usart1Info.SendBuf;
    va_start(va_args, fmt);
    vsprintf((char *)Usart1Info.SendBuf,fmt, va_args);
    va_end(va_args);
    McuDmaResetMemoryBaseAddr(DMA1_Channel4,(u32)Usart1Info.SendBuf,strlen((const char *)pStr));
}

void DebugSendData(u8 *Data, u8 Len)
{
    xSemaphoreTake(Usart1Info.SendBusySemaphore, portMAX_DELAY);
    for (u8 i = 0; i < Len; i++)
        Usart1Info.SendBuf[i] = Data[i];
    McuDmaResetMemoryBaseAddr(DMA1_Channel4, (u32)Usart1Info.SendBuf, Len);
}

void DebugMenu(void)
{
    DebugPrintf("\r\n=========================================================");
    DebugPrintf("\r\n==========(C) COPYRIGHT 2018 MaLongWei ==================");
    DebugPrintf("\r\n|         ARMControl Debug (Version 1.0)                |");
    DebugPrintf("\r\n|----command----     |------------function--------------|");
    DebugPrintf("\r\n| ledx(0-2) 0|1      | TURN ON/OFF THE LEDx             |");
    DebugPrintf("\r\n| enginex(1-4) angle | ALTER THE ENGINEx ANGLE          |");
    DebugPrintf("\r\n| c: CHECK           | CHECK STACK                      |");
    DebugPrintf("\r\n| r: RESET           | RESET THE SYSTEM                 |");
    DebugPrintf("\r\n| ?: HELP            | Display Help Menu                |");
    DebugPrintf("\r\n=========================================================");
    DebugPrintf("\r\n\r\n>>");
}

/*
************************************************************
*	函数名称：	DebugTask
*
*	函数功能：	调试任务
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		无
************************************************************
*/
void DebugTask(void *p_arg)
{
    int PerpheralNum, PerpheralState;
    float PerpheralValue;
    UBaseType_t uxHighWaterMark;
    while (1)
    {
        if (Usart1Info.RevFlag)
        {
            Usart1Info.RevFlag = 0;
            Usart1Info.ReceiveBuf[Usart1Info.RevLen] = 0;
            if (strstr((char *)Usart1Info.ReceiveBuf, "?"))
                DebugMenu();
            else if (strstr((char *)Usart1Info.ReceiveBuf, "r"))
            {
                __disable_irq();
                NVIC_SystemReset();
                while (1)
                    ;
            }
            else if(strstr((char *)Usart1Info.ReceiveBuf, "c"))
            {
                DebugPrintf(" TaskName               RemainTask      \r\n");
                uxHighWaterMark=uxTaskGetStackHighWaterMark(xTaskGetHandle("StartTask"));
                DebugPrintf("StartTask                  %d\r\n",uxHighWaterMark);
                uxHighWaterMark=uxTaskGetStackHighWaterMark(xTaskGetHandle("DebugTask"));
                DebugPrintf("DebugTask                  %d\r\n",uxHighWaterMark);
                uxHighWaterMark=uxTaskGetStackHighWaterMark(xTaskGetHandle("CmucTask"));
                DebugPrintf("CmucTask                   %d\r\n",uxHighWaterMark);
                uxHighWaterMark=uxTaskGetStackHighWaterMark(xTaskGetHandle("LedTask"));
                DebugPrintf("LedTask                    %d\r\n",uxHighWaterMark);
            }
            else if (strstr((char *)Usart1Info.ReceiveBuf, "led"))
            {
                if (sscanf((char *)Usart1Info.ReceiveBuf, "led%d %d", &PerpheralNum, &PerpheralState) == 2)
                {
                    if (AlterLedState(PerpheralNum, PerpheralState ? 1 : 0) == 0)
                        DebugPrintf("led_num:%d     led_state:%d\r\n", PerpheralNum, PerpheralState ? 1 : 0);
                }
            }
            else if (strstr((char *)Usart1Info.ReceiveBuf, "engine"))
            {
                if (sscanf((char *)Usart1Info.ReceiveBuf, "engine%d %f", &PerpheralNum, &PerpheralValue) == 2)
                {
                    if ((PerpheralValue < 0) || (PerpheralValue > 180))
                        return;
                    if (AlterEngineAngle(PerpheralNum, PerpheralValue) == 0)
                        DebugPrintf("engine_num:%d     engine_angle:%.2f\r\n", PerpheralNum, PerpheralValue);
                }
            }

            DebugLedFlag = 1;
            DebugPrintf("\r\n\r\n>>");
        }
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}
