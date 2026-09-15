#include "main.h"

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

void TIM2_Init(void){
    TIM2->ARR = 100;
}
