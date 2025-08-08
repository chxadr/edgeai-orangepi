#include "display_result.h"

cv::Mat disp_buffer_0(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3);

void* display_task(void* arg)
{
    printf("[Info] Start display task\n");
    int display_height = DISPLAY_WIDTH * FRAME_HEIGHT / FRAME_WIDTH;

    setenv("DISPLAY", ":0", 1);
    setenv("XAUTHORITY", "/home/orangepi/.Xauthority", 1);

    psig_install_handler();
    cv::Mat frame;

    while (!psig_kill_requested()) {
        pthread_mutex_lock(&disp_buffer_mutex);
        frame = disp_buffer_0.clone();
        pthread_mutex_unlock(&disp_buffer_mutex);

        if (!frame.empty()) {
            cv::Mat resized_frame;
            cv::resize(frame, resized_frame, cv::Size(DISPLAY_WIDTH, display_height), 0, 0, cv::INTER_LINEAR);
            cv::imshow("YOLOv8n result", resized_frame);
            cv::waitKey(1);
        }
    }

    cv::destroyAllWindows();
    thread_ready_num++;
    printf("[Info] Stopping display task\n");
    pthread_exit(EXIT_SUCCESS);

}