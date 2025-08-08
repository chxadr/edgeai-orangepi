/**
 * @file ctrl_motors.h
 * @author Adrien Chevrier
 * 
 * @brief Header file for controlling stepper motors.
 *
 * This file contains the function prototypes, data types, and definitions related 
 * to controlling stepper motors. It provides functions to control stepper motors 
 * using PWM signals and GPIO lines for direction and movement, as well as
 * motors tasks declaration for multithreading.
 *
 * @see ctrl_motors.c
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

#ifndef CTRL_MOTORS_H
#define CTRL_MOTORS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "psig_utils.h"
#include "gpio_utils.h"
#include "syspwm.h"
#include "ipc_elements.h"
#include "setup.h"


/// Type definition for a displacement/position on a captured frame [px] (signed).
typedef int16_t d_px_t;


/**
 * @brief Enumeration to represent step direction.
 *
 * This enum defines the possible step directions for controlling a stepper motor.
 */
typedef enum {
    STEP_P, ///< Positive step direction.
    STEP_M  ///< Negative step direction.
} step_dir_t;


extern d_px_t x_px_buff;    ///< Buffer where the X-axis position is read by the X-motor [px].
extern d_px_t y_px_buff;    ///< Buffer where the Y-axis position is read by the Y-motor [px].


/**
 * @brief Moves a stepper motor with direction and steps.
 *
 * This function moves a stepper motor by a specified number of steps, 
 * in the specified direction (positive or negative) and frequency.
 *
 * @param[in] pwm Pointer to the PWM channel used for motor step control.
 * @param[in,out] gpio_line Pointer to the GPIO line controlling motor direction.
 * @param[in] freq Frequency at which the motor should operate [Hz].
 * @param[in] steps Number of steps to move the motor.
 * @param[in] dir Direction to move the motor, either @c STEP_P or @c STEP_M .
 *
 * @return @c EXIT_SUCCESS on success, @c EXIT_FAILURE otherwise.
 */
int move_stepper_raw(const syspwm_t* pwm, gpiod_line *gpio_line, frequency_hz_t freq, step_t steps, step_dir_t dir);

/**
 * @brief Moves a stepper motor by a specified displacement in pixels.
 *
 * This function moves the stepper motor by a specified number of pixels,
 * in a direction (positive or negative) determined by the displacement sign,
 * and the specified frequency.
 *
 * @param[in] pwm Pointer to the PWM channel used for motor step control.
 * @param[in,out] gpio_line Pointer to the GPIO line controlling motor direction.
 * @param[in] freq Frequency at which the motor should operate [Hz].
 * @param[in] d Displacement to move the stepper motor [px] (signed).
 *
 * @return @c EXIT_SUCCESS on success, @c EXIT_FAILURE otherwise.
 */
int move_stepper(const syspwm_t* pwm, gpiod_line *gpio_line, frequency_hz_t freq, d_px_t d);

/**
 * @brief Task to control the X-axis stepper motor.
 * 
 * This task waits for the motor to receive its calibration X-position,
 * then reads X-position from the corresponding buffer.
 * Each time a new position is recorded, the displacement is computed
 * with the previous position, and the motor moves according
 * to this displacement. The previous position is finally updated
 * with the last received one.
 * 
 * @param[in] arg A pointer to any necessary arguments for the camera task.
 * @return A pointer to a result of the task execution.
 */
void* stepper_x_task(void* arg);

/**
 * @brief Task to control the Y-axis stepper motor.
 * 
 * This task waits for the motor to receive its calibration Y-position,
 * then reads Y-position from the corresponding buffer.
 * Each time a new position is recorded, the displacement is computed
 * with the previous position, and the motor moves according
 * to this displacement. The previous position is finally updated
 * with the last received one.
 * 
 * @param[in] arg A pointer to any necessary arguments for the camera task.
 * @return A pointer to a result of the task execution.
 */
void* stepper_y_task(void* arg);

#ifdef __cplusplus
}
#endif

#endif // CTRL_MOTORS_H
