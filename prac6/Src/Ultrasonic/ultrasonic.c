#include <stdint.h>
#define STM32G474xx
#include "stm32g4xx.h"
#include "adc.h"
#include "ultrasonic.h"

# define ULTRASONIC_CHANNEL ADC_CHANNEL_1
# define ADC_MAX_VALUE 4095 UL
# define MB1043_FULL_SCALE_MM 5120 UL

void Ultrasonic_Init (void) {
    RCC->AHB2ENR |= (1<<0);
    GPIOA->MODER |= GPIO_MODER_MODE0 ;
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0 ;
}

void