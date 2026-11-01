#ifndef DRIVERS_H
#define DRIVERS_H
#include "stm32c0xx.h"
#include <stdbool.h>

typedef enum {
    GPIOA_BUS,
    GPIOB_BUS,
    GPIOC_BUS, // need for gate 1 pwm pin
    TIM3_BUS,  // need for gate servo pwm timer
    TIM16_BUS,
    TIM17_BUS
} PeripheralBus_t;

typedef enum {
    INPUT = 0b00,
    OUTPUT = 0b01,
    ALT = 0b10,
    ANALOG = 0b11
} GPIOMode_t;

typedef enum {
    NONE = 0b00,
    PULL_UP = 0b01,
    PULL_DOWN = 0b10
} GPIOPull_t;

typedef enum {
    FALLING,
    RISING,
    BOTH
} EXTITrigger_t;

typedef enum {
    EXTI_GPIOA = 0x00,
    EXTI_GPIOB = 0x01
} EXTIPort_t;

void Clock_Enable(PeripheralBus_t peripheral);
void GPIO_InitPin(GPIO_TypeDef *port, uint8_t pin, GPIOMode_t mode, GPIOPull_t pull);
void Timer_Init(TIM_TypeDef *timer, IRQn_Type irq_type, uint16_t psc, uint16_t arr, bool enable_interrupt);
void GPIO_OutputTogglePin(GPIO_TypeDef *port, uint8_t pin);
void GPIO_OutputSetPin(GPIO_TypeDef *port, uint8_t pin);
void GPIO_OutputClearPin(GPIO_TypeDef *port, uint8_t pin);
bool GPIO_InputReadPin(GPIO_TypeDef *port, uint8_t pin);
void EXTI_InitPin(EXTIPort_t port, uint8_t pin, EXTITrigger_t trigger, IRQn_Type irq_type);

// routes a pin to one of its alt functions (AF[0-15])
void GPIO_SetAltFunc(GPIO_TypeDef *port, uint8_t pin, uint8_t af_number);

#endif // DRIVERS_H