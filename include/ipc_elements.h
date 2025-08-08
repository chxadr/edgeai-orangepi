/**
 * @file ipc_elements.h
 * @author Adrien Chevrier
 * 
 * @brief IPC (Inter-Process Communication) utilities for synchronization
 *        and data sharing.
 *
 * This file contains the declaration of the functions for initializing, releasing,
 * and closing IPC mechanisms (mutexes and semaphores) used for inter-thread synchronization
 * and communication, such as managing buffers for camera, motors, and display tasks.
 *
 * @see ipc_elements.c
 *
 * @note Uncomment statements commented with two pairs of dashes // ... //
 * to run the thread that displays inference results on screen.
 * Also modify ipc_elements.h, ipc_elements.cpp and yolov8n_inference.py
 * accordingly.
 *
 * @todo provide an easy way to display the results or not.
 *
 * @see display_result.h and display_result.cpp
 * @see ipc_elements.h and ipc_elements.cpp
 * @see yolov8n_inference.py 
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

#ifndef IPC_ELEMENTS_H
#define IPC_ELEMENTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <semaphore.h>

/**
 * @brief Number of threads in this application.
 *
 * 1 thread for the camera              (C++)
 * 1 thread for running inferences      (Python)
 * 2 threads to drive the motors        (C)
 * 1 thread to display inference result (C++)
 */
#define THREAD_NUMBER 4 //5// when displaying results.

/// Counter for the number of threads that are ready.
extern volatile uint8_t thread_ready_num;

// Mutexes and Semaphores
extern sem_t data_x_ready_sem;  ///< Semaphore for signaling when X coordinate data is ready.
extern sem_t data_y_ready_sem;  ///< Semaphore for signaling when Y coordinate data is ready.
extern sem_t data_x_done_sem;   ///< Semaphore for signaling when X coordinate data are consumed.
extern sem_t data_y_done_sem;   ///< Semaphore for signaling when Y coordinate data are consumed.
extern pthread_mutex_t cam_buffer_mutex;    ///< Mutex for controlling access to the camera buffer.
extern pthread_mutex_t disp_buffer_mutex;   ///< Mutex for controlling access to the display buffer.

/**
 * @brief Initializes the IPC mechanisms (mutexes and semaphores).
 * 
 * This function initializes the necessary mutexes and semaphores used for
 * thread synchronization and communication.
 */
void ipc_init(void);

/**
 * @brief Releases the IPC resources.
 * 
 * This function releases the semaphores for X and Y data.
 */
void ipc_release(void);

/**
 * @brief Closes and cleans up the IPC resources.
 * 
 * This function destroys the initialized mutexes and semaphores, freeing any
 * allocated resources. It is intended to be called when the system is shutting down.
 */
void ipc_close(void);

#ifdef __cplusplus
}
#endif

#endif // IPC_ELEMENTS_H
