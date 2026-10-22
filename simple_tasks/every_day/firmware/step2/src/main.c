/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2026-10-07
 * 
 * @copyright Copyright (c) 2026
 * 
 * Obtain the temperature using the following formula:
    Temperature (in °C) = {(V25 - VSENSE) / Avg_Slope} + 25.
    Where,
    V25 = VSENSE value for 25° C and
    Avg_Slope = Average Slope for curve between Temperature vs. VSENSE (given in
    mV/° C or μV/ °C).
    Refer to the Electrical characteristics section for the actual values of V25 and
    Avg_Slope.
 */
#include "main.h"

void RCC_config(void){
    /* 2 wait states if 48 MHz < SYSCLK <= 72 MHz */
    //FLASH->ACR = FLASH_ACR_LATENCY_2;

    /* Turns HSE on */
    //RCC->CR |= RCC_CR_HSEON;

    /* Waits HSE is ready */
    //while (!(RCC->CR & RCC_CR_HSERDY));
    
    /* System clock - PLL, multiplication factor for PLL - 7 (HSE = 8MHz -> 8 * 7 = 56 MHz) */
    //RCC->CFGR |= RCC_CFGR_PLLSRC;
    //RCC->CFGR |= RCC_CFGR_PLLMULL7;

    /* Waits PLL is stable */
    //while (!(RCC->CR & RCC_CR_PLLRDY));
    
    /* Configure prescalers for the buses */
    //RCC->CFGR |= RCC_CFGR_HPRE_DIV2;      // AHB = SYSCLK / 2 = 28 MHz
    //RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;     // APB1 = AHB / 1 = 28 MHz (max = 36 MHz)
    //RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;     // APB2 = AHB / 1 = 28 MHz

    /* Switch system clock on PLL */
    //RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait for the end of switching */
    //while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    // Включаем HSI на случай проблем с внешним кварцем
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY));

    FLASH->ACR = FLASH_ACR_LATENCY_2;

    /* Попробуйте запустить HSE и дождаться флага */
    RCC->CR |= RCC_CR_HSEON;
    
    // Если виснет здесь — значит кварц на плате не отвечает или поврежден
    while (!(RCC->CR & RCC_CR_HSERDY)); 
    
    // ... остальной код настройки PLL
}

void Enable_Clocks(void){
    /* Enable ADC1 clock */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPAEN;
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
}

void ADC1_Init(void){

    /* Four conversions are chose */
    ADC1->SQR1 |= ADC_SQR1_L_0 | ADC_SQR1_L_1;  // (0x3 << 20)
    ADC1->SQR3 |= (0 << ADC_SQR3_SQ1_Pos) | (1 << ADC_SQR3_SQ2_Pos) | (2 << ADC_SQR3_SQ3_Pos) | 
                    (16 << ADC_SQR3_SQ4_Pos);

    /* The recommended sampling time for the temperature sensor is 17.1 mcs */
    ADC1->SMPR1 |= ADC_SMPR1_SMP16;     /* 239.5 + 12.5 = 252 cycles (14 MHz -> 18 mcs), it needs in recalculation */

    /* Enable scan mode */
    ADC1->CR1 |= ADC_CR1_SCAN;
    
    /* Right alignment */
    ADC1->CR2 &= ~ADC_CR2_ALIGN;

    /* Enable DMA mode */
    ADC1->CR2 |= ADC_CR2_DMA;

    /* Enable external trigger (SWSTART) */
    ADC1->CR2 |= ADC_CR2_EXTTRIG | ADC_CR2_EXTSEL;

    /* Enable interrupt (the end of conversion) */
    //ADC1->CR1 |= ADC_CR1_EOCIE;

    /* Turn temperature sensor on and turn his power on */
    ADC1->CR2 |= ADC_CR2_TSVREFE | ADC_CR2_ADON;

    /* Need at least two free cycles before calibration */
    ADC1->CR2 |= ADC_CR2_CAL;

    /* Wait for the end of the calibration */
    while (READ_BIT(ADC1->CR2, ADC_CR2_CAL));

    /* Continious conversion mode */
    ADC1->CR2 |= ADC_CR2_CONT;

    //NVIC_EnableIRQ(ADC1_2_IRQn);

}

