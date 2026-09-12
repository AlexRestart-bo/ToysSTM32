#include "main.h"

/**
 * @brief Delays the program by microseconds
 * 
 * @param mcs any integer positive number (must be more than 0)
 * @return int 
 * @note It is not related to interruptions and does not affect their operation
 */
int waiting_microseconds(unsigned int mcs){
    
    SystemCoreClockUpdate();    /* Gets of the board frequency independently */

    int ticks_per_us = SystemCoreClock / MICROINSEC;    /* Ticks per microsecond */
    int ticks_per_ms = SystemCoreClock / MILIINSEC;     /* Ticks per milisecond */

    /* Max value for 24-bit register 2^24-1 */
    unsigned long load_value = ticks_per_us*mcs;

    unsigned int total_period = 0;
    unsigned int fract_period = mcs;    /* temporary all in fract_period */

    if (load_value > SYSTICK_MAX){
        total_period = fract_period / MILIINSEC;        /* Number of miliseconds */
        fract_period = fract_period % MILIINSEC;        /* Number of microseconds */
    }

    while(total_period--){
        SysTick->LOAD = ticks_per_ms;

        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
        /* If timer counted to 0 it became 1 (COUNTFLAG) */
        while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != SysTick_CTRL_COUNTFLAG_Msk);
        /* Reading CTRL clears COUNTFLAG. */
        (void)SysTick->CTRL;

        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    }
    /* BOARD_SYSCLK / MICROINSEC = 72000000 / 1000000 = 72 for AHB frequency 72 MHz */
    SysTick->LOAD = fract_period * ticks_per_us;

    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != SysTick_CTRL_COUNTFLAG_Msk);
    (void)SysTick->CTRL;
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

    SysTick->LOAD = 0;

    return 0;
}