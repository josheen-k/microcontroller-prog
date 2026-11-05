#include "my_stm_helper.h"
#include "config.h"
#include <math.h>

// convert a servo angle in deg to pulse width in us (micro sec)
uint16_t Angle_To_Pulse(float angle_deg) {
    if (angle_deg < 0.0f) {
        angle_deg = 0.0f;
    }
    if (angle_deg > 180.0f) {
        angle_deg = 180.0f;
    }
    return (uint16_t)(SERVO_PULSE_MIN + angle_deg* (SERVO_PULSE_MAX - SERVO_PULSE_MIN)/180.0f);
}

// reads 3 accelometr axis in g, returns false if i2c read failed
bool Read_Accel(float *ax, float *ay, float *az) {
    uint8_t buf[6];
    if (!I2C_Master_ReadRegs(I2C1, MPU_ADDR, REG_ACCEL_XOUT_H, buf, 6)) {
        return false;
    }
    *ax = (int16_t)((buf[0] << 8) | buf[1]) / ACCEL_SENS;
    *ay = (int16_t)((buf[2] << 8) | buf[3]) / ACCEL_SENS;
    *az = (int16_t)((buf[4] << 8) | buf[5]) / ACCEL_SENS;
    return true;
}

// roll, pitch and tilt angle, all in degrees
void Calc_Angles(float ax, float ay, float az, float *roll, float *pitch, float *tilt) {
    *roll  = atan2f(ay, az) * RAD_TO_DEG; 
    *pitch = atan2f(-ax, sqrtf(ay*ay + az*az)) * RAD_TO_DEG;
    *tilt  = sqrtf((*pitch) * (*pitch) + (*roll) * (*roll));
}

// sets up all peripherals used by the controller
void Master_Init(void) {
    // enable clocks
    Clock_Enable(GPIOA_BUS);
    Clock_Enable(GPIOB_BUS);
    Clock_Enable(TIM3_BUS); // servo pwm
    Clock_Enable(TIM16_BUS); // i2c timeout clock
    Clock_Enable(TIM17_BUS); // 20ms control loop tick
    Clock_Enable(I2C1_BUS);

    // i2c pins: PB8=SCL, PB9=SDA, af no.6 
    GPIO_InitPin(GPIOB, 8, ALT, NONE);
    GPIO_SetAltFcn(GPIOB, 8, 6);
    GPIO_InitPin(GPIOB, 9, ALT, NONE);
    GPIO_SetAltFcn(GPIOB, 9, 6);

    // servo pins on TIM3 , af1
    GPIO_InitPin(GPIOB, 4, ALT, NONE);  
    GPIO_SetAltFcn(GPIOB, 4, 1); // roll (CH1)
    GPIO_InitPin(GPIOB, 5, ALT, NONE);
    GPIO_SetAltFcn(GPIOB, 5, 1); // pitch(CH2)
    GPIO_InitPin(GPIOB, 0, ALT, NONE);
    GPIO_SetAltFcn(GPIOB, 0, 1); // tilt (CH3)

    // leds and buytton
    GPIO_InitPin(GPIOA, LED_AUTO, OUTPUT, NONE);
    GPIO_InitPin(GPIOB, LED_MANUAL, OUTPUT, NONE);
    GPIO_InitPin(GPIOA, AUTO_BUTTON, INPUT, PULL_DOWN);
    GPIO_OutputClearPin(GPIOA, LED_AUTO);
    GPIO_OutputSetPin(GPIOB, LED_MANUAL); //start in manual

    // timers
    Timer_Init(TIM16, TIM16_IRQn, 47999, 0xFFFF, false); // 1ms tick for i2c timeouts
    Timer_Init(TIM3, TIM3_IRQn, 47, 19999, false); // 1 mhz tick, 20ms period= 50hz pwm
    TIM3_PWM1_Init(SERVO_ROLL, Angle_To_Pulse(90.0f)); // roll neutral 90deg
    TIM3_PWM1_Init(SERVO_PITCH, Angle_To_Pulse(90.0f)); // pitch neutral 90deg
    TIM3_PWM1_Init(SERVO_TILT, Angle_To_Pulse(0.0f)); // tilt neutral 0deg

    // I2C: 100khz (presc=11, sclh = scll =19), then wake mpu
    I2C_Master_Init(I2C1, 11, 19, 19);
    I2C_Master_WriteReg(I2C1, MPU_ADDR, REG_PWR_MGMT_1, 0x00);

    // 20ms control loop tick: PSC=47999 (1ms), ARR=19 (20 tciks)
    Timer_Init(TIM17, TIM17_IRQn, 47999, 19, true);
}