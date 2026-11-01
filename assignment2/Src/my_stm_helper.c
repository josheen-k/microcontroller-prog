#include "my_stm_helper.h"

// WRAPPERS TAKEN FROM MOCK TEST APENDIX:
void Clock_Enable(PeripheralBus_t peripheral) {
    switch (peripheral) {
        case GPIOA_BUS: RCC->IOPENR |= 1<<0; break; // Enable Clock for GPIOA
        case GPIOB_BUS: RCC->IOPENR |= 1<<1; break; // Enable Clock for GPIOB
        case GPIOC_BUS: RCC->IOPENR |= 1<<2; break; // Enable Clock for GPIOC
        case TIM3_BUS:  RCC->APBENR1 |= 1<<1; break; // Enable Clock for Timer 3
        case TIM16_BUS: RCC->APBENR2 |= 1<<17; break; // Enable Clock for Timer 16
        case TIM17_BUS: RCC->APBENR2 |= 1<<18; break; // Enable Clock for Timer 17
    }
}

void GPIO_InitPin(GPIO_TypeDef *port, uint8_t pin, GPIOMode_t mode, GPIOPull_t pull) {
    // Configure Mode
    port->MODER &= ~(0b11 << (pin * 2));
    port->MODER |= (mode << (pin * 2));

    // Configure Pull-Up/Pull-Down Resistor
    port->PUPDR &= ~(0b11 << (pin * 2));
    port->PUPDR |= (pull << (pin * 2));
}

void Timer_Init(TIM_TypeDef *timer, IRQn_Type irq_type, uint16_t psc, uint16_t arr, bool enable_interrupt) {
    timer->PSC = psc;
    timer->ARR = arr;

    if (enable_interrupt) {
        timer->DIER |= 1<<0; // Turn on interrupts for the Timer
        NVIC_EnableIRQ(irq_type); // Allow timer to interrupt Cortex processor
    }
    // Enable Timer/Counter
    timer->CR1 |= 1<<0;
}

void GPIO_OutputClearPin(GPIO_TypeDef *port, uint8_t pin) {
    port->ODR &= ~(1 << pin);
}

void GPIO_OutputSetPin(GPIO_TypeDef *port, uint8_t pin) {
    port->ODR |= (1 << pin);
}

void GPIO_OutputTogglePin(GPIO_TypeDef *port, uint8_t pin) {
    port->ODR ^= (1 << pin);
}

bool GPIO_InputReadPin(GPIO_TypeDef *port, uint8_t pin) {
    return (port->IDR & (1 << pin)) != 0; // Check if Pin is high
}

void EXTI_InitPin(EXTIPort_t port, uint8_t pin, EXTITrigger_t trigger, IRQn_Type irq_type) {

    uint8_t idx = pin / 4; // Determine EXTICR register index (0,1,2,3)
    uint8_t shift = (pin % 4) * 8; // Determine bits (each pin is 8 bits)

    // EXTI line for specific GPIO Port
    EXTI->EXTICR[idx] &= ~(0xFF << shift);
    EXTI->EXTICR[idx] |= (port << shift);

    // Clear existing trigger configurations
    EXTI->FTSR1 &= ~(1 << pin);
    EXTI->RTSR1 &= ~(1 << pin);

    // Configure Trigger Edge
    if (trigger == FALLING || trigger == BOTH) {
        EXTI->FTSR1 |= (1 << pin); // Enable Falling Edge
    }
    if (trigger == RISING || trigger == BOTH) {
        EXTI->RTSR1 |= (1 << pin); // Enable Rising Edge
    }

    // Set interrupt mask, i.e. enable
    EXTI->IMR1 |= (1 << pin);

    // Clear any pending flags (FPR1 = Falling, RPR1 = Rising)
    // Write to clear register
    EXTI->FPR1 |= (1 << pin);
    EXTI->RPR1 |= (1 << pin);

    // Enable NVIC Interrupt Line on Cortex processor
    NVIC_EnableIRQ(irq_type);
}

// WRAPPER TAKEN FROM PRAC 4:
void GPIO_SetAltFunc(GPIO_TypeDef *port, uint8_t pin, uint8_t af_number) {
    // AFR [0] handles pins 0 -7. AFR [1] handles pins 8 -15.
    // Each pin gets 4 bits to define its Alternate Function (0 to 15) .
    if (pin < 8) {
        port->AFR[0] &= ~(0xFUL << (pin * 4));
        port->AFR[0] |= ((uint32_t)af_number << (pin * 4));
    } else {
        port->AFR[1] &= ~(0xFUL << ((pin - 8) * 4));
        port->AFR[1] |= ((uint32_t)af_number << ((pin - 8) * 4));
    }
}