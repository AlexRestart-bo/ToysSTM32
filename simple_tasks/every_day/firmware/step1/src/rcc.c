#include "main.h"

/**
 * @brief Configures SYSCLK how 72 MHz
 */
void RCC_config(void)
{
    /* 2 wait states for 72 MHz */
    FLASH->ACR = FLASH_ACR_LATENCY_2;

    /* Turns HSE on */
    RCC->CR |= RCC_CR_HSEON;
    
    /* Waits HSE is ready */
    while (!(RCC->CR & RCC_CR_HSERDY));
    
    /* System clock - PLL, multiplication factor for PLL - 9 (HSE = 8MHz -> 8 * 9 = 72 MHz) */
    RCC->CFGR |= RCC_CFGR_PLLSRC;
    RCC->CFGR |= RCC_CFGR_PLLMULL9;
    
    /* Turns PLL on */
    RCC->CR |= RCC_CR_PLLON;

    /* Waits PLL is stable */
    while (!(RCC->CR & RCC_CR_PLLRDY));
    
    /* Configure prescalers for the buses */
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;      // AHB = SYSCLK / 1 = 72 MHz
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;     // APB1 = AHB / 2 = 36 MHz (max = 36 MHz)
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;     // APB2 = AHB / 1 = 72 MHz
    
    /* Switch system clock on PLL */
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait for the end of switching */
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}
