#ifndef __MCU_DMA_H
#define __MCU_DMA_H
#include "stm32f10x.h"

void McuDmaInit(DMA_Channel_TypeDef *Channelx,uint32_t DmaPeripheralBaseAddr,uint32_t DmaMemoryBaseAddr,uint32_t DmaDIR,uint32_t DmaBufferSize,
                uint32_t DmaPeripheralInc,uint32_t DmaMemoryInc,uint32_t DmaPeripheralDataSize,uint32_t DmaMemoryDataSize,
                uint32_t DmaMode,uint32_t DmaPriority,uint32_t DmaM2M);
void McuDmaResetMemoryBaseAddr(DMA_Channel_TypeDef *Channelx,uint32_t MemoryBaseAddr,uint32_t BufferSize);
#endif
