/**
 * @file wait_utils.c
 * @author Adrien Chevrier
 * 
 * @brief Implementation file for the header @c wait_utils.h .
 *
 * @see wait_utils.h
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

#include "wait_utils.h"

bool wait_interruptible_us(time_us_t total_duration, time_us_t check_interval_duration, bool (*exit_requested)(void))
{
    time_us_t waited_duration = 0;
    while (waited_duration < total_duration) {
        // Exit early on event.
        if (exit_requested && exit_requested()) {
            return true;
        }

        // Determine the next chunk of sleep time.
        time_us_t sleep_duration = check_interval_duration;

        // If the remaining time to wait is less than the check interval, adjust sleep time.
        if (total_duration - waited_duration < check_interval_duration) {
            sleep_duration = (time_us_t)(total_duration - waited_duration);
        }

        // Sleep for a duration chunk.
        usleep(sleep_duration);
        waited_duration += sleep_duration;
    }
    // Full wait completed.
    return false;
}

bool wait_interruptible_ms(time_ms_t total_duration, time_ms_t check_interval_duration, bool (*exit_requested)(void))
{
    return wait_interruptible_us(total_duration * 1000, check_interval_duration * 1000, exit_requested);
}
