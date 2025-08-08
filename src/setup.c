/**
 * @file setup.c
 * @author Adrien Chevrier
 * 
 * @brief Implementation file for the header @c setup.h .
 *
 * @see setup.h
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

#include "setup.h"

/*******************************************************************************
 * GPIO chip and lines data structures initialization
 ******************************************************************************/

gpiod_chip* gpio_chip = NULL;

gpiod_line* dir_x_line = NULL;
gpiod_line* dir_y_line = NULL;
gpiod_line* step_x_line = NULL;
gpiod_line* step_y_line = NULL;
gpiod_line* lim_x_line = NULL;
gpiod_line* lim_y_line = NULL;
gpiod_line* m_sw_line  = NULL;

/*******************************************************************************
 * GPIO and PWM chip and lines data structures init/close functions
 ******************************************************************************/

int gpio_setup(void)
{
    printf("[Info] Start GPIO setup ...\n");

    // Open GPIO chip.
    char chip_path[32];
    snprintf(chip_path, sizeof(chip_path), "/dev/%s", GPIO_CHIP);
    gpio_chip = gpiod_chip_open(chip_path);
    if (!gpio_chip) {
        perror("[Error] Failed to open GPIO chip");
        return EXIT_FAILURE;
    }

    // Retrive GPIO lines to use.
    dir_x_line  = gpiod_chip_get_line(gpio_chip, GPIO_DIR_X);
    dir_y_line  = gpiod_chip_get_line(gpio_chip, GPIO_DIR_Y);
    lim_x_line  = gpiod_chip_get_line(gpio_chip, GPIO_LIM_X);
    lim_y_line  = gpiod_chip_get_line(gpio_chip, GPIO_LIM_Y);
    m_sw_line   = gpiod_chip_get_line(gpio_chip, GPIO_M_SW);

    if (!dir_x_line || !dir_y_line ||
        !lim_x_line || !lim_y_line || !m_sw_line) {
        perror("[Error] Failed to get GPIO lines\n");
        return EXIT_FAILURE;
    }

    // Request output lines.
    gpiod_line_request_output(dir_x_line, "dir_x", LOW);
    gpiod_line_request_output(dir_y_line, "dir_y", LOW);

    // Request input lines.
    gpiod_line_request_input(lim_x_line, "lim_x");
    gpiod_line_request_input(lim_y_line, "lim_y");
    gpiod_line_request_input(m_sw_line, "m_sw");

    printf("[Info] GPIO setup complete\n");

    // Display the set GPIO lines and their configurations.
    printf("[Info] Check GPIO setup ...\n");
    system(
        "/usr/bin/gpioinfo | "
        "/usr/bin/grep 'dir_x\\|dir_y\\|"
        "lim_x\\|lim_y\\|m_sw'"
    );

    return EXIT_SUCCESS;
}

int pwm_setup(void)
{
    // Check if PWM pins are exported.
    int step_x = syspwm_check(&PWM_STEP_X);
    int step_y = syspwm_check(&PWM_STEP_Y);
    if (step_x == EXIT_FAILURE || step_y == EXIT_FAILURE) {
        perror("[Error] You must export PWM channels has root");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void gpio_close(void)
{
    gpiod_chip_close(gpio_chip);
}
