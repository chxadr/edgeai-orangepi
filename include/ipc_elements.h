#ifndef IPC_ELEMENTS_H
#define IPC_ELEMENTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <semaphore.h>


#define THREAD_NUMBER 4 //5 if display_thread ON


// Usefull variables
extern volatile uint8_t thread_ready_num;

// Mutexes and Semaphores
extern sem_t data_x_ready_sem;
extern sem_t data_y_ready_sem;
extern sem_t data_x_done_sem;
extern sem_t data_y_done_sem;
extern pthread_mutex_t cam_buffer_mutex;
extern pthread_mutex_t disp_buffer_mutex;

// API Functions
int ipc_init(void);
int ipc_release(void);
int ipc_close(void);

#ifdef __cplusplus
}
#endif

#endif // IPC_ELEMENTS_H
