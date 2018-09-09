#include "communication.h"
#include "nrf24l01.h"
#include "led.h"
#include "debug.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


//�������ȼ�
#define LED_TASK_PRIO 3
//�����ջ��С
#define LED_STK_SIZE 64
//������
TaskHandle_t LedTaskHandler;
//������
void LedTask(void *p_arg);

//�������ȼ�
#define CMUC_TASK_PRIO 4
//�����ջ��С
#define CMUC_STK_SIZE 128
//������
TaskHandle_t CmucTaskHandler;
//������
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
*	�������ƣ�	LedTask
*
*	�������ܣ�	ָʾ������
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		��
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
*	�������ƣ�	CmucTask
*
*	�������ܣ�	ͨ������
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		��
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
