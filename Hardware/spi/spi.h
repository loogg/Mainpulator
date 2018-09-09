#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define SPI_BUF_LEN     100
typedef struct
{
    u8 SendBuf[SPI_BUF_LEN];
    u8 ReceiveBuf[SPI_BUF_LEN];
    SemaphoreHandle_t ConfigBusySemaphore;
    SemaphoreHandle_t CompleteBusySemaphore;
} SpiInfo;
extern SpiInfo Spi1Info;

void Spi1Init(void);
void SpiReadWriteSameData(SPI_TypeDef *Spix,u8 *ReceiveBuf,u8 SendData,u8 Len);
void SpiReadWriteBuf(SPI_TypeDef *Spix,u8 *ReceiveBuf,u8 *SendBuf,u8 Len);
#endif
