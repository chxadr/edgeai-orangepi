/**
 * @file c_interface.h
 * @author Adrien Chevrier
 * 
 * @brief Header file providing an interface between Pyhton scripts and C/C++
 * programs, allowing the main Python thread to control C/C++ threads, and
 * the Python inference thread to receive/send data to C/C++ threads.
 *
 * @see c_interface.cpp
 *
 * @see main.py
 * @see yolov8n_inference.py
 * @see libloader.py
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

#ifndef C_INTERFACE_H
#define C_INTERFACE_H

#include <pthread.h>
#include <sched.h>
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

/*******************************************************************************
 * Board and Thread Management
 ******************************************************************************/

/**
 * @brief Initializes the hardware and system setup.
 *
 * This function sets up the system GPIOs and verify that the PWM channels
 * have been already exported. It also initializes all mutexes and semaphores.
 *
 * @return @c EXIT_SUCCESS on success, @c EXIT_FAILURE otherwise.
 */
int init_board(void);

/**
 * @brief Spawns the necessary C/C++ threads for system operation.
 *
 * This function spawns all the required C/C++ threads that handle different tasks 
 * such as motor control and camera capture/display.
 *
 * @return @c EXIT_SUCCESS on success, @c EXIT_FAILURE if one thread could not be created.
 */
int spawn_threads(void);

/**
 * @brief Joins the spawned C/C++ threads.
 *
 * This function waits for the spawned C/C++ threads to finish execution.
 */
void join_threads(void);

/**
 * @brief Cleans up the system before exit.
 *
 * This function handles any necessary cleanup operations, such as releasing 
 * resources by destroying mutexes and semaphores and closing GPIOs, before 
 * exiting the system.
 */
void exit_clean(void);

/*******************************************************************************
 * Signal Management
 ******************************************************************************/

/**
 * @brief Marks the thread as ready to exit.
 *
 * This function notifies that the current thread is ready to exit, allowing 
 * the system to manage the thread lifecycle.
 * 
 * @warning Must be called by any Python thread before it exits.
 */
void thread_exit_ready(void);

/**
 * @brief Checks if a termination signal has been received.
 *
 * @return @c true if a termination signal has been received, otherwise @c false.
 */
bool kill_requested(void);

/*******************************************************************************
 * Camera/Display Operations
 ******************************************************************************/

/**
 * @brief Sends a frame to be displayed.
 *
 * This function takes a frame (raw pixel data) and stores it in the display buffer 
 * for later rendering by the thread in charge of displaying the result.
 *
 * @param[in] data Pointer to the raw image data.
 * @param[in] width The width of the image frame.
 * @param[in] height The height of the image frame.
 */
void send_frame(uint8_t* data, int width, int height);


/**
 * @brief Retrieves the latest captured frame from the camera.
 *
 * This function copies the most recent camera frame into a newly allocated buffer. 
 * The function also calculates the size of the frame and returns it through
 * the @c out_size parameter.
 *
 * @param[in,out] out_size Pointer to a variable that will hold the size of the frame.
 *
 * @return A pointer to the raw frame data. Can be @c nullptr .
 * 
 * @warning The caller is responsible for freeing the memory.
 */
uint8_t* get_latest_frame(size_t* out_size);

/**
 * @brief Frees the memory allocated for a frame.
 *
 * This function frees the memory that was allocated for storing a captured 
 * image frame. It is important to call this function after the frame is no 
 * longer needed to avoid memory leaks.
 *
 * @param[in,out] ptr Pointer to the raw frame data to be freed.
 *
 * @warning Must be called by any Python thread
 * copying frames from the camera thread buffer.
 */
void free_frame(uint8_t* ptr);

/*******************************************************************************
 * Communication with stepper motors
 ******************************************************************************/

 /**
 * @brief Sends an absolute position on the image to the motor control system.
 *
 * This function sends the target absolute position for the stepper motors 
 * to the control threads.
 *
 * @param[in] x The target absolute X coordinate [px].
 * @param[in] y The target absolute Y coordinate [px].
 */
void send_abs_pos(d_px_t x, d_px_t y);

/**
 * @brief Sends positions to move the stepper motors in a circular pattern.
 *
 * This function sends the coordinates of points that form a circle
 * to the motor control threads.
 *
 * @param[in] r The radius of the circle to draw [px].
 * @param[in] n_pts The number of points used to approximate the circle.
 * @param[in] delay The delay between each movement [us].
 */
void circle_demo(d_px_t r, uint8_t n_pts, time_us_t delay);

#ifdef __cplusplus
}
#endif

#endif // C_INTERFACE_H
