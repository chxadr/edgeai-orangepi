#include "stepper_demo.h"

void line(d_px_t dx, d_px_t dy)
{
    x_px_buff = dx;
    sem_post(&data_x_ready_sem);
    y_px_buff = dy;
    sem_post(&data_y_ready_sem);

    printf("[Info] sent dx=%d, dy=%d\n", x_px_buff, y_px_buff);
    sem_wait(&data_x_done_sem);
    sem_wait(&data_y_done_sem);
}

void circle(d_px_t r, uint8_t n_pts, time_us_t delay_us)
{
    double da = 2 * M_PI / n_pts;

    for (uint8_t i = 0; i < n_pts; i++) {
        if (psig_kill_requested()) break;

        double a = da * i;
        d_px_t dx = (d_px_t)(r * cos(a + da));
        d_px_t dy = (d_px_t)(r * sin(a + da));
        printf("[Info] Point %d: ", i);
        line(dx, dy);

        wait_interruptible_us(delay_us, 1000, psig_kill_requested);
    }
}
