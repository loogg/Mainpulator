#ifndef __MCU_USART_H
#define __MCU_USART_H
#include "stm32f10x.h"

void McuUsartInit(USART_TypeDef *usart,uint32_t UsartBaudRate,uint16_t UsartWordLength,uint16_t UsartStopBits,
                  uint16_t UsartParity,uint16_t UsartMode,uint16_t UsartHardwareFlowControl);
#endif
