#ifndef ASSIGNMENT_1_H
#define ASSIGNMENT_1_H

typedef enum {
    OFF = 0,
    RED,
    GREEN,
    BLUE
} ItemColour_t;

ItemColour_t Apply_LFSR_Fault(ItemColour_t true_col);
void Generate_Next_Item(void);
void SysTick_Init(void);

// Declaring extra variables and functions use in assigment1.c code (moved to main)
volatile ItemColour_t truth_colour;
volatile ItemColour_t sensor_colour;
void Set_Truth_RGB(ItemColour_t colour);
void Set_Sensor_RGB(ItemColour_t colour);

#endif