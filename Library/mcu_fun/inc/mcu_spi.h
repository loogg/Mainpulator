#ifndef __MCU_SPI_H
#define __MCU_SPI_H
#include "stm32f10x.h"

void McuSpiInit(SPI_TypeDef *Spix, uint16_t SpiDirection, uint16_t SpiMode, uint16_t SpiDataSize, uint16_t SpiCPOL,
                uint16_t SpiCPHA, uint16_t SpiNSS, uint16_t SpiBaudRatePrescaler, uint16_t SpiFirstBit, uint16_t SpiCRCPolynomial);
#endif
