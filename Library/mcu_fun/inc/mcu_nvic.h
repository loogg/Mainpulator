#ifndef __MCU_NVIC_H
#define __MCU_NVIC_H
#include "stm32f10x.h"


void McuNvicInit(uint8_t NvicIRQChannel,uint8_t NvicIRQChannelPreemptionPriority,
                 uint8_t NvicIRQChannelSubPriority,FunctionalState NvicIRQChannelCmd);
#endif
