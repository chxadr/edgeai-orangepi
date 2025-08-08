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

    char chip_path[32];
    snprintf(chip_path, sizeof(chip_path), "/dev/%s", GPIO_CHIP);
    gpio_chip = gpiod_chip_open(chip_path);
    if (!gpio_chip) {
        perror("[Error] Failed to open GPIO chip");
        return EXIT_FAILURE;
    }

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

    // Request output lines
    gpiod_line_request_output(dir_x_line, "dir_x", LOW);
    gpiod_line_request_output(dir_y_line, "dir_y", LOW);

    // Request input lines
    gpiod_line_request_input(lim_x_line, "lim_x");
    gpiod_line_request_input(lim_y_line, "lim_y");
    gpiod_line_request_input(m_sw_line, "m_sw");

    printf("[Info] GPIO setup complete\n");

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
    int step_x = syspwm_check(&PWM_STEP_X);
    int step_y = syspwm_check(&PWM_STEP_Y);
    if (step_x == EXIT_FAILURE || step_y == EXIT_FAILURE) {
        perror("[Error] You must export PWM channels has root");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int gpio_close(void)
{
    gpiod_chip_close(gpio_chip);
    return EXIT_SUCCESS;
}
