/**
 * @file clock_utils.h
 * @author Alexey Filippov
 * @brief Utilites for to manage delays and system clocks 
 * @version 1.0
 * @date 2026-09-12
 * 
 */

#ifndef CLOCK_UTILS_H
#define CLOCK_UTILS_H

/**
 * @brief Delays the program by microseconds
 * 
 * @param mcs any integer positive number (must be more than 0)
 * @return int 
 * @note It is not related to interruptions and does not affect their operation
 */
int waiting_microseconds(unsigned int mcs);

#endif