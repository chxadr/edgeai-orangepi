#include "camera.h"

cv::Mat cam_buffer_0(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3);

void* camera_task(void* arg)
{

    printf("[Info] Start camera task\n");

    psig_install_handler();
    cv::VideoCapture cap(0, cv::CAP_V4L2);
    if (!cap.isOpened()) {
        perror("[Error] Could not open camera\n");
        perror("[Error] Abort camera task\n");
        thread_ready_num++;
        pthread_exit(nullptr);
    }

    printf("[Info] Set camera parameters ...\n");

    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(cv::CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
    cap.set(cv::CAP_PROP_FPS, FRAME_FPS);

    int set_h = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int set_w = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    double set_fps = cap.get(cv::CAP_PROP_FPS);

    int set_fourcc_raw = static_cast<int>(cap.get(cv::CAP_PROP_FOURCC));
    char set_fourcc[] = {
        static_cast<char>(set_fourcc_raw & 0xFF),
        static_cast<char>((set_fourcc_raw >> 8) & 0xFF),
        static_cast<char>((set_fourcc_raw >> 16) & 0xFF),
        static_cast<char>((set_fourcc_raw >> 24) & 0xFF),
        0
    };

    printf("[Info] Check camera parameters ...\n");
    printf("[Info] Camera capture FOURCC: %s\n", set_fourcc);
    printf("[Info] Camera capture Width: %d\n", set_h);
    printf("[Info] Camera capture Height: %d\n", set_w);
    printf("[Info] Camera capture FPS: %.2f\n", set_fps);

    cv::Mat frame;

    while (!psig_kill_requested()) {

        cap >> frame;

        if (frame.empty()) {
            printf("[Warning] Empty image captured\n");
            continue;
        }

        if (pthread_mutex_trylock(&cam_buffer_mutex) == 0) {
            cam_buffer_0 = frame.clone();
            pthread_mutex_unlock(&cam_buffer_mutex);
        }

    }

    thread_ready_num++;
    printf("[Info] Stopping camera task\n");
    cap.release();
    pthread_exit(EXIT_SUCCESS);
}
