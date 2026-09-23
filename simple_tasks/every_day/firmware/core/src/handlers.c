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

static void eventPressingHandler(buttonHandler* btn, uint32_t* button_reg, uint32_t* led_reg, uint32_t button_bit, uint32_t led_bit);

/**
 * @brief Uses for correct momentary LED by button (LED connects to PA1, the button connscts to PA4)
 * @note Checks a button state few times, then solve to turn on or off the LED.
 *      See buttonHandler for getting more information
 */
void TIM1_UP_IRQHandler(void){
    TIM1->SR &= ~(TIM_SR_UIF);

    if(button1_handler.ischanged)
        eventPressingHandler(&button1_handler, (uint32_t*)&GPIOA->IDR, (uint32_t*)&GPIOA->ODR, GPIO_IDR_IDR5, GPIO_ODR_ODR0);

    if(button2_handler.ischanged)
        eventPressingHandler(&button2_handler, (uint32_t*)&GPIOA->IDR, (uint32_t*)&GPIOA->ODR, GPIO_IDR_IDR6, GPIO_ODR_ODR1);
}

/* First button (to PA3) has been pressed */
void EXTI3_IRQHandler(void){
    if(EXTI->PR & EXTI_PR_PR3){
        //button1_handler.ischanged = true;
        EXTI->PR = EXTI_PR_PR3;
    }
}

/* Second button (to PA4) has been pressed */
void EXTI4_IRQHandler(void){
    if(EXTI->PR & EXTI_PR_PR4){
        //button2_handler.ischanged = true;
        EXTI->PR = EXTI_PR_PR4;
    }
}

void EXTI9_5_IRQHandler(void){
    if(EXTI->PR & EXTI_PR_PR5){
        button1_handler.ischanged = true;
        EXTI->PR = EXTI_PR_PR5;
    }else if(EXTI->PR & EXTI_PR_PR6){
        button2_handler.ischanged = true;
        EXTI->PR = EXTI_PR_PR6;
    }
}

/**
 * @brief Handles a button state is changed and eliminates a shudder.
 *  Polls an input pin a few times trough btn->delay_time cycles of TIM1. Torches a led if the event is confirmed.
 *      
 * @param btn 
 * @param button_reg 
 * @param led_reg 
 * @param button_bit 
 * @param led_bit 
 */
static void eventPressingHandler(buttonHandler* btn, uint32_t* button_reg, uint32_t* led_reg, uint32_t button_bit, uint32_t led_bit){
    switch (btn->lock_type){
    case nothing:
        if (!READ_BIT(*button_reg, button_bit) && btn->status == OFF) {    /* Checks was it turned off before and is it pressed */
            btn->lock_type = wantON;
            btn->ticks = 0;
        }
        else if(READ_BIT(*button_reg, button_bit) && btn->status == ON) {
            btn->lock_type = wantOFF;
            btn->ticks = 0;
        }else
            btn->ischanged = false;

        //if(btn->lock_type == nothing)    /* Stop iterrupts for reducing the load with CPU */
            //NVIC_DisableIRQ(TIM1_UP_IRQn);
        break;
    case wantON:
        btn->ticks++;
        if(btn->ticks >= btn->delay_time){
            btn->ticks = 0;
            btn->efforts++;
            if (!READ_BIT(*button_reg, button_bit)){
                if(btn->efforts >= CHECKING_TIMES){  /* A few times later state is changed if upper condition has been rigth for all time */
                    SET_BIT(*led_reg, led_bit);
                    btn->status = ON;    /* It gets permition, because it has always confirmed this state */
                    
                    btn->efforts = 0;
                    btn->lock_type = nothing;    /* Handling a rattle is completed */
                }
            }else{      /* */
                btn->efforts = 0;
                btn->lock_type = nothing;
            }
        }
        break;
    case wantOFF:
        btn->ticks++;
        if(btn->ticks >= btn->delay_time){
            btn->ticks = 0;
            btn->efforts++;
            if (READ_BIT(*button_reg, button_bit)){
                if(btn->efforts >= CHECKING_TIMES){  /* A few times later state is changed if upper condition has been rigth for all time */
                    CLEAR_BIT(*led_reg, led_bit);
                    btn->status = OFF;    /* It gets permition, because it has always confirmed this state */
                    
                    btn->efforts = 0;
                    btn->lock_type = nothing;    /* Handling a rattle is completed */
                }
            }else{      /* */
                btn->efforts = 0;
                btn->lock_type = nothing;
            }
        }
        break;
    }
}
