/**
 * @file c_interface.cpp
 * @author Adrien Chevrier
 * 
 * @brief Implementation file for the header @c c_interface.h .
 *
 * @see c_interface.h
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

#include "c_interface.h"

// Declare thread strcutures.
static pthread_t camera_thread;
//static pthread_t display_thread;//
static pthread_t stepper_x_thread;
static pthread_t stepper_y_thread;

/*******************************************************************************
 * Board and Thread Management
 ******************************************************************************/

int init_board(void)
{
    if (gpio_setup() == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    if (pwm_setup() == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

	ipc_init();

	return EXIT_SUCCESS;
}

int spawn_threads(void)
{

	if (pthread_create(&camera_thread, nullptr, camera_task, nullptr) != 0) {
		std::cerr << "[Error] Could not create task for camera" << std::endl;
		return EXIT_FAILURE;
	}

	if (pthread_create(&stepper_x_thread, nullptr, stepper_x_task, nullptr) != 0) {
		std::cerr << "[Error] Could not create task for x-stepper motor" << std::endl;
		return EXIT_FAILURE;
	}

	if (pthread_create(&stepper_y_thread, nullptr, stepper_y_task, nullptr) != 0) {
		std::cerr << "[Error] Could not create task for y-stepper motor" << std::endl;
		return EXIT_FAILURE;
	}

	//if (pthread_create(&display_thread, nullptr, display_task, nullptr) != 0) {//
	//	std::cerr << "[Error] Could not create task for display" << std::endl;	 //
	//	return EXIT_FAILURE;													 //
	//}																			 //

	return EXIT_SUCCESS;
}

void join_threads(void)
{
	pthread_join(camera_thread, nullptr);
	//pthread_join(display_thread, nullptr);//
	pthread_join(stepper_x_thread, nullptr);
	pthread_join(stepper_y_thread, nullptr);
}

void exit_clean(void)
{
	ipc_close();
	gpio_close();
}

/*******************************************************************************
 * Signal Management
 ******************************************************************************/

void thread_exit_ready(void)
{
    thread_ready_num++;
}

bool kill_requested(void)
{
	return psig_kill_requested();
}

/*******************************************************************************
 * Camera/Display Operations
 ******************************************************************************/

void send_frame(uint8_t* data, int width, int height)
{
	// Create a Mat object from the raw data and store it in the display buffer.
	// Does nothing if it failed to lock the mutex.
    if (pthread_mutex_trylock(&disp_buffer_mutex) == 0) {
		disp_buffer_0 = cv::Mat(height, width, CV_8UC3, data).clone(); 
		pthread_mutex_unlock(&disp_buffer_mutex);
	}
}

uint8_t* get_latest_frame(size_t* out_size)
{
	// Wait until a frame is available.
    pthread_mutex_lock(&cam_buffer_mutex);
    size_t data_size = cam_buffer_0.total() * cam_buffer_0.elemSize();
    uint8_t* buffer = (uint8_t*)malloc(data_size);

	// Copy the data from the camera buffer to the new buffer.
    memcpy(buffer, cam_buffer_0.data, data_size);

    pthread_mutex_unlock(&cam_buffer_mutex);

	// Return data and size.
    *out_size = data_size;
    return buffer;
}

void free_frame(uint8_t* ptr)
{
    free(ptr);
}

/*******************************************************************************
 * Communication with stepper motors
 ******************************************************************************/

void send_abs_pos(d_px_t x, d_px_t y)
{
	write_abs_pos(x, y);
}

void circle_demo(d_px_t r, uint8_t n_pts, time_us_t delay)
{
	circle(r, n_pts, delay);
}
