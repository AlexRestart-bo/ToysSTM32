/**
 * @file handlers.c
 * @author Alexey Filippov
 * @brief Callbacks for peripheral events
 * @version 1.0
 * @date 2026-09-14
 * 
 */

#include "main.h"

unsigned long long ticks_tim1 = 0;

/**
 * @brief Uses for correct momentary LED by button (LED connects to PA1, the button connscts to PA4)
 * @note Checks a button state few times, then solve to turn on or off the LED.
 *      See buttonHandler for getting more information
 */
void TIM1_UP_IRQHandler(void){
    TIM1->SR &= ~(TIM_SR_UIF);
    /**/
    switch (button2_handler.lock_type){
    case nothing:
        if (!READ_BIT(GPIOA->IDR, GPIO_IDR_IDR4) && button2_handler.status == OFF) {    /* Checks was it turned off before and is it pressed */
            button2_handler.lock_type = wantON;
            button2_handler.ticks = 0;
        }
        else if(READ_BIT(GPIOA->IDR, GPIO_IDR_IDR4) && button2_handler.status == ON) {
            button2_handler.lock_type = wantOFF;
            button2_handler.ticks = 0;
        }

        //if(button2_handler.lock_type == nothing)    /* Stop iterrupts for reducing the load with CPU */
            //NVIC_DisableIRQ(TIM1_UP_IRQn);
        break;
    case wantON:
        button2_handler.ticks++;
        if(button2_handler.ticks >= button2_handler.delay_time){
            button2_handler.ticks = 0;
            button2_handler.efforts++;
            if (!READ_BIT(GPIOA->IDR, GPIO_IDR_IDR4)){
                if(button2_handler.efforts >= CHECKING_TIMES){  /* A few times later state is changed if upper condition has been rigth for all time */
                    SET_BIT(GPIOA->ODR, GPIO_ODR_ODR1);
                    button2_handler.status = ON;    /* It gets permition, because it has always confirmed this state */
                    
                    button2_handler.efforts = 0;
                    button2_handler.lock_type = nothing;    /* Handling a rattle is completed */
                }
            }else{      /* */
                button2_handler.efforts = 0;
                button2_handler.lock_type = nothing;
            }
        }
        break;
    case wantOFF:
        button2_handler.ticks++;
        if(button2_handler.ticks >= button2_handler.delay_time){
            button2_handler.ticks = 0;
            button2_handler.efforts++;
            if (READ_BIT(GPIOA->IDR, GPIO_IDR_IDR4)){
                if(button2_handler.efforts >= CHECKING_TIMES){  /* A few times later state is changed if upper condition has been rigth for all time */
                    CLEAR_BIT(GPIOA->ODR, GPIO_ODR_ODR1);
                    button2_handler.status = OFF;    /* It gets permition, because it has always confirmed this state */
                    
                    button2_handler.efforts = 0;
                    button2_handler.lock_type = nothing;    /* Handling a rattle is completed */
                }
            }else{      /* */
                button2_handler.efforts = 0;
                button2_handler.lock_type = nothing;
            }
        }
        break;
    }
}

/* First button (to PA3) has been pressed */
void EXTI3_IRQHandler(void){
    if(EXTI->PR & EXTI_PR_PR3){
        button1_event = true;
        EXTI->PR = EXTI_PR_PR3;
    }
}

/* Second button (to PA4) has been pressed */
void EXTI4_IRQHandler(void){
    if(EXTI->PR & EXTI_PR_PR4){
        button2_event = true;
        EXTI->PR = EXTI_PR_PR4;
    }
}
