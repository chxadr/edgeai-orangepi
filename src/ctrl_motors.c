/**
 * @file ctrl_motors.c
 * @author Adrien Chevrier
 * 
 * @brief Implementation file for the header @c ctrl_motors.h .
 *
 * @see ctrl_motors.h
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

#include "ctrl_motors.h"

// Coordinates buffers to read.
d_px_t x_px_buff;
d_px_t y_px_buff;

// Initial positions (to be calibrated).
static d_px_t x0_px = 0;
static d_px_t y0_px = 0;

int move_stepper_raw(const syspwm_t* pwm, gpiod_line *gpio_line, frequency_hz_t freq, step_t steps, step_dir_t dir)
{
    // Write on GPIO for direction.
    int exit_code = gpio_write(gpio_line, dir);
    // Generate PWM signal for steps.
    syspwm_stepper_sig(pwm, freq, steps);
    return exit_code;
}

int move_stepper(const syspwm_t* pwm, gpiod_line *gpio_line, frequency_hz_t freq, d_px_t d)
{
    step_dir_t dir;

    // Find direction from displacement sign.
    if (d <= 0) {
        d *= -1;
        dir = STEP_M;
    } else {
        dir = STEP_P;
    }

    // Convert pixel displacement to a number of steps.
    step_t steps = (step_t) (d / STEP_SIZE);
    return move_stepper_raw(pwm, gpio_line, freq, steps, dir);
}

void* stepper_x_task(void* arg)
{
    printf("[Info] Start x-stepper motor task\n");

    // Install signal handler for system signals.
    psig_install_handler();

    // Wait for calibration.
    printf("[Info] x-stepper waiting for cal...\n");
    sem_wait(&data_x_ready_sem);
    // Calibrate initial X-position.
    x0_px = x_px_buff;
    sem_post(&data_x_done_sem);
    printf("[Info] Set x-stepper ref to x=%d\n", x0_px);

    // Reading loop that continues until a termination signal is received.
    while (!psig_kill_requested()) {
        // Wait for a new X-position.
        sem_wait(&data_x_ready_sem);
        // Read X-position. 
        d_px_t x_px = x_px_buff;
        sem_post(&data_x_done_sem);
        printf("[Info] Received x=%d\n", x_px);
        // Move the motor.
        move_stepper(&PWM_STEP_X, dir_x_line, PWM_FREQ, x_px-x0_px);
        // Update previous position.
        x0_px = x_px;
    }

    // Indicate the X-motor task is complete and release resources.
    syspwm_enable(&PWM_STEP_X, SYSPWM_DISABLE);
    thread_ready_num++;
    printf("[Info] Stopping x-stepper motor task\n");
    pthread_exit(EXIT_SUCCESS);
}

void* stepper_y_task(void* arg)
{
    printf("[Info] Start y-stepper motor task\n");

    // Install signal handler for system signals
    psig_install_handler();

    // Wait for calibration.
    printf("[Info] y-stepper waiting for cal...\n");
    sem_wait(&data_y_ready_sem);
    // Calibrate initial Y-position.
    y0_px = y_px_buff;
    sem_post(&data_y_done_sem);
    printf("[Info] Set y-stepper ref to y=%d\n", y0_px);
    
    // Reading loop that continues until a termination signal is received.
    while (!psig_kill_requested()) {
        // Wait for a new Y-position.
        sem_wait(&data_y_ready_sem);
        // Read Y-position.
        d_px_t y_px = y_px_buff;
        sem_post(&data_y_done_sem);
        printf("[Info] Received y=%d\n", y_px);
        // Move the motor.
        move_stepper(&PWM_STEP_Y, dir_y_line, PWM_FREQ, y_px-y0_px);
        // Update previous position.
        y0_px = y_px;
    }

    // Indicate the Y-motor task is complete and release resources.
    syspwm_enable(&PWM_STEP_Y, SYSPWM_DISABLE);
    thread_ready_num++;
    printf("[Info] Stopping y-stepper motor task\n");
    pthread_exit(EXIT_SUCCESS);
}
