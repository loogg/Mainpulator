#include "usart.h"
#include "mcu_gpio.h"
#include "mcu_usart.h"
#include "mcu_nvic.h"
#include "mcu_dma.h"
#include "string.h"


UsartInfo Usart1Info = {{0}, {0}, 0, 0,NULL};

void Usart1Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    
    McuGpioInit(GPIOA, GPIO_Pin_9, GPIO_Speed_50MHz, GPIO_Mode_AF_PP);
    McuGpioInit(GPIOA, GPIO_Pin_10, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING);

    McuUsartInit(USART1, 115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No,
                 USART_Mode_Tx | USART_Mode_Rx, USART_HardwareFlowControl_None);  
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    USART_ITConfig(USART1, USART_IT_TC, ENABLE);
    McuNvicInit(USART1_IRQn, 5, 0, ENABLE);
    USART_Cmd(USART1, ENABLE);


    McuDmaInit(DMA1_Channel4, (u32)&USART1->DR, (u32)Usart1Info.SendBuf, DMA_DIR_PeripheralDST, 0, DMA_PeripheralInc_Disable,
               DMA_MemoryInc_Enable, DMA_PeripheralDataSize_Byte, DMA_MemoryDataSize_Byte, DMA_Mode_Normal,
               DMA_Priority_Medium, DMA_M2M_Disable);

    McuDmaInit(DMA1_Channel5, (u32)&USART1->DR, (u32)Usart1Info.ReceiveBuf, DMA_DIR_PeripheralSRC, USART_BUF_LEN, DMA_PeripheralInc_Disable,
               DMA_MemoryInc_Enable, DMA_PeripheralDataSize_Byte, DMA_MemoryDataSize_Byte, DMA_Mode_Circular,
               DMA_Priority_Medium, DMA_M2M_Disable);
    DMA_Cmd(DMA1_Channel5, ENABLE);
    
    Usart1Info.SendBusySemaphore = xSemaphoreCreateBinary();
	if (Usart1Info.SendBusySemaphore != NULL)
	{
		xSemaphoreGive(Usart1Info.SendBusySemaphore);
	}

}


void USART1_IRQHandler(void)
{
	u8 chr;
    BaseType_t xHigherPriorityTaskWoken;
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		USART1->SR;
		chr = USART1->DR;
		if (chr == 0x0a)
		{
			Usart1Info.RevFlag = 1;
			DMA_Cmd(DMA1_Channel5, DISABLE);
			Usart1Info.RevLen = USART_BUF_LEN - DMA_GetCurrDataCounter(DMA1_Channel5);
			DMA1_Channel5->CNDTR = USART_BUF_LEN;
			DMA_Cmd(DMA1_Channel5, ENABLE);
		}
	}

    if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
    {
        USART_ClearITPendingBit(USART1,USART_IT_TC);
        if(Usart1Info.SendBusySemaphore != NULL)
        {
            xSemaphoreGiveFromISR(Usart1Info.SendBusySemaphore,&xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }

    }
}
