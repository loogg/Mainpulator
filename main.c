#include "stm32f10x.h"
#include "delay.h"
#include "arm.h"
#include "debug.h"
#include "communication.h"

#include "FreeRTOS.h"
#include "task.h"

//任务优先级
#define START_TASK_PRIO 1
//任务堆栈大小
#define START_STK_SIZE 128
//任务句柄
TaskHandle_t StartTaskHandler;
//任务函数
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
*	函数名称：	StartTask
*
*	函数功能：	启动任务
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		无
************************************************************
*/
void StartTask(void *p_arg)
{
    taskENTER_CRITICAL(); //进入临界区
    DebugInit();
    CommunicationInit();
    taskEXIT_CRITICAL(); //退出临界区

    while(1)
    {
        vTaskDelay(100 / portTICK_RATE_MS);
    }

}
