#ifndef C_INTERFACE_H
#define C_INTERFACE_H

#include <pthread.h>
#include <sched.h>  // Manage priority
#include <iostream>
#include <cstring>

#include "setup.h"
#include "ipc_elements.h"
#include "ctrl_motors.h"
#include "camera.h"
#include "display_result.h"
#include "stepper_demo.h"

#ifdef __cplusplus
extern "C" {
#endif

int init_board(void);
int spawn_threads(void);
void join_threads(void);
void exit_clean(void);

void thread_exit_ready(void);
bool sigint_received(void);

void display_frame(uint8_t* data, int width, int height, int channels);
uint8_t* get_latest_frame(size_t* out_size);
void free_frame(uint8_t* ptr);
void send_abs_pos(d_px_t dx, d_px_t dy);

void circle_demo(d_px_t r, uint8_t n_pts, time_us_t delay_us);

#ifdef __cplusplus
}
#endif

#endif // C_INTERFACE_H
