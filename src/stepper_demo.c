/**
 * @file stepper_demo.c
 * @author Adrien Chevrier
 * 
 * @brief Implementation file for the header @c stepper_demo.h .
 *
 * @see stepper_demo.h
 * 
 * @version 0.1
 * @date 2025-07-15
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

#include "stepper_demo.h"

void write_abs_pos(d_px_t x, d_px_t y)
{
    // Send X and Y coordinates to the buffers.
    x_px_buff = x;
    sem_post(&data_x_ready_sem);
    y_px_buff = y;
    sem_post(&data_y_ready_sem);
    printf("[Info] sent x=%d, y=%d\n", x_px_buff, y_px_buff);

    // Wait for the motors to read the coordinates.
    sem_wait(&data_x_done_sem);
    sem_wait(&data_y_done_sem);
}

void circle(d_px_t r, uint8_t n_pts, time_us_t delay)
{
    // Angular step.
    double da = 2 * M_PI / n_pts;

    for (uint8_t i = 0; i < n_pts; i++) {
        // Stop on termination signal.
        if (psig_kill_requested()) break;

        // Current angle.
        double a = da * i;
        // Compute and write coordinates to the buffers.
        d_px_t x = (d_px_t)(r * cos(a));
        d_px_t y = (d_px_t)(r * sin(a));
        printf("[Info] Point %d: ", i);
        write_abs_pos(x, y);

        wait_interruptible_us(delay, 1000, psig_kill_requested);
    }
}