void DMA1_Init(uint16_t* data){
    /* Set priority ?? PSIZE AND MSIZE FOR SIZE ?? PINC AND MINK FOR INCREMENT */
    DMA1_Channel1->CCR |= DMA_CCR_PL_0 | DMA_CCR_PL_1 |     // very high priority
                            DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0 |     // ADC->DR is 16-bit register
                            DMA_CCR_MINC | DMA_CCR_CIRC |
                            DMA_CCR_HTIE | DMA_CCR_TCIE;    // Half transfer and complete
    
    /* Number of data to transfer */
    DMA1_Channel1->CNDTR = 2 * SENSORS;
    
    /* Define peripheral and memory addresses */
    DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;
    DMA1_Channel1->CMAR = (uint32_t)data;

    SET_BIT(DMA1_Channel1->CCR, DMA_CCR_EN);
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

void GPIO_Init(void){
    /* See datasheet */
    GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);    /* PA0 as analog input -> ADCx_IN0 */
    GPIOA->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1);    /* PA1 as analog input -> ADCx_IN1 */
    GPIOA->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2);    /* PA2 as analog input -> ADCx_IN2 */
}

float get_tempsensor_value(void){

    uint16_t data_sense = ADC1->DR & 0xFFF;    /* Get lower 12 bits (max_value = 0xFFF), reading Data Register clears EOC bit in Status Register */

    float v_sense = 3.3f * (float)data_sense / (float)0xFFF;

    return (float)(((PRIMARY_VOLT - v_sense) / AVG_SLOPE) + PRIMARY_TEMP);
}

void translate_values(sensorData* inflow){

    int k = inflow->new_portion - 1;   /* An order a data portion that's ready: first half of the buffer or second half */ 

    for (int i = 0; i < SENSORS; i++){

        float v_sense = 3.3f * (float)inflow->sensor_readings[(k * SENSORS) + i] / (float)0xFFF;

        if (i == INPUT_TEMPSENSOR){
            inflow->sensor_values[i] = (float)(((PRIMARY_VOLT - v_sense) / AVG_SLOPE) + PRIMARY_TEMP);     /* The formula is described in rm */
        }else 
            inflow->sensor_values[i] = v_sense;
    }

    inflow->new_portion = 0;
}

sensorData input_flow = {0};

int main(void){
    RCC_config();
    Enable_Clocks();
    GPIO_Init();
    DMA1_Init(input_flow.sensor_readings);
    ADC1_Init();

    /* Start convertions */
    ADC1->CR2 |= ADC_CR2_SWSTART;

    while(1){
        if(input_flow.new_portion == FREADY || input_flow.new_portion == SREADY)
            translate_values(&input_flow);
    };

    return 0;
}

/*
void ADC1_2_IRQHandler(void){
    if (READ_BIT(ADC1->SR, ADC_SR_EOC)){
        input_flow.sensor_readings[input_flow.runner] = (uint16_t)(ADC1->DR & 0xFFF);
        input_flow.refresh_sensor = input_flow.runner;
        input_flow.runner = (input_flow.runner + 1) % SENSORS;
        input_flow.new_portion = 1;
    }
}
*/

void DMA1_Channel1_IRQHandler(void){
    if(READ_BIT(DMA1->ISR, DMA_ISR_HTIF1)){
        SET_BIT(DMA1->IFCR, DMA_IFCR_CHTIF1);
        input_flow.new_portion = FREADY;
    }
    
    if(READ_BIT(DMA1->ISR, DMA_ISR_TCIF1)){
        SET_BIT(DMA1->IFCR, DMA_IFCR_CTCIF1);
        input_flow.new_portion = SREADY;
    }
    
    if(READ_BIT(DMA1->ISR, DMA_ISR_GIF1)){
        SET_BIT(DMA1->IFCR, DMA_IFCR_CGIF1);
    }
}
