/**
 * @file gpio_utils.c
 * @author Adrien Chevrier
 * 
 * @brief Implementation file for the header @c gpio_utils.h .
 *
 * @see gpio_utils.h
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

#include "gpio_utils.h"

int gpio_write(gpiod_line *line, gpiod_value_t value)
{
    // Use the function provided by libgpiod.
    if (gpiod_line_set_value(line, value) < 0) {
        perror("[Error] gpiod_line_set_value failed");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
