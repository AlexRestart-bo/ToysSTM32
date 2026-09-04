/**
 * @file melodies.c
 * @brief Musical note sequences for PWM buzzer
 * 
 * Note: A short gap (20 ms) between notes prevents clicking sounds
 * and makes melody more natural.
 */

#include "main.h"

/**
 * @brief Plays simplest melody by UGCM1205X
 * 
 */
void play_simplest_melody(void){
    static unsigned char step = 0;
    switch (step++) {
        case 0:
            set_note(A4);
            break;
        case 1:
            set_note(C4);
            break;
        case 2:
            set_note(B4);
            break;
        case 3:
            set_note(C4);
            step = 0;
            break;
    }
}

/**
 * @brief Plays "Marmotte" was composed by Ludwig van Beethoven
 * 
 */
void play_marmot(void){
    // The part of the composition. It consists of 32 notes, every note has got a frequency and a duration
    static const noteSample marmot_setting[32] = {  {G4, EIGHTH}, {G4, EIGHTH}, {A4, EIGHTH}, {B4, EIGHTH}, 
                                                    {C4, 3*EIGHTH}, {B4, EIGHTH}, {A4, EIGHTH}, {G4, EIGHTH}, 
                                                    {G4, EIGHTH}, {G4, EIGHTH}, {A4, EIGHTH}, {B4, EIGHTH}, 
                                                    {C4, 3*EIGHTH}, {B4, EIGHTH}, {A4, EIGHTH}, {G4, EIGHTH},

                                                    {A4, EIGHTH}, {B4, EIGHTH}, {C4, EIGHTH}, {D4, EIGHTH}, 
                                                    {C4, 3*EIGHTH}, {B4, EIGHTH}, {A4, EIGHTH}, {G4, EIGHTH}, 
                                                    {FS4, EIGHTH}, {G4, EIGHTH}, {A4, EIGHTH}, {B4, EIGHTH}, 
                                                    {C4, 3*EIGHTH}, {B4, EIGHTH}, {A4, EIGHTH}, {G4, EIGHTH}, };

    static unsigned char step = 0;  // The switch for notes
    set_note(marmot_setting[step].note);
    stop_for_ms(marmot_setting[step].duration);

    // The intermission needs for more natural sound
    TIM2->CR1 &= ~TIM_CR1_CEN;
    stop_for_ms(20);
    TIM2->CR1 |= TIM_CR1_CEN;

    step = (step + 1) % 32; // Goes to next note
}