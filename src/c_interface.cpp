#include "c_interface.h"

static pthread_t camera_thread;
//static pthread_t display_thread;
static pthread_t stepper_x_thread;
static pthread_t stepper_y_thread;

int init_board(void)
{
    if (gpio_setup() == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    if (pwm_setup() == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

	return EXIT_SUCCESS;
}

int spawn_threads(void)
{
	ipc_init();

	if (pthread_create(&camera_thread, nullptr, camera_task, nullptr) != 0) {
		std::cout << "[Error] Could not create task for camera" << std::endl;
		return EXIT_FAILURE;
	}

	if (pthread_create(&stepper_x_thread, nullptr, stepper_x_task, nullptr) != 0) {
		std::cout << "[Error] Could not create task for x-stepper motor" << std::endl;
		return EXIT_FAILURE;
	}

	if (pthread_create(&stepper_y_thread, nullptr, stepper_y_task, nullptr) != 0) {
		std::cout << "[Error] Could not create task for y-stepper motor" << std::endl;
		return EXIT_FAILURE;
	}

	//if (pthread_create(&display_thread, nullptr, display_task, nullptr) != 0) {
	//	std::cout << "[Error] Could not create task for display" << std::endl;
	//	return EXIT_FAILURE;
	//}

	return EXIT_SUCCESS;
}

void join_threads(void)
{
	pthread_join(camera_thread, nullptr);
	//pthread_join(display_thread, nullptr);
	pthread_join(stepper_x_thread, nullptr);
	pthread_join(stepper_y_thread, nullptr);
}

void exit_clean(void)
{
	ipc_close();
	gpio_close();
}

void thread_exit_ready(void)
{
    thread_ready_num++;
}

bool sigint_received(void)
{
	return psig_kill_requested();
}

void display_frame(uint8_t* data, int width, int height, int channels)
{
    if (pthread_mutex_trylock(&disp_buffer_mutex) == 0) {
		disp_buffer_0 = cv::Mat(height, width, CV_8UC3, data).clone(); 
		pthread_mutex_unlock(&disp_buffer_mutex);
	}
}

uint8_t* get_latest_frame(size_t* out_size)
{
    pthread_mutex_lock(&cam_buffer_mutex);
    size_t data_size = cam_buffer_0.total() * cam_buffer_0.elemSize();
    uint8_t* buffer = (uint8_t*)malloc(data_size);
    memcpy(buffer, cam_buffer_0.data, data_size);
    pthread_mutex_unlock(&cam_buffer_mutex);

    *out_size = data_size;
    return buffer;
}

void free_frame(uint8_t* ptr)
{
    free(ptr);
}

void send_abs_pos(d_px_t dx, d_px_t dy)
{
	line(dx, dy);
}

void circle_demo(d_px_t r, uint8_t n_pts, time_us_t delay_us)
{
	circle(r, n_pts, delay_us);
}
