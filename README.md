# Mainpulator
USE STM32_FreeRTOS,OWN DEBUG FUNCTION

System uses FreeRTOS

Serial port uses DMA + semaphore

NRF24L01 SPI interface uses DMA + semaphore

The correlation function of the steering gear control is in the file **Hardware/arm/**

The communication interface is in the file **APP/Communication/**

You can put the execution of communication into the Task Function **CmucTask()** 

```
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
        if(NRF24L01_RxPacket(CommunicationBuf)==0)
        {
            CmucLedFlag=1;
            
        }
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}
```



## four tasks were used.<br>
TaskName  | TaskPrio | TaskStackSize | TaskHandler      | TaskFunction
--------  | -------- | ------------- | -----------      | ------------
StartTask |    1     |      128      | StartTaskHandler | 初始化
DebugTask |    2     |      128      | DebugTaskHandler | 调试
LedTask   |    3     |      64       | LedTaskHandler   | 指示灯
CmucTask  |    4     |      128      | CmucTaskHandler  | 通信


