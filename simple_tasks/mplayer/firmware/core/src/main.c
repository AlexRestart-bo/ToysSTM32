/**
 * @file main.c
 * @brief PWM tone generator for STM32F103
 * 
 * Hardware: PA0 - PWM output (speaker), PC13 - button (optional)
 * Uses TIM2 in PWM mode 1, SysTick for 1ms delays.
 * Frequencies are calculated for 8 MHz APB1 clock.
 */

#include "main.h"

#define PSC_VALUE 400           // Timer prescaler: 8 MHz / 400 = 20 kHz timer clock
#define SYSTICK_LOAD 1000       // For 1 ms tick at 8 MHz/8 = 1 MHz SysTick clock
#define MAX_ARR 19000           // Max auto-reload (limit for human ear ~ 1 Hz)
#define MIN_ARR 2               // Min auto-reload 

volatile unsigned long int ms_tick = 0;

void Enable_Clocks(void);
void GPIO_Config(void);
void TIM2_Init(void);
void SysTick_Init(void);
void set_note(float frequency);
void stop_for_ms(unsigned int ms);

int main(void) {
    Enable_Clocks();
    GPIO_Config();
    TIM2_Init();
    SysTick_Init();
    set_note(C4);
    while(1) {
        // It is a melody from "melodies.c"
        //play_marmot();
        for (volatile int i = 0; i < 1000000; i++);

        GPIOC->ODR ^= GPIO_ODR_ODR13;
    }
}

void Enable_Clocks(void){
    // GPIO clocks
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;     // PORT A base on APB 
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;     // enable TIM2 clock
}
    
void GPIO_Config(void){
    GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);        // Cleans bits PA0 in register CRL
    GPIOA->CRL |= (GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0);    // Output mode, max speed 50 MHz
    GPIOA->CRL |= GPIO_CRL_CNF0_1;                          // Alternate function output
    GPIOA->CRL &= ~GPIO_CRL_CNF0_0;                         // Push-Pull for PWM

    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_1;                        // Output mode, max speed 50 MHz, push-pull
}

void TIM2_Init(void){
    TIM2->ARR = 0;                  // Firstly it works without intermissions
    TIM2->PSC = PSC_VALUE - 1;      // if frequency of APB2 is 8 MHz (by default) , MAX = 20 KHz
    TIM2->CCR1 = 0;                 // That sets duty cycle of impulses
    TIM2->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;   // PWM mode 1 (0b110 << 4)
    TIM2->CCER |= TIM_CCER_CC1E;    // OC1 signal is output on the corresponding output pin, polarity is by default (upcounting)
    TIM2->CR1 = TIM_CR1_CEN;        // Enables counter
}

/* Event occurs every milisecond (SysTick_Handler calls) */
void SysTick_Init(void){
    SysTick->LOAD = SYSTICK_LOAD - 1;    // AHB/8
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;     // Do not use core clock
}

/**
 * @brief Turns new note on
 * @param frequency 
 * @return * void 
 */
void set_note(float frequency){
    // Timer frequency = 20 kHz (PSC=400), so ARR = 20000 / frequency
    // Example: C4 (261.6 Hz) → ARR ≈ 76
    unsigned long int x = 8000000.0 / (frequency * PSC_VALUE);  // Calculates a value for registers ARR and CCR1

    if (x > MAX_ARR) x = MAX_ARR;       // Prevent overflow (ARR is 16-bit)
    if (x < MIN_ARR) x = MIN_ARR;       // Prevent too high frequency (>10 kHz)

    TIM2->CR1 &= ~TIM_CR1_CEN;                      // Stops counting
    TIM2->CNT = 0;                                  // There is fogotten to throw shadow registers off 
    TIM2->ARR = x - 1;
    TIM2->CCR1 = ( x % 2 ) ? x / 2 : x / 2 - 1;     // Half of x value
    TIM2->CR1 = TIM_CR1_CEN;                        // Turns counter on
}

/**
 * @brief Does intermission in miliseconds
 * @param ms is quantity of miliseconds to wait for
 * @return * void 
 */
void stop_for_ms(unsigned int ms){
    ms_tick = 0;
    while(ms_tick < ms);
}

void SysTick_Handler(void){
    ms_tick++;
}
