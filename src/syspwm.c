#include "syspwm.h"

/* Usefull PWM pins */

const syspwm_t SYSPWM_3 = { 0, 3 };
const syspwm_t SYSPWM_4 = { 0, 4 };

/* Helper function */

int write_sysfs(const char* path, const char* value)
{
    FILE* f = fopen(path, "w");
    if (f) {
        fputs(value, f);
        fclose(f);
        return EXIT_SUCCESS;
    } else {
        char err_msg[256];
        snprintf(err_msg, sizeof(err_msg), "[Error] Could not open %s", path);
        perror(err_msg);
        return EXIT_FAILURE;
    }
}

/* API functions */

bool syspwm_is_exported(const syspwm_t* pwm)
{
    char path[128];
    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip%u/pwm%u", pwm->chip_no, pwm->channel_no);
    struct stat st;
    return stat(path, &st) == 0;
}

int syspwm_init(const syspwm_t* pwm, period_ns_t period_ns, duty_percent_t duty_c)
{
    if (period_ns == 0 || duty_c > 100) {
        fprintf(stderr, "[Error] Invalid period or duty cycle\n");
        return EXIT_FAILURE;
    }

    char path[128], buf[32];
    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip%u/pwm%u/period", pwm->chip_no, pwm->channel_no);
    snprintf(buf, sizeof(buf), "%u", period_ns);
    if (write_sysfs(path, buf) != EXIT_SUCCESS) return EXIT_FAILURE;

    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip%u/pwm%u/duty_cycle", pwm->chip_no, pwm->channel_no);
    snprintf(buf, sizeof(buf), "%u", (uint32_t)(duty_c * period_ns / 100));
    if (write_sysfs(path, buf) != EXIT_SUCCESS) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int syspwm_check(const syspwm_t* pwm)
{
    if (!syspwm_is_exported(pwm)) {
        fprintf(stderr, "[Error] PWM pwmchip%u/pwm%u is not exported\n", pwm->chip_no, pwm->channel_no);
        return EXIT_FAILURE;
    }
    printf("[Info] PWM pwmchip%u/pwm%u is exported\n", pwm->chip_no, pwm->channel_no);

    char path[128], val[32];
    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip%u/pwm%u/period", pwm->chip_no, pwm->channel_no);
    FILE* f = fopen(path, "r");
    if (f) {
        if (fgets(val, sizeof(val), f)) {
            printf("[Info] Current period (ns): %s", val);
        }
        fclose(f);
    }

    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip%u/pwm%u/duty_cycle", pwm->chip_no, pwm->channel_no);
    f = fopen(path, "r");
    if (f) {
        if (fgets(val, sizeof(val), f)) {
            printf("[Info] Current duty cycle (ns): %s", val);
        }
        fclose(f);
    }

    return EXIT_SUCCESS;
}

int syspwm_enable(const syspwm_t* pwm, syspwm_state_t enable)
{
    char path[128];
    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip%u/pwm%u/enable", pwm->chip_no, pwm->channel_no);
    return write_sysfs(path, enable ? "1" : "0"); 
}


void syspwm_stepper_sig(const syspwm_t* pwm, frequency_hz_t freq, step_t steps)
{
    if (freq == 0) {
        fprintf(stderr, "[Error] Frequency cannot be zero\n");
        return;
    }
    period_ns_t period = 1000000000UL / freq;
    duty_percent_t duty = 50;

    syspwm_enable(pwm, SYSPWM_DISABLE);
    syspwm_init(pwm, period, duty);
    syspwm_enable(pwm, SYSPWM_ENABLE);

    time_us_t sleep_us = period / 1000;

    for (step_t i = 0; i < steps; ++i) {
        wait_interruptible_us(sleep_us, 1000, psig_kill_requested);
        if (psig_kill_requested()) break;
    }

    syspwm_enable(pwm, SYSPWM_DISABLE);
}
