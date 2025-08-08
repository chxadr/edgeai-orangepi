/**
 * @file setup.h
 * @author Adrien Chevrier
 * 
 * @brief Header file for GPIO and PWM setup configurations.
 *
 * This file contains the definitions, data structures, and function prototypes 
 * necessary to set up and control GPIO pins and PWM channels for motor control 
 * and sensor handling. It provides configuration for GPIO chips, motors, limit 
 * switches, and emergency switches.
 *
 * @note Sensors are note used for now.
 * @todo Use limit sensors and master emergency switch.
 *
 * @see setup.c
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

#ifndef SETUP_H
#define SETUP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#include "gpio_utils.h"
#include "syspwm.h"


/*******************************************************************************
 * GPIO chip and pins number definitions
 ******************************************************************************/

#define GPIO_CHIP "gpiochip1"   ///< The GPIO chip device name used for controlling GPIO pins.

#define GPIO_DIR_X 75   ///< GPIO pin number for controlling the X-axis motor direction.          
#define GPIO_DIR_Y 226  ///< GPIO pin number for controlling the Y-axis motor direction.

#define GPIO_LIM_X 233  ///< GPIO pin number for the X-axis limit switch sensor.
#define GPIO_LIM_Y 74   ///< GPIO pin number for the Y-axis limit switch sensor.

#define GPIO_M_SW 78    ///< GPIO pin number for the master emergency switch.

/*******************************************************************************
 * GPIO chip and lines data structures
 ******************************************************************************/

/**
 * @brief
 * Pointer to the GPIO chip structure representing
 * the chip used for GPIO operations.
 */
extern gpiod_chip* gpio_chip;

extern gpiod_line* dir_x_line;  ///< GPIO line for controlling the X-axis motor direction.
extern gpiod_line* dir_y_line;  ///< GPIO line for controlling the Y-axis motor direction.
extern gpiod_line* lim_x_line;  ///< GPIO line for the X-axis limit switch sensor.
extern gpiod_line* lim_y_line;  ///< GPIO line for the Y-axis limit switch sensor.
extern gpiod_line* m_sw_line;   ///< GPIO line for the master emergency switch.

/*******************************************************************************
 * PWM channels
 ******************************************************************************/

#define PWM_STEP_X SYSPWM_4 ///< PWM channel used for controlling the X-axis stepper motor.
#define PWM_STEP_Y SYSPWM_3 ///< PWM channel used for controlling the Y-axis stepper motor.

/*******************************************************************************
 * Constants for output Signals and Ratios
 ******************************************************************************/

#define PWM_FREQ 350     ///< PWM frequency [Hz].
#define STEP_SIZE 2      ///< Step size on a captured frame (1 step ~ 2 pixels).

/*******************************************************************************
 * GPIO and PWM chip and lines data structures init/close functions
 ******************************************************************************/

/**
 * @brief Initializes GPIO pins and configures the GPIO chip and lines.
 *
 * This function sets up the necessary GPIO lines for motor control,
 * limit switches, and the emergency switch.
 *
 * @return @c EXIT_SUCCESS on success, @c EXIT_FAILURE if the GPIO chip
 *         is not accessible.
 */
int gpio_setup(void);

/**
 * @brief Tests the PWM channels for motor control.
 *
 * This function checks if the PWM channels to use are all ready.
 *
 * @return @c EXIT_SUCCESS on success, @c EXIT_FAILURE if the PWM channels
 *         are not exported.
 */
int pwm_setup(void);

/**
 * @brief Closes and cleans up the GPIO and PWM resources.
 *
 * This function shuts down the GPIO chip, releasing the resources.
 */
void gpio_close(void);

#ifdef __cplusplus
}
#endif

#endif // SETUP_H
