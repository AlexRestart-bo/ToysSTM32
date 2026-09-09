/**
 * @file main.c
 * @brief PWM tone generator for STM32F103
 * 
 * Hardware: PA0 - PWM output (speaker), PC13 - button (optional)
 * Uses TIM2 in PWM mode 1, SysTick for 1ms delays.
 * Frequencies are calculated for 8 MHz APB1 clock.
 */

#include "main.h"

#define PSC_VALUE 10000         // Timer prescaler: 1 MHz / 1000 = 1 kHz timer clock
#define ARR_VALUE 7200          // Auto-reload: if SYSCLK = 72 MHz (APB2 -> TIM1)


void Enable_Clocks(void);
void GPIO_Config(void);
void TIM1_Init(void);

int main(void) {
    RCC_config();
    Enable_Clocks();
    GPIO_Config();
    TIM1_Init();

    while(1){
        for (volatile int i = 0; i < 1000000; i++);
        //GPIOC->ODR ^= GPIO_ODR_ODR13;
    }
}

void Enable_Clocks(void){
    // GPIO clocks
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN | RCC_APB2ENR_IOPCEN;     // enable TIM1 and GPIOC clock
}
    
void GPIO_Config(void){
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);      // Cleans bits PC13 in register CRH
    GPIOC->CRH |= (GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0);  // Output mode, max speed 2 MHz, push-pull
}

void TIM1_Init(void){
    TIM1->ARR = ARR_VALUE - 1;
    TIM1->PSC = PSC_VALUE - 1;
    TIM1->DIER = TIM_DIER_UIE;
    TIM1->SR &= ~TIM_SR_UIF;
    TIM1->CNT = 0;
    TIM1->CR1 = TIM_CR1_CEN;
    NVIC_SetPriority(TIM1_UP_IRQn, 1);
    NVIC_EnableIRQ(TIM1_UP_IRQn);
}

void TIM1_UP_IRQHandler(void){
    TIM1->SR &= ~(TIM_SR_UIF);
    GPIOC->ODR ^= GPIO_ODR_ODR13;
}


