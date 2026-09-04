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

#include "notes.h"

/**
 * @brief Generate a tone at the specified frequency
 * 
 * Sets TIM2 PWM output to produce a square wave with 50% duty cycle
 * at the requested frequency. Automatically stops and restarts timer.
 * 
 * @param frequency Desired tone frequency in Hz (e.g., 440.0 for A4)
 * @note Valid range: ~1 Hz to 10 kHz (limited by timer resolution)
 * @note If frequency <= 0, function does nothing (safety check)
 */
void set_note(float frequency);

/**
 * @brief Wait (block) for specified number of milliseconds
 * 
 * Uses SysTick interrupt to count milliseconds. This is a busy-wait delay.
 * 
 * @param ms Number of milliseconds to wait
 * @warning Do NOT call from interrupt context (uses SysTick interrupt)
 */
void stop_for_ms(unsigned int ms);

/**
 * @brief Play a simple 4-note test melody (UGCM1205X theme)
 * 
 * Cycles through notes: A4 → C4 → B4 → C4 → repeat
 * Each call advances to the next note (state machine)
 */
void play_simplest_melody(void);

/**
 * @brief Play "Marmotte" melody by Ludwig van Beethoven
 * 
 * Plays a 32-note phrase from Beethoven's composition.
 * Each call plays one note and advances to the next.
 * 
 * @note Includes a short gap (20 ms) between notes for natural sound
 */
void play_marmot(void);

#endif // MAIN_H