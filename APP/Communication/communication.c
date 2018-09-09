#include "communication.h"
#include "nrf24l01.h"
#include "led.h"
#include "debug.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


//任务优先级
#define LED_TASK_PRIO 3
//任务堆栈大小
#define LED_STK_SIZE 64
//任务句柄
TaskHandle_t LedTaskHandler;
//任务函数
void LedTask(void *p_arg);

//任务优先级
#define CMUC_TASK_PRIO 4
//任务堆栈大小
#define CMUC_STK_SIZE 128
//任务句柄
TaskHandle_t CmucTaskHandler;
//任务函数
void CmucTask(void *p_arg);

u8 CommunicationBuf[TX_PLOAD_WIDTH];
u8 CmucLedFlag = 0;

void CommunicationInit(void)
{
    LedInit();
    Nrf24l01Init();
    xTaskCreate(LedTask, "LedTask", LED_STK_SIZE, NULL, LED_TASK_PRIO, &LedTaskHandler);
    xTaskCreate(CmucTask, "CmucTask", CMUC_STK_SIZE, NULL, CMUC_TASK_PRIO, &CmucTaskHandler);
}

/*
************************************************************
*	函数名称：	LedTask
*
*	函数功能：	指示灯任务
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		无
************************************************************
*/
void LedTask(void *p_arg)
{
    u8 SysLedCnt = 4, CmucLedCnt = 0, DebugLedCnt = 0;
    while (1)
    {
        if (++SysLedCnt == 5)
        {
            SysLedCnt = 0;
            LedToggle(0);
        }

        if (CmucLedFlag)
        {
            LedToggle(1);
            if (++CmucLedCnt == 5)
            {
                CmucLedCnt = 0;
                AlterLedState(1, 1);
                CmucLedFlag = 0;
            }
        }

        if (DebugLedFlag)
        {
            LedToggle(2);
            if (++DebugLedCnt == 5)
            {
                DebugLedCnt = 0;
                AlterLedState(2, 1);
                DebugLedFlag = 0;
            }
        }
        vTaskDelay(100 / portTICK_RATE_MS);
    }
}

/*
************************************************************
*	函数名称：	CmucTask
*
*	函数功能：	通信任务
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		无
************************************************************
*/
void CmucTask(void *p_arg)
{
    while (NRF24L01_Check())
    {
        DebugPrintf("NRF24L01 NOT FOUND\r\n");
        vTaskDelay(500 / portTICK_RATE_MS);
    }
    DebugPrintf("NRF24L01 FOUND\r\n");
    NRF24L01_RX_Mode();
    while (1)
    {
        if (NRF24L01_RxPacket(CommunicationBuf) == 0)
        {
            CmucLedFlag = 1;
        }
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}
