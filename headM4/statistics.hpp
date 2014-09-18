/** @file
 * @author Orestis Zachariadis
 * @brief Includes, defines and function prototypes used for statistics functionality.
 */
#ifndef STATISTICS_HPP_
#define STATISTICS_HPP_

#include "mbed.h"
#include "conf.h"

/** @brief Calculates processor idle time
 *
 * This task is supposed to run with the lowest priority so it will be running when no other task
 * is busy. @n
 * Counts consecutive timer ticks in a period of 1 second.
 * @note Timer resolution is in us so don't expect great accuracy.
 */
void CpuLoadTask(void const *args);

#endif /* STATISTICS_HPP_ */
