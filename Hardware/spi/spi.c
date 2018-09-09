#include "spi.h"
#include "mcu_gpio.h"
#include "mcu_spi.h"
#include "mcu_dma.h"
#include "mcu_nvic.h"

#include "string.h"

SpiInfo Spi1Info = {{0}, {0}, NULL, NULL};

void Spi1Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    McuGpioInit(GPIOA, GPIO_Pin_5, GPIO_Speed_50MHz, GPIO_Mode_AF_PP);
    McuGpioInit(GPIOA, GPIO_Pin_6, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING);
    McuGpioInit(GPIOA, GPIO_Pin_7, GPIO_Speed_50MHz, GPIO_Mode_AF_PP);

    McuSpiInit(SPI1, SPI_Direction_2Lines_FullDuplex, SPI_Mode_Master, SPI_DataSize_8b, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_NSS_Soft,
               SPI_BaudRatePrescaler_16, SPI_FirstBit_MSB, 7);
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
    SPI_Cmd(SPI1, ENABLE);

    McuDmaInit(DMA1_Channel3, (u32)&SPI1->DR, NULL, DMA_DIR_PeripheralDST, 0, DMA_PeripheralInc_Disable, DMA_MemoryInc_Enable,
               DMA_PeripheralDataSize_Byte, DMA_MemoryDataSize_Byte, DMA_Mode_Normal, DMA_Priority_Medium, DMA_M2M_Disable);

    McuDmaInit(DMA1_Channel2, (u32)&SPI1->DR, NULL, DMA_DIR_PeripheralSRC, 0, DMA_PeripheralInc_Disable, DMA_MemoryInc_Enable,
               DMA_PeripheralDataSize_Byte, DMA_MemoryDataSize_Byte, DMA_Mode_Normal, DMA_Priority_Medium, DMA_M2M_Disable);
    DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
    McuNvicInit(DMA1_Channel2_IRQn, 6, 0, ENABLE);

    Spi1Info.ConfigBusySemaphore = xSemaphoreCreateBinary();
    if (Spi1Info.ConfigBusySemaphore != NULL)
    {
        xSemaphoreGive(Spi1Info.ConfigBusySemaphore);
    }

    Spi1Info.CompleteBusySemaphore = xSemaphoreCreateBinary();
}

void SpiReadWriteSameData(SPI_TypeDef *Spix, u8 *ReceiveBuf, u8 SendData, u8 Len)
{
    if (Spix == SPI1)
    {
        for (u8 i = 0; i < Len; i++)
            Spi1Info.SendBuf[i] = SendData;
        xSemaphoreTake(Spi1Info.ConfigBusySemaphore, portMAX_DELAY);
        McuDmaResetMemoryBaseAddr(DMA1_Channel2, (u32)ReceiveBuf, Len);
        McuDmaResetMemoryBaseAddr(DMA1_Channel3, (u32)Spi1Info.SendBuf, Len);
        xSemaphoreTake(Spi1Info.CompleteBusySemaphore, portMAX_DELAY);
        xSemaphoreGive(Spi1Info.ConfigBusySemaphore);
    }
}

void SpiReadWriteBuf(SPI_TypeDef *Spix, u8 *ReceiveBuf, u8 *SendBuf, u8 Len)
{
    if (Spix == SPI1)
    {
        xSemaphoreTake(Spi1Info.ConfigBusySemaphore, portMAX_DELAY);
        McuDmaResetMemoryBaseAddr(DMA1_Channel2, (u32)ReceiveBuf, Len);
        McuDmaResetMemoryBaseAddr(DMA1_Channel3, (u32)SendBuf, Len);
        xSemaphoreTake(Spi1Info.CompleteBusySemaphore, portMAX_DELAY);
        xSemaphoreGive(Spi1Info.ConfigBusySemaphore);
    }
}

void DMA1_Channel2_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken;
    if (DMA_GetITStatus(DMA1_IT_TC2) != RESET)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC2);
        if (Spi1Info.CompleteBusySemaphore != NULL)
        {
            xSemaphoreGiveFromISR(Spi1Info.CompleteBusySemaphore, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}
