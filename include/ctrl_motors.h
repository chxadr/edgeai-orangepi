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

typedef int16_t d_px_t;

typedef enum {
    STEP_P, // Positive step direction
    STEP_M // Negative step direction
} step_dir_t;

extern d_px_t x_px_buff;
extern d_px_t y_px_buff;

int move_stepper_raw(const syspwm_t* pwm, gpiod_line *gpio_line, frequency_hz_t freq, step_t steps, step_dir_t dir);
int move_stepper(const syspwm_t* pwm, gpiod_line *gpio_line, frequency_hz_t freq, d_px_t d);

void* stepper_x_task(void* arg);
void* stepper_y_task(void* arg);

#ifdef __cplusplus
}
#endif

#endif // CTRL_MOTORS_H
