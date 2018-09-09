#include "mcu_nvic.h"

void McuNvicInit(uint8_t NvicIRQChannel, uint8_t NvicIRQChannelPreemptionPriority,
                 uint8_t NvicIRQChannelSubPriority, FunctionalState NvicIRQChannelCmd)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = NvicIRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NvicIRQChannelPreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NvicIRQChannelSubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = NvicIRQChannelCmd;
    NVIC_Init(&NVIC_InitStructure);
}
