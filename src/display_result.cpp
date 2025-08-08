/**
 * @file display_result.cpp
 * @author Adrien Chevrier
 * 
 * @brief Implementation file for the header @c display_result.h .
 *
 * @see display_result.h
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

#include "display_result.h"

// Result buffer initialization. Holds only one frame.
cv::Mat disp_buffer_0(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3);

void* display_task(void* arg)
{
    printf("[Info] Start display task\n");

    // Compute display height.
    int display_height = DISPLAY_WIDTH * FRAME_HEIGHT / FRAME_WIDTH;

    // Import user's GUI settings.
    setenv("DISPLAY", ":0", 1);
    setenv("XAUTHORITY", "/home/orangepi/.Xauthority", 1);

    // Install signal handler for system signals
    psig_install_handler();
    cv::Mat frame;

    // Reading loop that read results until a termination signal is received.
    while (!psig_kill_requested()) {
        // Wait for a result image to be available.
        pthread_mutex_lock(&disp_buffer_mutex);
        frame = disp_buffer_0.clone();
        pthread_mutex_unlock(&disp_buffer_mutex);

        // If the image is not empty, copy it, resize it, and display it.
        if (!frame.empty()) {
            cv::Mat resized_frame;
            cv::resize(frame, resized_frame, cv::Size(DISPLAY_WIDTH, display_height), 0, 0, cv::INTER_LINEAR);
            cv::imshow("YOLOv8n result", resized_frame);
            cv::waitKey(1);
        }
    }

    // Indicate the task is complete and release resources.
    cv::destroyAllWindows();
    thread_ready_num++;
    printf("[Info] Stopping display task\n");
    pthread_exit(EXIT_SUCCESS);

}