#include "main.h"

#define PSC_VALUE 400

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
        //GPIOC->ODR ^= GPIO_ODR_ODR13;
        //for(int i = 0; i < 400000; i++);
        play_marmot();
    }
}

void Enable_Clocks(void){
    // GPIO clocks
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN;  // PORT C and A if they base on APB2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // enable TIM2 clock
}
    
void GPIO_Config(void){
    GPIOC->CRH |= (1<<21);  // configures PC13 pin
    // Очищаем биты для PA0 в регистре CRL
    GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    // Устанавливаем: скорость 50 МГц, режим Alternate Function Push-Pull
    GPIOA->CRL |= (GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0); // Режим работы (Output mode, max speed 50 MHz)
    GPIOA->CRL |= GPIO_CRL_CNF0_1;                      // Альтернативная функция (Alternate function output)
    GPIOA->CRL &= ~GPIO_CRL_CNF0_0;                     // Push-Pull
}

void TIM2_Init(void){
    TIM2->ARR = 0;
    TIM2->PSC = PSC_VALUE - 1; // if frequency of APB2 is 8 MHz, MAX = 20 KHz
    TIM2->CCR1 = 0;
    TIM2->CCMR1 = (0b110 << 4);
    TIM2->CCER |= TIM_CCER_CC1E;
    TIM2->CR1 = TIM_CR1_CEN;
}

void SysTick_Init(void){
    SysTick->LOAD = 999;    // AHB/8
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;     // Do not use core clock
}

void set_note(float frequency){
    unsigned long int x = 8000000.0 / (frequency * PSC_VALUE);
    if (x > 19000) x = 19000;
    TIM2->CR1 &= ~(1<<0);
    TIM2->CNT = 0;
    TIM2->ARR = x - 1;
    TIM2->CCR1 = ( x % 2 ) ? x / 2 : x / 2 - 1;
    TIM2->CR1 = TIM_CR1_CEN;
}

void stop_for_ms(unsigned int ms){
    ms_tick = 0;
    while(ms_tick < ms);
}

void SysTick_Handler(void){
    ms_tick++;
}
