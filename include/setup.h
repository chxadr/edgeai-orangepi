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

// GPIO chip
#define GPIO_CHIP "gpiochip1"

// Motors direction controls
#define GPIO_DIR_X 75
#define GPIO_DIR_Y 226

// Limit switch sensors
#define GPIO_LIM_X 233
#define GPIO_LIM_Y 74

// Master emergency switch
#define GPIO_M_SW 78

/*******************************************************************************
 * GPIO chip and lines data structures
 ******************************************************************************/

// GPIO chip
extern gpiod_chip* gpio_chip;

// GPIO lines
extern gpiod_line* dir_x_line;
extern gpiod_line* dir_y_line;
extern gpiod_line* lim_x_line;
extern gpiod_line* lim_y_line;
extern gpiod_line* m_sw_line;

/*******************************************************************************
 * PWM channels
 ******************************************************************************/

#define PWM_STEP_X SYSPWM_4
#define PWM_STEP_Y SYSPWM_3

/*******************************************************************************
 * Constants for output Signals and Ratios
 ******************************************************************************/

#define STEP_RATIO 0.5F  // Stepper motors step resolution
#define PWM_FREQ 350     // PWM Final Frequency [Hz]
#define PWM_DCY 0.5F     // PWM Duty Cycle
#define STEP_SIZE 2      // 1 Step ~ 2 px in current config

/*******************************************************************************
 * GPIO and PWM chip and lines data structures init/close functions
 ******************************************************************************/

int gpio_setup(void);
int pwm_setup(void);
int gpio_close(void);

#ifdef __cplusplus
}
#endif

#endif // SETUP_H
