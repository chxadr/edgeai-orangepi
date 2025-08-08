#include "ctrl_motors.h"

d_px_t x_px_buff;
d_px_t y_px_buff;

static d_px_t x0_px = 0;
static d_px_t y0_px = 0;

int move_stepper_raw(const syspwm_t* pwm, gpiod_line *gpio_line, frequency_hz_t freq, step_t steps, step_dir_t dir)
{
    gpio_write(gpio_line, dir);
    syspwm_stepper_sig(pwm, freq, steps);
    return EXIT_SUCCESS;
}

int move_stepper(const syspwm_t* pwm, gpiod_line *gpio_line, frequency_hz_t freq, d_px_t d)
{
    step_dir_t dir;

    if (d <= 0) {
        d *= -1;
        dir = STEP_M;
    } else {
        dir = STEP_P;
    }

    step_t steps = (step_t) (d / STEP_SIZE);
    move_stepper_raw(pwm, gpio_line, freq, steps, dir);
    return EXIT_SUCCESS;
}

void* stepper_x_task(void* arg)
{
    psig_install_handler();
    printf("[Info] Start x-stepper motor task\n");

    printf("[Info] x-stepper waiting for cal...\n");
    sem_wait(&data_x_ready_sem);
    x0_px = x_px_buff;
    sem_post(&data_x_done_sem);
    printf("[Info] Set x-stepper ref to x=%d\n", x0_px);

    while (!psig_kill_requested()) {
        sem_wait(&data_x_ready_sem);
        d_px_t x_px = x_px_buff;
        sem_post(&data_x_done_sem);
        printf("[Info] Received dx=%d\n", x_px);
        move_stepper(&PWM_STEP_X, dir_x_line, PWM_FREQ, x_px-x0_px);
        x0_px = x_px;
    }

    syspwm_enable(&PWM_STEP_X, SYSPWM_DISABLE);
    thread_ready_num++;
    printf("[Info] Stopping x-stepper motor task\n");
    pthread_exit(EXIT_SUCCESS);
}

void* stepper_y_task(void* arg)
{
    psig_install_handler();
    printf("[Info] Start y-stepper motor task\n");

    printf("[Info] y-stepper waiting for cal...\n");
    sem_wait(&data_y_ready_sem);
    y0_px = y_px_buff;
    sem_post(&data_y_done_sem);
    printf("[Info] Set y-stepper ref to y=%d\n", y0_px);
    
    while (!psig_kill_requested()) {
        sem_wait(&data_y_ready_sem);
        d_px_t y_px = y_px_buff;
        sem_post(&data_y_done_sem);
        printf("[Info] Received dy=%d\n", y_px);
        move_stepper(&PWM_STEP_Y, dir_y_line, PWM_FREQ, y_px-y0_px);
        y0_px = y_px;
    }

    syspwm_enable(&PWM_STEP_Y, SYSPWM_DISABLE);
    thread_ready_num++;
    printf("[Info] Stopping y-stepper motor task\n");
    pthread_exit(EXIT_SUCCESS);
}
