#ifndef __MCU_GPIO_H
#define __MCU_GPIO_H

#include "stm32f10x.h"

void McuGpioInit(GPIO_TypeDef *Gpiox,uint16_t GpioPin,GPIOSpeed_TypeDef GpioSpeed,GPIOMode_TypeDef GpioMode);
void McuGpioSwjJtagDisable(void);
#endif
