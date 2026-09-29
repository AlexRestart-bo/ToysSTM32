#include "main.h"

void TIM1_Init(void){       // 1 kHz
    TIM1->ARR = ARR_VALUE - 1;
    TIM1->PSC = PSC_VALUE - 1;
    TIM1->DIER = TIM_DIER_UIE;
    TIM1->SR &= ~TIM_SR_UIF;
    TIM1->CNT = 0;
    TIM1->CR1 = TIM_CR1_CEN;
    NVIC_SetPriority(TIM1_UP_IRQn, 2);
    NVIC_EnableIRQ(TIM1_UP_IRQn);
}

void TIM2_Init(void){
    TIM2->ARR = ARR_VALUE_TIM2 - 1;
    TIM2->PSC = PSC_VALUE_TIM2 - 1;
    TIM2->DIER = TIM_DIER_UIE;
    TIM2->SR &= ~TIM_SR_UIF;
    TIM2->CNT = 0;
    TIM2->CR1 = TIM_CR1_CEN;
    NVIC_SetPriorityGrouping(2);
    NVIC_SetPriority(TIM2_IRQn, 3);
    NVIC_EnableIRQ(TIM2_IRQn);
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
    TIM3->CCER |= TIM_CCER_CC4E; //| TIM_CCER_CC4P;    // Invertion refers to TIM3_CH3

    TIM3->CR1 |= TIM_CR1_ARPE;  /* Enable auto-reload preload */
    TIM3->EGR |= TIM_EGR_UG;    /* Reset counter */
    TIM3->CR1 |= TIM_CR1_CEN;
}
