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
    TIM2->ARR = ARR_VALUE_APB1 - 1;
    TIM2->PSC = PSC_VALUE_TIM2 - 1;
    TIM2->DIER = TIM_DIER_UIE;
    TIM2->SR &= ~TIM_SR_UIF;
    TIM2->CNT = 0;
    TIM2->CR1 = TIM_CR1_CEN;
    NVIC_SetPriorityGrouping(2);
    NVIC_SetPriority(TIM2_IRQn, 3);
    NVIC_EnableIRQ(TIM2_IRQn);
}
