/**
 * @file gpio_utils.h
 * @author Adrien Chevrier
 * 
 * @brief Header file for GPIO utility functions.
 *
 * This file contains function prototypes and data types related to GPIO (General Purpose 
 * Input/Output) operations, including reading and writing values to GPIO pins using the 
 * @c libgpiod library.
 *
 * @note For now there is only a function to write on GPIO pins.
 *
 * @see gpio_utils.c
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

#ifndef GPIO_UTILS_H
#define GPIO_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <gpiod.h>

// Type definition for convenience when using libgpiod.
typedef struct gpiod_chip gpiod_chip;
typedef struct gpiod_line gpiod_line;

/**
 * @brief 
 * The two possible states of a GPIO pin. Could be written
 * to a GPIO pin or read.
 */
typedef enum {
    LOW,    ///< State corresponding to low voltage.
    HIGH    ///< State corresponding to high voltage.
} gpiod_value_t;

/**
 * @brief Write a value to a GPIO line.
 *
 * This function sets the state of a specified GPIO line to either @c LOW or @c HIGH.
 *
 * @param line Pointer to a @c gpiod_line structure representing the GPIO line to write to.
 * @param value The value to write to the GPIO line. It can be @c LOW` or @c HIGH.
 * 
 * @return @c EXIT_SUCCESS on success, @c EXIT_FAILURE otherwise.
 */
int gpio_write(gpiod_line *line, gpiod_value_t value);

#ifdef __cplusplus
}
#endif

#endif // GPIO_UTILS_H