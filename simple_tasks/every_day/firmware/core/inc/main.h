/**
 * @file main.h
 * @brief Main application header for PWM tone generator
 * 
 * Hardware: STM32F103, PA0 - PWM output (speaker)
 * Provides tone generation and melody playback functions.
 */

#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "stm32f1xx.h"
#include "stm32f103xb.h"
#include "system_stm32f1xx.h"

#define BOARD_SYSCLK 72000000   /* SYSCLK configured as 72 MHz */
#define MICROINSEC 1000000      /* Number microseconds in a second */
#define MILIINSEC 1000          /* Number miliseconds in a second */
#define SYSTICK_TICKS_PER_US (BOARD_SYSCLK/MICROINSEC)      /* Ticks per microsecond (72) */
#define SYSTICK_TICKS_PER_MS (BOARD_SYSCLK/MILIINSEC)       /* Ticks per milisecond (72000) */


#define SYSTICK_MAX 0xFFFFFF    /* The limit for 24-bit register */

void RCC_config(void);

#endif // MAIN_H