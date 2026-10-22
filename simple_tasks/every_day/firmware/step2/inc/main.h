/**
 * @file main.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2026-10-06
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f1xx.h"
#include "stm32f103xb.h"
#include "system_stm32f1xx.h"

#define AVG_SLOPE (4.3f * 0.001f)   /* Average slope in V */
#define PRIMARY_TEMP 25.0f          /* Start temperature for beginning measurements */
#define PRIMARY_VOLT 1.43f          /* Voltage at 25 °C */

#define SENSORS 4
#define INPUT_TEMPSENSOR 3

typedef struct {
    volatile uint16_t sensor_readings[2*SENSORS] __aligned(4);     /* Data from ADCs, it has two parts for DMA */
    volatile uint8_t runner;            /* Number of a current sensor */
    volatile uint8_t refresh_sensor;    /* An order of a sensor that value was refreshed */ 
    volatile float sensor_values[SENSORS];  /* Real physic values */
    volatile uint8_t new_portion;       /* New data has been handled */
} sensorData;

#define FREADY 1    /* First half of bufer is ready */
#define SREADY 2    /* Second half of bufer is ready */

#endif