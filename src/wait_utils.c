#include "wait_utils.h"

bool wait_interruptible_us(time_us_t total_us, time_us_t check_interval_us, bool (*exit_requested)(void))
{
    time_us_t waited_us = 0;
    while (waited_us < total_us) {
        if (exit_requested && exit_requested()) {
            return true; // exited early
        }

        time_us_t sleep_us = check_interval_us;
        if (total_us - waited_us < check_interval_us) {
            sleep_us = (time_us_t)(total_us - waited_us);
        }

        usleep(sleep_us);
        waited_us += sleep_us;
    }
    return false; // completed full wait
}

bool wait_interruptible_ms(time_ms_t total_ms, time_ms_t check_interval_ms, bool (*exit_requested)(void))
{
    return wait_interruptible_us(total_ms * 1000, check_interval_ms * 1000, exit_requested);
}
