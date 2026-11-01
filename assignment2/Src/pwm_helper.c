#include "pwm_helper.h"
#include "my_stm_helper.h"

// FUNCTION FLOW SIMILAR TO PRAC 4:
void PWM_Init(void) {
    // Enable clocks for GPIOB, GPIOC and TIM3
    // Clock_Enable(GPIOB_BUS); // GPIOB is already enabled thru GPIO_init() in main
    Clock_Enable(GPIOC_BUS);
    Clock_Enable(TIM3_BUS);

    // Set pc7 and pb0 alt func mode
    GPIO_InitPin(GPIOC, 7, ALT, NONE);
    GPIO_InitPin(GPIOB, 0, ALT, NONE);

    // Route pc7 to channel 2 and pb0 to channel 3, both alt function 1
    GPIO_SetAltFunc(GPIOC, 7, 1);
    GPIO_SetAltFunc(GPIOB, 0, 1);

    // Configure timer for 50Hz
    TIM3->PSC = 47; // Prescaler: we want 1 MHz
    TIM3->ARR = 19999; // Period: 20,000 ticks = 20ms

    // Configure pwm mode 1 on channel 2
    // Clear output compare mode bits, then set to PWM mode 1 (110)
    TIM3->CCMR1 &= ~(0b11 << 8); 
    TIM3->CCMR1 &= ~(0b111 << 12);
    TIM3->CCMR1 |=  (0b110 << 12);
    TIM3->CCMR1 |=  (1 << 11); // Enable preload

    // Configure pwm mode 1 on channel 3
    TIM3->CCMR2 &= ~(0b11 << 0);
    TIM3->CCMR2 &= ~(0b111 << 4);
    TIM3->CCMR2 |=  (0b110 << 4);
    TIM3->CCMR2 |=  (1 << 3); // Enable preload

    // Enable channel output for both
    TIM3->CCER |= (1 << 4); // gate 1
    TIM3->CCER |= (1 << 8); // gate 2

    // Both gates at neutral before timer runs
    TIM3->CCR2 = NEUTRAL;
    TIM3->CCR3 = NEUTRAL;

    // Enable auto reload preload and force update to avoid glitch
    TIM3->CR1 |= (1 << 7); // (ARPE)
    TIM3->EGR |= (1 << 0); // (UG)

    // Start the timer (CEN)
    TIM3->CR1 |= (1 << 0);
}

// Wrappers to easily set the pulse width for both gates
void Gate1_SetWidth(uint16_t microseconds) {
    TIM3->CCR2 = microseconds;
}

void Gate2_SetWidth(uint16_t microseconds) {
    TIM3->CCR3 = microseconds;
}