#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>

// mpu
#define MPU_ADDR 0x68 // 7-bit address (AD0 low)
#define REG_PWR_MGMT_1 0x6B // Power management register (write 0 to wake)
#define REG_ACCEL_XOUT_H 0x3B // First of 6 accelerometer data bytes
#define ACCEL_SENS 16384.0f // LSB per g at the default +/-2 g range
#define RAD_TO_DEG 57.2957795f // Convert radians to degrees

// servo pulse width
#define SERVO_PULSE_MIN 500 // 0 deg
#define SERVO_PULSE_MAX 2500 // 180 deg

// pin allocation
#define SERVO_ROLL 1 // PB4, TIM3_CH1
#define SERVO_PITCH 2 // PB5, TIM3_CH2
#define SERVO_TILT 3 // PB0, TIM3_CH3
#define LED_AUTO 2 // PA2, autopilot led
#define LED_MANUAL 11 // PB11, manual led
#define AUTO_BUTTON 12 // PA12, autopilot button, pulldown

// state thresholds in deg (entry = going up, exit = coming down, 5deg deadband)
// roll and pitch
#define AXIS_COMPLIANT_IN 30.0f // free->compliant when angle >=30
#define AXIS_CLAMPED_IN 50.0f // compliant->clamped when angl >=50
#define AXIS_COMPLIANT_OUT 45.0f // clamped->compliant when angle<45
#define AXIS_FREE_OUT 25.0f // compliant->free when angle< 25
// tilt
#define TILT_COMPLIANT_IN 20.0f // free->compliant when tilt >=20
#define TILT_CLAMPED_IN 70.0f // compliant->clamped when tilt >=70
#define TILT_COMPLIANT_OUT 65.0f // clamped->compliant when til <65
#define TILT_FREE_OUT 15.0f // compliant->free when tilt <15

void Master_Init(void); // clocks, pins, timers, pwm, i2c, wakes mpu
bool Read_Accel(float *ax, float *ay, float *az);
void Calc_Angles(float ax, float ay, float az, float *roll, float *pitch, float *tilt);
uint16_t Angle_To_Pulse(float angle_deg);

#endif