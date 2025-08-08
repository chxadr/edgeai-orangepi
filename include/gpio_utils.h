#ifndef GPIO_UTILS_H
#define GPIO_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <gpiod.h>

typedef struct gpiod_chip gpiod_chip;
typedef struct gpiod_line gpiod_line;

typedef enum {
    LOW,
    HIGH
} gpiod_value_t;

int gpio_write(gpiod_line *line, gpiod_value_t value);

#ifdef __cplusplus
}
#endif

#endif // GPIO_UTILS_H