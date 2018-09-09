#include "mcu_spi.h"

void McuSpiInit(SPI_TypeDef *Spix, uint16_t SpiDirection, uint16_t SpiMode, uint16_t SpiDataSize, uint16_t SpiCPOL,
                uint16_t SpiCPHA, uint16_t SpiNSS, uint16_t SpiBaudRatePrescaler, uint16_t SpiFirstBit, uint16_t SpiCRCPolynomial)
{
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SpiDirection;
    SPI_InitStructure.SPI_Mode = SpiMode;
    SPI_InitStructure.SPI_DataSize = SpiDataSize;
    SPI_InitStructure.SPI_CPOL = SpiCPOL;
    SPI_InitStructure.SPI_CPHA = SpiCPHA;
    SPI_InitStructure.SPI_NSS = SpiNSS;
    SPI_InitStructure.SPI_BaudRatePrescaler = SpiBaudRatePrescaler;
    SPI_InitStructure.SPI_FirstBit = SpiFirstBit;
    SPI_InitStructure.SPI_CRCPolynomial = SpiCRCPolynomial;
    SPI_Init(Spix, &SPI_InitStructure);
}
