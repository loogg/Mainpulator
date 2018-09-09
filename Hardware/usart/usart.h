#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define USART_BUF_LEN       100
typedef struct
{
    u8 SendBuf[USART_BUF_LEN];
    u8 ReceiveBuf[USART_BUF_LEN];
    u8 RevFlag;
    u16 RevLen;
    SemaphoreHandle_t SendBusySemaphore;
}UsartInfo;
extern UsartInfo Usart1Info;

void Usart1Init(void);
#endif
