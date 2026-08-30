#ifndef NOTES_H
#define NOTES_H

#define EIGHTH 150

#define C4 261.63
#define CS4 277.18
#define D4 293.66
#define DS4 311.13
#define E4 329.63
#define F4 349.23
#define FS4 369.99
#define G4 392.0
#define GS4 415.30
#define A4 440.0
#define AS4 466.16
#define B4 493.88

typedef struct {
    float note;
    unsigned int duration;
} noteSample;

#endif