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
void SysTick_Init(void);

int waiting_microseconds(unsigned int mcs);
static void run_lights(void);

int main(void) {
    RCC_config();
    Enable_Clocks();
    GPIO_Config();
    TIM1_Init();

    SysTick_Init();

    while(1){
        //waiting_microseconds(100'000'000);
        //GPIOC->ODR ^= GPIO_ODR_ODR13;
        run_lights();
    }
}

void Enable_Clocks(void){
    /* Enable TIM1, GPIOC  and GPIOA clock */
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN | RCC_APB2ENR_IOPAEN; // | RCC_APB2ENR_IOPCEN 
}

/**
 * @brief Configurates GPIOs
 * @note PC13 can be configurated as open drain (it is at the board - Blue Pill), any other demands push-pull for LEDs
 */
void GPIO_Config(void){
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);      // Cleans bits PC13 in register CRH
    GPIOC->CRH |= (GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0);  // Output mode, max speed 10 MHz, open drain
    // PA0
    GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    GPIOA->CRL |= GPIO_CRL_MODE0_1;
    // PA1
    GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);
    GPIOA->CRL |= GPIO_CRL_MODE1_1;
    // PA2
    GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);
    GPIOA->CRL |= GPIO_CRL_MODE2_1;

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
    //GPIOC->ODR ^= GPIO_ODR_ODR13;
}

void SysTick_Init(void){
    /* In this configuration AHB frequency the same system frequency generates PLL (72 MHz) */
    SysTick->LOAD = BOARD_SYSCLK / MICROINSEC - 1;
    /* Clock source is AHB without prescaler */
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

    SysTick->VAL = 0;
    
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

/*
static int powint(int a, int b){
    int out = 1;
    if (b < 0) return -1;
    
    while (b--){
        out *= a; 
    }

    return out;
}
*/

int waiting_microseconds(unsigned int mcs){
    /* Max value for 24-bit register 2^24-1 */
    unsigned long load_value = SYSTICK_TICKS_PER_US*mcs;

    unsigned int total_period = 0;
    unsigned int fract_period = mcs;    /* temporary all in fract_period */

    if (load_value > SYSTICK_MAX){
        total_period = fract_period / MILIINSEC;        /* Number of miliseconds */
        fract_period = fract_period % MILIINSEC;        /* Number of microseconds */
    }

    while(total_period--){
        /* BOARD_SYSCLK / MICROINSEC = 72000000 / 1000000 = 72 for AHB frequency 72 MHz */
        SysTick->LOAD = SYSTICK_TICKS_PER_MS;
        /* Turns counter on */
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
        /* If timer counted to 0 it became 1 (COUNTFLAG) */
        while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != SysTick_CTRL_COUNTFLAG_Msk);
        (void)SysTick->CTRL;
        /* Turns counter off */
        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    }
    /* BOARD_SYSCLK / MICROINSEC = 72000000 / 1000000 = 72 for AHB frequency 72 MHz */
    SysTick->LOAD = fract_period*SYSTICK_TICKS_PER_US;
    /* Turns counter on */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    /* If timer counted to 0 it became 1 (COUNTFLAG) */
    while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != SysTick_CTRL_COUNTFLAG_Msk);
    (void)SysTick->CTRL;
    /* Turns counter off */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

    SysTick->LOAD = 0;

    return 0;
}

/**
 * @brief Torches LEDs consistantly (touches PA0, PA1, PA2)
 *      LEDs are connected in series with resistors (330)
 */
static void run_lights(void){
    static uint8_t led_order = 0;

    switch (led_order)
    {
    case 0:
        CLEAR_BIT(GPIOA->ODR, GPIO_ODR_ODR1);
        CLEAR_BIT(GPIOA->ODR, GPIO_ODR_ODR2);
        SET_BIT(GPIOA->ODR, GPIO_ODR_ODR0);
        break;

    case 1:
        CLEAR_BIT(GPIOA->ODR, GPIO_ODR_ODR0);
        CLEAR_BIT(GPIOA->ODR, GPIO_ODR_ODR2);
        SET_BIT(GPIOA->ODR, GPIO_ODR_ODR1);
        break;

    case 2:
        CLEAR_BIT(GPIOA->ODR, GPIO_ODR_ODR0);
        CLEAR_BIT(GPIOA->ODR, GPIO_ODR_ODR1);
        SET_BIT(GPIOA->ODR, GPIO_ODR_ODR2);
        break;

    default:
        break;
    }

    led_order = (led_order + 1) % LEDS;

    waiting_microseconds(300'000);
}
