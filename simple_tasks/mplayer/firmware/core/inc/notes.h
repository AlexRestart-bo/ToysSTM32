/**
 * @file notes.h
 * @brief Musical note definitions and data structures for PWM tone generator
 * 
 * This file provides note frequencies (equal-tempered scale, A4 = 440 Hz)
 * and a structure for representing musical notes with durations.
 */

#ifndef NOTES_H
#define NOTES_H

/**
 * @brief Duration of one eighth note in milliseconds
 * 
 * Used as base unit: quarter note = 2*EIGHTH, half note = 4*EIGHTH, etc.
 * At 150 ms, tempo ≈ 120 BPM (beats per minute)
 */
#define EIGHTH 150

// Note frequencies (equal-tempered scale, A4 = 440 Hz)
#define C4  261.63f   /**< Middle C (do) */
#define CS4 277.18f   /**< C sharp (do#) */
#define D4  293.66f   /**< D (re) */
#define DS4 311.13f   /**< D sharp (re#) */
#define E4  329.63f   /**< E (mi) */
#define F4  349.23f   /**< F (fa) */
#define FS4 369.99f   /**< F sharp (fa#) */
#define G4  392.00f   /**< G (sol) */
#define GS4 415.30f   /**< G sharp (sol#) */
#define A4  440.00f   /**< A (la) - concert pitch */
#define AS4 466.16f   /**< A sharp (la#) */
#define B4  493.88f   /**< B (si) */

/**
 * @brief Structure representing a single note sample
 * 
 * Used in melody arrays to store both frequency and duration
 */
typedef struct {
    float note;          /**< Frequency in Hz (use C4, D4, etc. from above) */
    unsigned int duration; /**< Duration in milliseconds (use EIGHTH, 2*EIGHTH, etc.) */
} noteSample;

#endif // NOTES_H