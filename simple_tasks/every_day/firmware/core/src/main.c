/**
 * @file main.c
 * @brief PWM tone generator for STM32F103
 * 
 * Hardware: PA0 - PWM output (speaker), PC13 - button (optional)
 * Uses TIM2 in PWM mode 1, SysTick for 1ms delays.
 * Frequencies are calculated for 8 MHz APB1 clock.
 */

#include "main.h"

void Enable_Clocks(void);
void GPIO_Config(void);
void SysTick_Init(void);
void EXTI_Config(void);

static void run_lights(void);

/* Status of the buttons: true - pressed, false - no pressed */
volatile bool button1_event = false;
volatile bool button2_event = false;

int main(void) {
    RCC_config();
    Enable_Clocks();
    GPIO_Config();
    TIM1_Init();
    TIM2_Init();
    EXTI_Config();
    SysTick_Init();

    while(1){
        //waiting_microseconds(1'000'000);
        //GPIOC->ODR ^= GPIO_ODR_ODR13;
        //run_lights();
        //if (button1_event){      /* good approach for handling a pressing the button */
        //    button1_event = false;
        //    button_check(&GPIOA->IDR, &GPIOA->ODR, GPIO_IDR_IDR3, GPIO_ODR_ODR2);
        //}

        //if (button2_event){
        //    button2_event = false;
            //button_check(&GPIOA->IDR, &GPIOA->ODR, GPIO_IDR_IDR4, GPIO_ODR_ODR1);
            
        //}
    }
}

void Enable_Clocks(void){
    /* Enable TIM1, GPIOC  and GPIOA clock */
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
}

/**
 * @brief Prepares extern lines for handling a button pressing
 * @note Triggers callback on rising and falling events. PA3 connects with button1, PA4 connects with PA4
 */
void EXTI_Config(void){
    EXTI->FTSR |= EXTI_FTSR_FT5 | EXTI_FTSR_FT6;
    EXTI->RTSR |= EXTI_RTSR_RT5 | EXTI_RTSR_RT6;
    //AFIO->EXTICR[0] &= ~AFIO_EXTICR1_EXTI3;        /* AFIO_EXTICR1 needs for to choose source input for interrupt EXTI (PA3) */
    //AFIO->EXTICR[1] &= ~AFIO_EXTICR2_EXTI4;
    AFIO->EXTICR[1] &= ~(AFIO_EXTICR2_EXTI5 | AFIO_EXTICR2_EXTI6);
    EXTI->IMR |= EXTI_IMR_IM5 | EXTI_IMR_IM6;
    NVIC_SetPriority(EXTI9_5_IRQn, 1);
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    //NVIC_EnableIRQ(EXTI4_IRQn);
}

/**
 * @brief Configurates GPIOs
 * @note PC13 can be configurated as open drain (it is at the board - Blue Pill), any other demands push-pull for LEDs.
 */
void GPIO_Config(void){
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);      // Cleans bits PC13 in register CRH
    GPIOC->CRH |= (GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0);  // Output mode, max speed 10 MHz, open drain
    // PA0  -   RED LED
    GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    GPIOA->CRL |= GPIO_CRL_MODE0_1;
    // PA1  -   BLUE LED
    GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);
    GPIOA->CRL |= GPIO_CRL_MODE1_1;
    // PA2  -   YELLOW LED
    GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);
    GPIOA->CRL |= GPIO_CRL_MODE2_1;
    // PA3
    //GPIOA->CRL &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3);
    //GPIOA->CRL |= GPIO_CRL_CNF3_1;      // Input mode
    //GPIOA->ODR |= GPIO_ODR_ODR3;        // pull-up
    // PA4
    //GPIOA->CRL &= ~(GPIO_CRL_MODE4 | GPIO_CRL_CNF4);
    //GPIOA->CRL |= GPIO_CRL_CNF4_1;      // Input mode
    //GPIOA->ODR |= GPIO_ODR_ODR4;        // pull-up
    // PA5  -   BUTTON1
    GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5);
    GPIOA->CRL |= GPIO_CRL_CNF5_1;
    GPIOA->ODR |= GPIO_ODR_ODR5;
    // PA6  -   BUTTON2
    GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);
    GPIOA->CRL |= GPIO_CRL_CNF6_1;
    GPIOA->ODR |= GPIO_ODR_ODR6;

}

void SysTick_Init(void){
    /* In this configuration AHB frequency the same system frequency generates PLL (72 MHz) */
    SysTick->LOAD = BOARD_SYSCLK / MICROINSEC - 1;
    /* Clock source is AHB without prescaler */
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

    SysTick->VAL = 0;
    
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
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

