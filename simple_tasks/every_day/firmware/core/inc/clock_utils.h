/**
 * @file clock_utils.h
 * @author Alexey Filippov
 * @brief Utilites for to manage delays and system clocks 
 * @version 1.0
 * @date 2026-09-12
 * 
 */

#ifndef CLOCK_UTILS_H
#define CLOCK_UTILS_H

#define DELAY_FOR_RATTLE 10000      /* Delay in a rattle for detecting pressing */
#define DELAY_FOR_RATTLE_MS 10      /* Miliseconds */
#define CHECKING_TIMES 4            /* Every event from a button is checked four times */

typedef enum {OFF = 0, ON = 1} ButtonStatus;
/**
 * @brief Stores variables for handling of a button pressing
 * @note Delay time is expressed in miliseconds and demands a corresponding configuration TIM1.
 *  It uses TIM1 for doing a delay.
 */
typedef struct {
    unsigned char efforts;          /* Quantity times when button was checked */
    unsigned char retentions;       /* Quantity statuses when button is pressed */
    unsigned int delay_time;        /* Duration of a delay between checkings (in miliseconds) */
    unsigned int ticks;             /* Every miliseconds increments this variable */
    ButtonStatus status;            /* Is it turned OFF or ON */
    enum lock {wantON = 0, wantOFF = 1, nothing = 2} lock_type;  /* A lock for cheching one condition */
} buttonHandler;

extern buttonHandler button2_handler;

/**
 * @brief Delays the program by microseconds
 * 
 * @param mcs any integer positive number (must be more than 0)
 * @return int 
 * @note It is not related to interruptions and does not affect their operation
 */
int waiting_microseconds(unsigned int mcs);

void button_check(uint32_t* button_reg, uint32_t* led_reg, uint32_t button_bit, uint32_t led_bit);

#endif