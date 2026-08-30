#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include "stm32f1xx.h"
#include "stm32f103xb.h"
#include "system_stm32f1xx.h"

#include "notes.h"

void set_note(float frequency);
void stop_for_ms(unsigned int ms);

void play_simplest_melody(void);
void play_marmot(void);

#endif