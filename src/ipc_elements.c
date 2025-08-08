/**
 * @file ipc_elements.c
 * @author Adrien Chevrier
 * 
 * @brief Implementation file for the header @c ipc_elements.h .
 *
 * @see ipc_elements.h
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

#include "ipc_elements.h"
#include <stdio.h>

// Counter for the number of threads that are ready.
volatile uint8_t thread_ready_num = 0;

// Mutexes and semaphores.
sem_t data_x_ready_sem;
sem_t data_y_ready_sem;
sem_t data_x_done_sem;
sem_t data_y_done_sem;
pthread_mutex_t cam_buffer_mutex;
pthread_mutex_t disp_buffer_mutex;

void ipc_init(void)
{
    pthread_mutex_init(&cam_buffer_mutex, NULL);
    //pthread_mutex_init(&disp_buffer_mutex, NULL);//
    sem_init(&data_x_ready_sem, 0, 0);
    sem_init(&data_y_ready_sem, 0, 0);
    sem_init(&data_x_done_sem, 0, 0);
    sem_init(&data_y_done_sem, 0, 0);
}

void ipc_release(void)
{
    while (thread_ready_num < THREAD_NUMBER) {
        sem_post(&data_x_ready_sem);
        sem_post(&data_y_ready_sem);
        sem_post(&data_x_done_sem);
        sem_post(&data_y_done_sem);
    }
}

void ipc_close(void)
{
	pthread_mutex_destroy(&cam_buffer_mutex);
    //pthread_mutex_destroy(&disp_buffer_mutex);//
    sem_destroy(&data_x_ready_sem);
    sem_destroy(&data_y_ready_sem);
    sem_destroy(&data_x_done_sem);
    sem_destroy(&data_y_done_sem);
}
