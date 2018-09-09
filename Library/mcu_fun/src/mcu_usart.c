#include "mcu_usart.h"

void McuUsartInit(USART_TypeDef *Usartx, uint32_t UsartBaudRate, uint16_t UsartWordLength, uint16_t UsartStopBits,
                  uint16_t UsartParity, uint16_t UsartMode, uint16_t UsartHardwareFlowControl)
{
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = UsartBaudRate;
    USART_InitStructure.USART_WordLength = UsartWordLength;
    USART_InitStructure.USART_StopBits = UsartStopBits;
    USART_InitStructure.USART_Parity = UsartParity;
    USART_InitStructure.USART_Mode = UsartMode;
    USART_InitStructure.USART_HardwareFlowControl = UsartHardwareFlowControl;
    USART_Init(Usartx, &USART_InitStructure);
}
