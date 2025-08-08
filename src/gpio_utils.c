#include "gpio_utils.h"

int gpio_write(gpiod_line *line, gpiod_value_t value)
{
    if (gpiod_line_set_value(line, value) < 0) {
        perror("[Error] gpiod_line_set_value failed");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}