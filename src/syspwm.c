/**
 * @file syspwm.c
 * @author Adrien Chevrier
 * 
 * @brief Implementation file for the header @c syspwm.h .
 *
 * @see syspwm.h
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

#include "syspwm.h"

// Type definitions for PWM configuration
const syspwm_t SYSPWM_3 = { 0, 3 };
const syspwm_t SYSPWM_4 = { 0, 4 };


/*******************************************************************************
 * Helper functions
 ******************************************************************************/

 /**
  * @brief Write a value to a sysfs path.
  * 
  * This helper function writes a specified value to a given sysfs file, 
  * which is used for configuring PWM properties on older GNU/Linux-based systems.
  * 
  * @param[in] path The sysfs file path name where the value needs to be written.
  * @param[in] value The value to be written to the file, as a @c char* .
  * @return @c EXIT_SUCCESS` on success, @c EXIT_FAILURE if the file cannot be opened.
  */
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

/**
 * @brief Check if a PWM pin has been exported.
 * 
 * This function checks whether a specific PWM pin is exported and available 
 * for use by verifying the corresponding sysfs path.
 *
 * It tries to access the file's statistics using the function:
 * @code 
 * int stat(const char *path, struct stat *buf);
 * @endcode
 * which fails with @c EACCES error if the user running the program
 * does not have the permission to access the file.
 *
 * @note PWM pins have to be exported as root. You can create a service
 * that does it when booting the Orange Pi.
 * 
 * @param[in] pwm A pointer to a @c syspwm_t structure representing the PWM pin.
 * @return @c true if the PWM pin is exported, @c false otherwise.
 */
bool syspwm_is_exported(const syspwm_t* pwm)
{
    char path[128];
    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip%u/pwm%u", pwm->chip_no, pwm->channel_no);
    struct stat st;
    return stat(path, &st) == 0;
}


/*******************************************************************************
 * API functions
 ******************************************************************************/

int syspwm_init(const syspwm_t* pwm, period_ns_t period, duty_percent_t duty_c)
{
    // Error on wrong parameter values.
    if (period == 0 || duty_c > 100) {
        fprintf(stderr, "[Error] Invalid period or duty cycle\n");
        return EXIT_FAILURE;
    }

    // Write period.
    char path[128], buf[32];
    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip%u/pwm%u/period", pwm->chip_no, pwm->channel_no);
    snprintf(buf, sizeof(buf), "%u", period);
    if (write_sysfs(path, buf) != EXIT_SUCCESS) return EXIT_FAILURE;

    // Write duty cycle.
    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip%u/pwm%u/duty_cycle", pwm->chip_no, pwm->channel_no);
    snprintf(buf, sizeof(buf), "%u", (uint32_t)(duty_c * period / 100));
    if (write_sysfs(path, buf) != EXIT_SUCCESS) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}


int syspwm_check(const syspwm_t* pwm)
{
    // Check if the PWM pin is exported.
    if (!syspwm_is_exported(pwm)) {
        fprintf(stderr, "[Error] PWM pwmchip%u/pwm%u is not exported\n", pwm->chip_no, pwm->channel_no);
        return EXIT_FAILURE;
    }
    printf("[Info] PWM pwmchip%u/pwm%u is exported\n", pwm->chip_no, pwm->channel_no);

    // Print PWM current period [ns].
    char path[128], val[32];
    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip%u/pwm%u/period", pwm->chip_no, pwm->channel_no);
    FILE* f = fopen(path, "r");
    if (f) {
        if (fgets(val, sizeof(val), f)) {
            printf("[Info] Current period (ns): %s", val);
        }
        fclose(f);
    }

    // Print PWM current duty cycle [ns].
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
    // Avoid division by zero.
    if (freq == 0) {
        fprintf(stderr, "[Error] Frequency cannot be zero\n");
        return;
    }

    // Compute period and set fixed PWM duty cycle.
    period_ns_t period = 1000000000UL / freq;
    duty_percent_t duty = 50;

    // Change PWM parameters.
    syspwm_enable(pwm, SYSPWM_DISABLE);
    syspwm_init(pwm, period, duty);

    // Power-on the PWM pin.
    syspwm_enable(pwm, SYSPWM_ENABLE);

    // Compute signal duration.
    time_us_t duration = period / 1000;

    // Wait for the duration to complete or for a termination signal.
    /// TODO: Use GNU/Linux Kernel timer-based interrupts instead.
    for (step_t i = 0; i < steps; ++i) {
        wait_interruptible_us(duration, 1000, psig_kill_requested);
        if (psig_kill_requested()) break;
    }

    // Power-off the PWM pin.
    syspwm_enable(pwm, SYSPWM_DISABLE);
}
