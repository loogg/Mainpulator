#include "mcu_dma.h"

void McuDmaInit(DMA_Channel_TypeDef *Channelx,uint32_t DmaPeripheralBaseAddr,uint32_t DmaMemoryBaseAddr,uint32_t DmaDIR,uint32_t DmaBufferSize,
                uint32_t DmaPeripheralInc,uint32_t DmaMemoryInc,uint32_t DmaPeripheralDataSize,uint32_t DmaMemoryDataSize,
                uint32_t DmaMode,uint32_t DmaPriority,uint32_t DmaM2M)
{

    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr=DmaPeripheralBaseAddr;
    DMA_InitStructure.DMA_MemoryBaseAddr=DmaMemoryBaseAddr;
    DMA_InitStructure.DMA_DIR=DmaDIR;
    DMA_InitStructure.DMA_BufferSize=DmaBufferSize;
    DMA_InitStructure.DMA_PeripheralInc=DmaPeripheralInc;
    DMA_InitStructure.DMA_MemoryInc=DmaMemoryInc;
    DMA_InitStructure.DMA_PeripheralDataSize=DmaPeripheralDataSize;
    DMA_InitStructure.DMA_MemoryDataSize=DmaMemoryDataSize;
    DMA_InitStructure.DMA_Mode=DmaMode;
    DMA_InitStructure.DMA_Priority=DmaPriority;
    DMA_InitStructure.DMA_M2M=DmaM2M;
    DMA_Init(Channelx,&DMA_InitStructure);

}

void McuDmaResetMemoryBaseAddr(DMA_Channel_TypeDef *Channelx,uint32_t MemoryBaseAddr,uint32_t BufferSize)
{
    DMA_Cmd(Channelx,DISABLE);
    Channelx->CMAR=MemoryBaseAddr;
    Channelx->CNDTR=BufferSize;
    DMA_Cmd(Channelx,ENABLE);
}

