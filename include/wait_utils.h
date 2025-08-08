/**
 * @file wait_utils.h
 * @author Adrien Chevrier
 * 
 * @brief Header file for creating interruptible waiting durations.
 *
 * This file provides two functions that enable a program to wait
 * for a specific duration, subdivised in smaller chucks, to be able
 * to react to a specific event triggered by a function passed as
 * parameter.
 *
 * @see wait_utils.c
 * 
 * @version 0.1
 * @date 2025-07-15
 *
 * @todo Rewrite this module to use GNU/Linux Kernel timer-based interrupts.
 *
 * @copyright Copyright (c) 2025 Adrien Chevrier
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef WAIT_UTILS_H
#define WAIT_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

// Type definitions for waiting durations
typedef uint32_t time_us_t; ///< Type for duration [us].
typedef uint16_t time_ms_t; ///< Type for duration [ms].

/**
 * Waits for the specified duration in microseconds, but in small chunks,
 * allowing checks for shutdown conditions.
 *
 * @param total_duration Total duration to wait [us].
 * @param check_interval_duration Interval to check the condition [us].
 * @param exit_requested Pointer to a function that returns @c true
 *                       if the wait should abort early, @c false
 *                       otherwise.
 *
 * @return @c true if exited early, @c false if completed entire wait.
 *
 * @todo Replace @c usleep() usage (deprecated) with @c nanosleep() usage.
 */
bool wait_interruptible_us(time_us_t total_duration, time_us_t check_interval_duration, bool (*exit_requested)(void));

/**
 * Waits for the specified duration in milliseconds, but in small chunks,
 * allowing checks for shutdown conditions.
 *
 * @param total_duration Total duration to wait [ms].
 * @param check_interval_duration Interval to check the condition [ms].
 * @param exit_requested Pointer to a function that returns @c true
 *                       if the wait should abort early, @c false
 *                       otherwise.
 *
 * @return @c true if exited early, @c false if completed entire wait.
  *
 * @todo Replace @c usleep() usage (deprecated) with @c nanosleep() usage.
 */
bool wait_interruptible_ms(time_ms_t total_duration, time_ms_t check_interval_duration, bool (*exit_requested)(void));

#ifdef __cplusplus
}
#endif

#endif // WAIT_UTILS_H
