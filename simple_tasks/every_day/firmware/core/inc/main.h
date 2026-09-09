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
#include "stm32f1xx.h"
#include "stm32f103xb.h"
#include "system_stm32f1xx.h"

void RCC_config(void);

#endif // MAIN_H