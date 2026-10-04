#ifndef TIM_H
#define TIM_H

/* For TIM1 1 kHz */
#define PSC_VALUE 1000          // Timer prescaler: 1 MHz / 1000 = 1 kHz timer clock
#define ARR_VALUE 72            // Auto-reload: if SYSCLK = 72 MHz (APB2 -> TIM1)
/* For TIM2 1 kHz */
#define ARR_VALUE_TIM2 1000     /* Auto-reload for TIM2 (It's on APB1)*/
#define PSC_VALUE_TIM2 72       /* Timer prescaler for TIM2, frequency TIMs is 72 MHz unlike ABP1  */
/* For TIM3 1 kHz */
#define ARR_VALUE_TIM3 1000
#define PSC_VALUE_TIM3 72
/* For TIM4 1 kHz */
#define ARR_VALUE_TIM4 1000    /* 36 MHz / 36 = 1 MHz, period = 1 microsecond */
#define PSC_VALUE_TIM4 72

#define MAX_ULL 0XFFFF'FFFF'FFFF'FFFFull    /* (unsigned long long)18446744073709551615ULL */

extern volatile unsigned long long tim4_ticks;

void TIM1_Init(void);
void TIM2_Init(void);
void TIM3_Init(void);
void TIM4_Init(void);

#endif