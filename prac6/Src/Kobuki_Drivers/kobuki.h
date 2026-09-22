#ifndef KOBUKI_H
#define KOBUKI_H
#include <stdint.h>

// Kobuki structure
typedef struct {
    uint8_t cliff;
    uint8_t bumper;
    uint8_t wheeldrop;
    uint8_t button;
} Kobuki_Typedef;

// Define buffer for feedback
#define SIZE_FEEDBACK 14

// Buffer definitions for payload (offsets into the Basic Sensor Data payload,
// confirmed against the Kobuki serial bytestream protocol spec)
#define BUMPER 2
#define WHEEL_DROP 3
#define CLIFF 4
#define BUTTON 11

// Truncate speed
#define SPEED_LIMIT (int16_t) 150 // mm/s

// Receive payload
uint8_t Kobuki_Rx(uint8_t *feedback, uint32_t size_feedback);

// Transmit payload
void Kobuki_Tx(uint8_t *payload, uint8_t size_payload);

// Kobuki receive payload and store
void Kobuki_Read(Kobuki_Typedef *kobuki);

// Kobuki drive forward/backward (+ve is forward)
void Kobuki_Drive(int16_t speed);

// Kobuki rotate CW/CCW (+ve is CCW)
void Kobuki_Rotate(int16_t speed);

#endif