#include "ipc_elements.h"
#include <stdio.h>

// Usefull variables
volatile uint8_t thread_ready_num;

// Mutexes and semaphores
sem_t data_x_ready_sem;
sem_t data_y_ready_sem;
sem_t data_x_done_sem;
sem_t data_y_done_sem;
pthread_mutex_t cam_buffer_mutex;
pthread_mutex_t disp_buffer_mutex;

/* API */

int ipc_init(void)
{
    pthread_mutex_init(&cam_buffer_mutex, NULL);
    //pthread_mutex_init(&disp_buffer_mutex, NULL);

    sem_init(&data_x_ready_sem, 0, 0);
    sem_init(&data_y_ready_sem, 0, 0);
    sem_init(&data_x_done_sem, 0, 0);
    sem_init(&data_y_done_sem, 0, 0);

    return EXIT_SUCCESS;
}

int ipc_release(void)
{
    while (thread_ready_num < THREAD_NUMBER) {
        sem_post(&data_x_ready_sem);
        sem_post(&data_y_ready_sem);
        sem_post(&data_x_done_sem);
        sem_post(&data_y_done_sem);
    }
    return EXIT_SUCCESS;
}

int ipc_close(void)
{
	pthread_mutex_destroy(&cam_buffer_mutex);
    //pthread_mutex_destroy(&disp_buffer_mutex);

    sem_destroy(&data_x_ready_sem);
    sem_destroy(&data_y_ready_sem);
    sem_destroy(&data_x_done_sem);
    sem_destroy(&data_y_done_sem);

    return EXIT_SUCCESS;
}
