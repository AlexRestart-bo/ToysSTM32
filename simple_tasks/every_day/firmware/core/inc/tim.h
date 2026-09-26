#ifndef TIM_H
#define TIM_H

#define PSC_VALUE 1000          // Timer prescaler: 1 MHz / 1000 = 1 kHz timer clock
#define ARR_VALUE 72            // Auto-reload: if SYSCLK = 72 MHz (APB2 -> TIM1)
#define ARR_VALUE_APB1 3600       /* Auto-reload for TIM2 (It's on APB1)*/
#define PSC_VALUE_TIM2 10000     /* Timer prescaler for TIM2 */

void TIM1_Init(void);
void TIM2_Init(void);

#endif