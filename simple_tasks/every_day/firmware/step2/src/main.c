/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2026-10-07
 * 
 * @copyright Copyright (c) 2026
 * 
 * Obtain the temperature using the following formula:
    Temperature (in °C) = {(V25 - VSENSE) / Avg_Slope} + 25.
    Where,
    V25 = VSENSE value for 25° C and
    Avg_Slope = Average Slope for curve between Temperature vs. VSENSE (given in
    mV/° C or μV/ °C).
    Refer to the Electrical characteristics section for the actual values of V25 and
    Avg_Slope.
 */
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

void RCC_config(void){
    /* 2 wait states if 48 MHz < SYSCLK <= 72 MHz */
    FLASH->ACR = FLASH_ACR_LATENCY_2;

    /* Turns HSE on */
    RCC->CR |= RCC_CR_HSEON;

    /* Waits HSE is ready */
    while (!(RCC->CR & RCC_CR_HSERDY));
    
    /* System clock - PLL, multiplication factor for PLL - 7 (HSE = 8MHz -> 8 * 7 = 56 MHz) */
    RCC->CFGR |= RCC_CFGR_PLLSRC;
    RCC->CFGR |= RCC_CFGR_PLLMULL7;

    /* Waits PLL is stable */
    while (!(RCC->CR & RCC_CR_PLLRDY));
    
    /* Configure prescalers for the buses */
    RCC->CFGR |= RCC_CFGR_HPRE_DIV2;      // AHB = SYSCLK / 2 = 28 MHz
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;     // APB1 = AHB / 1 = 28 MHz (max = 36 MHz)
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;     // APB2 = AHB / 1 = 28 MHz

    /* Switch system clock on PLL */
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait for the end of switching */
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

void Enable_Clocks(void){
    /* Enable ADC1 clock */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
}

void SysTick_Init(void){
    /* In this configuration AHB frequency the same system frequency generates PLL (72 MHz) */
    SysTick->LOAD = BOARD_SYSCLK / MICROINSEC - 1;
    /* Clock source is AHB without prescaler */
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

    SysTick->VAL = 0;
    
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

void ADC1_Init(void){

    /* One conversion is chose */
    ADC1->SQR1 &= ~ADC_SQR1_L;

    /* The recommended sampling time for the temperature sensor is 17.1 mcs */
    ADC1->SMPR1 |= ADC_SMPR1_SMP16;     /* 239.5 + 12.5 = 252 cycles (14 MHz -> 18 mcs) */

    /* Turn temperature sensor on */
    ADC1->CR2 |= ADC_CR2_TSVREFE;

    /* Right alignment */
    ADC1->CR2 &= ~ADC_CR2_ALIGN;

    /* Enable external trigger */
    ADC1->CR2 |= ADC_CR2_EXTTRIG;

    /* Ebable software trigger */
    //ADC1->CR2 |= ADC_CR2_ADON;
    //__NOP();    /* Need at least two free cycles before calibration */
    //__NOP();

    //ADC1->CR2 |= ADC_CR2_CAL;

    /* Wait for the end of the calibration */
    //while (READ_BIT(ADC1->CR2, ADC_CR2_CAL));ADC1->CR2 |= ADC_CR2_EXTSEL;

    /* Turn power of the ADC on */


}

float get_tempsensor_value(void){
    
}

int main(void){
    RCC_config();
    Enable_Clocks();
    SysTick_Init();
    ADC1_Init();

    float temperature = 0;

    while(1){
        ADC1->CR2 |= ADC_CR2_SWSTART;

        while(!READ_BIT(ADC1->SR, ADC_SR_EOC));

        temperature = get_tempsensor_value();
    }

    return 0;
}
