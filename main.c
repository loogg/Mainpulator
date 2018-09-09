#include "stm32f10x.h"
#include "delay.h"
#include "arm.h"
#include "debug.h"
#include "communication.h"

#include "FreeRTOS.h"
#include "task.h"

//�������ȼ�
#define START_TASK_PRIO 1
//�����ջ��С
#define START_STK_SIZE 128
//������
TaskHandle_t StartTaskHandler;
//������
void StartTask(void *p_arg);

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    DelayInit();
    ArmInit();
    
    xTaskCreate(StartTask, "StartTask", START_STK_SIZE, NULL, START_TASK_PRIO, &StartTaskHandler);
    vTaskStartScheduler();
    while (1);
}

/*
************************************************************
*	�������ƣ�	StartTask
*
*	�������ܣ�	��������
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		��
************************************************************
*/
void StartTask(void *p_arg)
{
    taskENTER_CRITICAL(); //�����ٽ���
    DebugInit();
    CommunicationInit();
    taskEXIT_CRITICAL(); //�˳��ٽ���

    while(1)
    {
        vTaskDelay(100 / portTICK_RATE_MS);
    }

}
