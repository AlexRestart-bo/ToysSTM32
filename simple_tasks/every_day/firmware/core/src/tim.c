#include "main.h"

#define POINT_PULSE 100     /* The value for TIM2->CCRx register must be less than ARR_VALUE_TIM2 and more than 0 */

volatile unsigned long long tim4_ticks = 0;

void TIM1_Init(void){       // 1 kHz
    TIM1->ARR = ARR_VALUE - 1;
    TIM1->PSC = PSC_VALUE - 1;
    TIM1->DIER = TIM_DIER_UIE;
    TIM1->SR &= ~TIM_SR_UIF;
    TIM1->CNT = 0;
    TIM1->CR1 = TIM_CR1_CEN;
    NVIC_SetPriority(TIM1_UP_IRQn, 3);
    NVIC_EnableIRQ(TIM1_UP_IRQn);
}

void TIM2_Init(void){
    TIM2->ARR = ARR_VALUE_TIM2 - 1;
    TIM2->PSC = PSC_VALUE_TIM2 - 1;

    TIM2->CCR4 = POINT_PULSE;
    TIM2->CCMR2 |= TIM_CCMR2_OC4M | TIM_CCMR2_OC4PE;        /* Need PWM mode 2, because it's high if CNT < CCRx */
    TIM2->CCER |= TIM_CCER_CC4E;

    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_OPM | TIM_CR1_ARPE;    /* One pulse mode and auto-reload preload enable */
    TIM2->EGR |= TIM_EGR_UG;
}

void TIM3_Init(void){
    TIM3->ARR = ARR_VALUE_TIM3 - 1;
    TIM3->PSC = PSC_VALUE_TIM3 - 1;
    //PB0
    TIM3->CCR3 = 0;    // Duty cycle is 0 at the beginning
    TIM3->CCMR2 &= ~TIM_CCMR2_OC3M;     // Third channel is TIM3_CH3
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1;     // PWM mode 1 (upcounting)
    TIM3->CCMR2 |= TIM_CCMR2_OC3PE;
    TIM3->CCER |= TIM_CCER_CC3E;    /* Turn OC3 signal on */
    // PB1
    TIM3->CCR4 = 0;
    TIM3->CCMR2 &= ~TIM_CCMR2_OC4M;     // Fourth channel is TIM3_CH4
    TIM3->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE;
    TIM3->CCER |= TIM_CCER_CC4E | TIM_CCER_CC4P;    // Invertion refers to TIM3_CH3

    TIM3->CR1 |= TIM_CR1_ARPE;  /* Enable auto-reload preload */
    TIM3->EGR |= TIM_EGR_UG;    /* Reset counter */
    TIM3->CR1 |= TIM_CR1_CEN;
}

void TIM4_Init(void){
    TIM4->ARR = ARR_VALUE_TIM4 - 1;
    TIM4->PSC = PSC_VALUE_TIM4 - 1;
    TIM4->CNT = 0;
    TIM4->SR &= ~TIM_SR_UIF;
    TIM4->DIER = TIM_DIER_UIE;

    TIM4->EGR |= TIM_EGR_UG;
    NVIC_SetPriority(TIM4_IRQn, 1);
    NVIC_EnableIRQ(TIM4_IRQn);
    TIM4->CR1 = TIM_CR1_CEN;
}
