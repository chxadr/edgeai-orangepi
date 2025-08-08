/**
 * @file stepper_demo.h
 * @author Adrien Chevrier
 * 
 * @brief Header file for communication with stepper motors.
 *
 * This file contains function prototypes for sending coordinates
 * to the stepper motors. These functions do not actually perform
 * the motor movement themselves.
 *
 * @see stepper_demo.c
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

#ifndef STEPPER_DEMO_H
#define STEPPER_DEMO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>

#include "ctrl_motors.h"
#include "ipc_elements.h"
#include "wait_utils.h"

/**
 * @brief Writes the absolute target position on the image in X and Y coordinates.
 *
 * This function sends the target absolute position (X,Y) on the image
 * via two buffers, one for each coordinate, then waits until
 * the two motors read those buffers.
 *
 * @param[in] x The target X absolute position [px].
 * @param[in] y The target Y absolute position [px].
 */
void write_abs_pos(d_px_t x, d_px_t y);

/**
 * @brief Sends positions to move the stepper motors in a circular pattern.
 *
 * This function sends the coordinates of points that form a circle to the motors.
 * Uses @c write_abs_pos() to send the position of each point approximating
 * the circle perimeter.
 *
 * @note Use for demonstration and testing purposes.
 *
 * @param[in] r The radius of the circle to draw [px].
 * @param[in] n_pts The number of points used to approximate the circle.
 * @param[in] delay The delay between each movement [us].
 */
void circle(d_px_t r, uint8_t n_pts, time_us_t delay);

#ifdef __cplusplus
}
#endif

#endif // STEPPER_DEMO_H