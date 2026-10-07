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

#define MAX_NUMBER_OF_BUTTONS 30
#define FULL_DUTY 5                /* Duty cycle is divided by FULL_DUTY parts, every step adds (1/FULL_DUTY)*100% to PWM duty */

typedef enum {OFF = 0, ON = 1} ButtonStatus;

typedef struct  {
    volatile bool front;
    volatile bool decline;
} somethingCome;

/**
 * @brief Every front and decline records to corresponding counter:
 * @example front[button_number]++ if button is pressed and decline[button_number]++ if button isn't pressed
 */
typedef struct {
    volatile uint8_t fronts[MAX_NUMBER_OF_BUTTONS];
    volatile uint8_t declines[MAX_NUMBER_OF_BUTTONS];
    somethingCome from_buttons[MAX_NUMBER_OF_BUTTONS];
} pressingCounter;

/**
 * @brief A set of available buttons
 * @note Some isn't busy
 */
typedef enum {
    FIRST   =   0,
    SECOND  =   1,
    THIRD   =   2,
    FOURTH  =   3,
    FIFTH   =   4,
    SIXTH   =   5
} buttonOrders;

/**
 * @brief Stores variables for handling of a button pressing
 * @note Delay time is expressed in miliseconds and demands a corresponding configuration TIM1.
 *  It uses TIM1 for doing a delay.
 */
typedef struct {
    volatile unsigned char efforts;          /* Quantity times when button was checked */
    volatile unsigned char retentions;       /* Quantity statuses when button is pressed */
    volatile unsigned int delay_time;        /* Duration of a delay between checkings (in miliseconds) */
    volatile unsigned int ticks;             /* Every miliseconds increments this variable */
    volatile ButtonStatus status;            /* Is it turned OFF or ON */
    volatile enum lock {wantON = 0, wantOFF = 1, nothing = 2} lock_type;  /* A lock for cheching one condition */
    volatile bool ischanged;                 /* It's possibly the status was changed */
    volatile buttonOrders name;              /* Serial number of the button */
} buttonHandler;

extern pressingCounter press_count;
extern buttonHandler button1_handler;
extern buttonHandler button2_handler;

/**
 * @brief Delays the program by microseconds
 * 
 * @param mcs any integer positive number (must be more than 0)
 * @return int 
 * @note It is not related to interruptions and does not affect their operation
 */
int waiting_microseconds(unsigned int mcs);

/**
 * @brief Uses TIM4 for waiting
 * 
 * @param ms time for delay in miliseconds must be less than MAX_ULL (~585 million years)
 * @return int 
 */
int delay_by_tim4(unsigned int ms);

void button_check(uint32_t* button_reg, uint32_t* led_reg, uint32_t button_bit, uint32_t led_bit);

#endif