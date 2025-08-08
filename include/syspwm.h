#ifndef SYSPWM_H
#define SYSPWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>

#include "wait_utils.h"
#include "psig_utils.h"

typedef uint32_t period_ns_t;
typedef uint32_t frequency_hz_t;
typedef uint16_t step_t;
typedef uint8_t duty_percent_t;
typedef uint8_t ramp_percent_t;

typedef enum {
    SYSPWM_DISABLE,
    SYSPWM_ENABLE
} syspwm_state_t;

typedef struct {
    uint8_t chip_no;
    uint8_t channel_no;
} syspwm_t;

/* Some usefull PWM pins for OrangePi */

extern const syspwm_t SYSPWM_3;
extern const syspwm_t SYSPWM_4;

bool syspwm_is_exported(const syspwm_t* pwm);

int syspwm_check(const syspwm_t* pwm);

int syspwm_init(const syspwm_t* pwm, period_ns_t period_ns, duty_percent_t duty_c);

int syspwm_enable(const syspwm_t* pwm, syspwm_state_t enable);

void syspwm_stepper_sig(const syspwm_t* pwm, frequency_hz_t freq, step_t steps);

#ifdef __cplusplus
}
#endif

#endif // SYSPWM_H
