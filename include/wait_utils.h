#ifndef WAIT_UTILS_H
#define WAIT_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>


typedef uint32_t time_us_t;
typedef uint16_t time_ms_t;

/**
 * Sleeps for the specified duration in microseconds, but in small chunks,
 * allowing checks for shutdown conditions or interrupts.
 *
 * @param total_us Total duration to sleep in microseconds.
 * @param check_interval_us Interval in microseconds to check the condition.
 * @param (*exit_requested)(void) Pointer to function that returns true if the wait should abort early.
 *
 * @return true if exited early, false if completed entire wait.
 */
bool wait_interruptible_us(time_us_t total_us, time_us_t check_interval_us, bool (*exit_requested)(void));

/**
 * Convenience wait in milliseconds.
 */
bool wait_interruptible_ms(time_ms_t total_ms, time_ms_t check_interval_ms, bool (*exit_requested)(void));

#ifdef __cplusplus
}
#endif

#endif // WAIT_UTILS_H
