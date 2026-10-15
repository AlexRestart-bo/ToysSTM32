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

volatile float temperature = 0;

void RCC_config(void){
    /* 2 wait states if 48 MHz < SYSCLK <= 72 MHz */
    //FLASH->ACR = FLASH_ACR_LATENCY_2;

    /* Turns HSE on */
    //RCC->CR |= RCC_CR_HSEON;

    /* Waits HSE is ready */
    //while (!(RCC->CR & RCC_CR_HSERDY));
    
    /* System clock - PLL, multiplication factor for PLL - 7 (HSE = 8MHz -> 8 * 7 = 56 MHz) */
    //RCC->CFGR |= RCC_CFGR_PLLSRC;
    //RCC->CFGR |= RCC_CFGR_PLLMULL7;

    /* Waits PLL is stable */
    //while (!(RCC->CR & RCC_CR_PLLRDY));
    
    /* Configure prescalers for the buses */
    //RCC->CFGR |= RCC_CFGR_HPRE_DIV2;      // AHB = SYSCLK / 2 = 28 MHz
    //RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;     // APB1 = AHB / 1 = 28 MHz (max = 36 MHz)
    //RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;     // APB2 = AHB / 1 = 28 MHz

    /* Switch system clock on PLL */
    //RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait for the end of switching */
    //while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    // Включаем HSI на случай проблем с внешним кварцем
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY));

    FLASH->ACR = FLASH_ACR_LATENCY_2;

    /* Попробуйте запустить HSE и дождаться флага */
    RCC->CR |= RCC_CR_HSEON;
    
    // Если виснет здесь — значит кварц на плате не отвечает или поврежден
    while (!(RCC->CR & RCC_CR_HSERDY)); 
    
    // ... остальной код настройки PLL
}

void Enable_Clocks(void){
    /* Enable ADC1 clock */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
}

void ADC1_Init(void){

    /* One conversion is chose */
    ADC1->SQR1 &= ~ADC_SQR1_L;
    ADC1->SQR3 |= (16 << ADC_SQR3_SQ1_Pos);

    /* The recommended sampling time for the temperature sensor is 17.1 mcs */
    ADC1->SMPR1 |= ADC_SMPR1_SMP16;     /* 239.5 + 12.5 = 252 cycles (14 MHz -> 18 mcs) */

    /* Right alignment */
    ADC1->CR2 &= ~ADC_CR2_ALIGN;

    /* Continious conversion mode */
    ADC1->CR2 |= ADC_CR2_CONT;

    /* Enable external trigger (SWSTART) */
    ADC1->CR2 |= ADC_CR2_EXTTRIG | ADC_CR2_EXTSEL;

    /* Enable interrupt (the end of conversion) */
    ADC1->CR1 |= ADC_CR1_EOCIE;

    /* Turn temperature sensor on and turn his power on */
    ADC1->CR2 |= ADC_CR2_TSVREFE | ADC_CR2_ADON;

    /* Need at least two free cycles before calibration */
    ADC1->CR2 |= ADC_CR2_CAL;

    /* Wait for the end of the calibration */
    while (READ_BIT(ADC1->CR2, ADC_CR2_CAL));

    NVIC_EnableIRQ(ADC1_2_IRQn);

}

float get_tempsensor_value(void){

    uint16_t data_sense = ADC1->DR & 0xFFF;    /* Get lower 12 bits (max_value = 0xFFF), reading Data Register clears EOC bit in Status Register */

    float v_sense = 3.3 * (float)data_sense / (float)0xFFF;

    return (float)(((PRIMARY_VOLT - v_sense) / AVG_SLOPE) + PRIMARY_TEMP);
}

int main(void){
    RCC_config();
    Enable_Clocks();
    ADC1_Init();

    /* Start convertions */
    ADC1->CR2 |= ADC_CR2_SWSTART;

    while(1);

    return 0;
}

void ADC1_2_IRQHandler(void){
    if (READ_BIT(ADC1->SR, ADC_SR_EOC)){
        temperature = get_tempsensor_value(); /* ADC_SR_EOC bit is clened after reading DR */
    }
}
