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

#define AVG_SLOPE (4.3 * 0.001)       /* Average slope in V */
#define PRIMARY_TEMP 25     /* Start temperature for beginning measurements */
#define PRIMARY_VOLT 1.43   /* Voltage at 25 °C */

#endif