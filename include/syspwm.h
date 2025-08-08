/**
 * @file syspwm.h
 * @author Adrien Chevrier
 * 
 * @brief Header file for sysfs PWM management on the Orange Pi Zero 3.
 *
 * This file provides the necessary function declarations and data structures for
 * driving Pulse Width Modulation (PWM) pins on the Orange Pi Zero 3. It includes
 * functions to initialize, enable, disable, and tune PWM signals, including the 
 * ability to drive stepper motors via PWM.
 *
 * The GNU/Linux kernel embedded on the Orange Pi Zero 3 was from version 5.4.125,
 * which at this time forced us to use sysfs to drive PWM signals.
 *
 * @see syspwm.c
 * @see The Orange Pi Zero 3 Wiki page on PWM management.
 *
 * - Orange Pi Zero 3 Wiki page about PWM -
 * http://www.orangepi.org/orangepiwiki/index.php/Orange_Pi_Zero_3#PWM_test_method
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

// Type definitions for PWM configuration
typedef uint32_t period_ns_t;       ///< Type for PWM period [ns].
typedef uint32_t frequency_hz_t;    ///< Type for PWM frequency [Hz].
typedef uint16_t step_t;            ///< Type for PWM step number.
typedef uint8_t duty_percent_t;     ///< Type for PWM duty cycle [%].

/**
 * @brief 
 * The two possible states of a PWM pin. Could be only written
 * to a PWM pin to power it on or off.
 */
typedef enum {
    SYSPWM_DISABLE, ///< Disable the PWM pin (power-off).
    SYSPWM_ENABLE   ///< Enable the PWM pin (power-on).
} syspwm_state_t;

/**
 * @brief
 * Data structure representing a PWM pin through its chip
 * number and channel number.
 * 
 * To declare a PWM pin:
 * @code
 * syspwm_t SYSPWM_3 = { 0, 3 };
 * @endcode
 * where the PWM chip number (0) and the PWM channel number (3)
 * can be found on the Orange Pi Zero 3 Wiki page. You will also
 * find a few indications to activate the PWM pins of your choice.
 * 
 * @note The PWM chip 0 needs UART0 to be turned off.
 * See the Orange Pi Zero 3 Wiki page.
 * 
 * - Orange Pi Zero 3 Wiki page about PWM -
 * http://www.orangepi.org/orangepiwiki/index.php/Orange_Pi_Zero_3#PWM_test_method
 */
typedef struct {
    uint8_t chip_no;
    uint8_t channel_no;
} syspwm_t;

// PWM pins used in this project
extern const syspwm_t SYSPWM_3; ///< PWM3
extern const syspwm_t SYSPWM_4; ///< PWM4

/**
 * @brief Check the status of a PWM pin.
 * 
 * This function checks if the specified PWM pin is already exported with sysfs
 * and prints its current configuration in that case.
 * 
 * @param[in] pwm A pointer to a @c syspwm_t structure representing the PWM pin.
 * @return @c EXIT_SUCCESS if the PWM pin is exported, @c EXIT_FAILURE otherwise.
 */
int syspwm_check(const syspwm_t* pwm);

/**
 * @brief Initialize a PWM pin with a specific period and duty cycle.
 * 
 * This function configures the PWM pin to operate with the specified period 
 * [ns] and duty cycle [%].
 * 
 * @param[in] pwm A pointer to a @c syspwm_t structure representing the PWM pin.
 * @param[in] period The period of the PWM signal [ns].
 * @param[in] duty_c The duty cycle of the PWM signal [%].
 * @return @c EXIT_SUCCESS on success, @c EXIT_FAILURE if the parameters
 *         are invalid or if sysfs is not accessible.
 */
int syspwm_init(const syspwm_t* pwm, period_ns_t period, duty_percent_t duty_c);

/**
 * @brief Enable or disable a PWM pin.
 * 
 * This function enables or disables the PWM output on the specified pin.
 * 
 * @param[in] pwm A pointer to a @c syspwm_t structure representing the PWM pin.
 * @param[in] enable The desired state for the PWM pin: @c SYSPWM_ENABLE or @c SYSPWM_DISABLE .
 * @return @c EXIT_SUCCESS on success, @c EXIT_FAILURE if sysfs is not accessible.
 */
int syspwm_enable(const syspwm_t* pwm, syspwm_state_t enable);


/**
 * @brief Generate a PWM signal suitable for driving stepper motors.
 * 
 * This function sends a PWM signal at given frequency and step count.
 * A step is a small part of the PWM signal that has the duration of one period.
 * This type of signal is typically used to drive stepper motors.
 * 
 * @param[in] pwm A pointer to a @c syspwm_t structure representing the PWM pin.
 * @param[in] freq The frequency of the PWM signal [Hz].
 * @param[in] steps The number of steps to generate.
 */
void syspwm_stepper_sig(const syspwm_t* pwm, frequency_hz_t freq, step_t steps);

#ifdef __cplusplus
}
#endif

#endif // SYSPWM_H
