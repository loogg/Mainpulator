#include "mcu_gpio.h"

void McuGpioInit(GPIO_TypeDef *Gpiox, uint16_t GpioPin, GPIOSpeed_TypeDef GpioSpeed, GPIOMode_TypeDef GpioMode)
{
    GPIO_InitTypeDef GPIO_InitStrcture;

    GPIO_InitStrcture.GPIO_Pin = GpioPin;
    GPIO_InitStrcture.GPIO_Speed = GpioSpeed;
    GPIO_InitStrcture.GPIO_Mode = GpioMode;

    GPIO_Init(Gpiox, &GPIO_InitStrcture);
}

void McuGpioSwjJtagDisable(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, DISABLE);
}
