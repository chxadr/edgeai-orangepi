/**
 * @file camera.cpp
 * @author Adrien Chevrier
 * 
 * @brief Implementation file for the header @c camera.h .
 *
 * @see camera.h
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

#include "camera.h"


// Buffer initialization. This buffer holds a single frame.
cv::Mat cam_buffer_0(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3);


void* camera_task(void* arg)
{
    printf("[Info] Start camera task\n");

    // Install signal handler for system signals.
    psig_install_handler();

    // Open the default camera (device 0) using V4L2 capture backend.
    cv::VideoCapture cap(0, cv::CAP_V4L2);
    if (!cap.isOpened()) {
        perror("[Error] Could not open camera\n");
        perror("[Error] Abort camera task\n");
        thread_ready_num++;
        pthread_exit(nullptr);
    }

    printf("[Info] Set camera parameters ...\n");

    // Set camera capture parameters: codec, resolution, and FPS.
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(cv::CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
    cap.set(cv::CAP_PROP_FPS, FRAME_FPS);

    // Retrieve and log the actual camera parameters for verification.
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

    // Print out the camera settings to ensure they are correct.
    printf("[Info] Check camera parameters ...\n");
    printf("[Info] Camera capture FOURCC: %s\n", set_fourcc);
    printf("[Info] Camera capture Width: %d\n", set_h);
    printf("[Info] Camera capture Height: %d\n", set_w);
    printf("[Info] Camera capture FPS: %.2f\n", set_fps);

    cv::Mat frame;

    // Capture loop that continues until a termination signal is received.
    while (!psig_kill_requested()) {

        // Capture a frame from the camera.
        cap >> frame;

        // Check if the frame is empty and continue if so.
        if (frame.empty()) {
            printf("[Warning] Empty image captured\n");
            continue;
        }

        // Try to lock the mutex before writing to the shared camera buffer.
        if (pthread_mutex_trylock(&cam_buffer_mutex) == 0) {
            cam_buffer_0 = frame.clone();
            pthread_mutex_unlock(&cam_buffer_mutex);
        }

        // If lock failed, just continue to capture.

    }

    // Indicate the camera task is complete and release resources.
    thread_ready_num++;
    printf("[Info] Stopping camera task\n");
    cap.release();
    pthread_exit(EXIT_SUCCESS);
}
