#ifndef STEPPER_DEMO_H
#define STEPPER_DEMO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>

#include "ctrl_motors.h"
#include "ipc_elements.h"
#include "wait_utils.h"

void line(d_px_t dx, d_px_t dy);
void circle(d_px_t r, uint8_t n_pts, time_us_t delay_us);

#ifdef __cplusplus
}
#endif

#endif // STEPPER_DEMO_H