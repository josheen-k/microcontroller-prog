#ifndef PWM_HELPER_H
#define PWM_HELPER_H
#include "stm32c0xx.h"
#include <stdint.h>

// Servo pulse widths in ms, calculated using pulse = 500 + (angle/180) * 2000
#define NEUTRAL 1500 // 90 deg (neutral position)
#define ANTI_CLOCK 1000 // 45 deg (45 anti = neutral-45 = 90-45)
#define CLOCK_WISE 2000 // 135 deg (45 clockwise = neutral+45 = 90+45)

// Configures tim3 for 50Hz pwm
void PWM_Init(void);

// Sets gate 1 pulse width
void Gate1_SetWidth(uint16_t microseconds);

// Sets gate 2 pulse width
void Gate2_SetWidth(uint16_t microseconds);

#endif