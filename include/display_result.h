/**
 * @file display_result.h
 * @author Adrien Chevrier
 * 
 * @brief Header file for the display task responsible for showing
 *        inference results.
 *
 * This file defines the necessary functions and data structures to display
 * YOLOv8n inference results on the screen. The display task waits for an
 * image to be placed in the buffer, resizes it using linear interpolation
 * for better visibility, and then displays the image on the screen.
 *
 * @see display_result.cpp
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

#ifndef DISPLAY_RESULT_H
#define DISPLAY_RESULT_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>

#include "psig_utils.h"
#include "ipc_elements.h"
#include "camera.h"

/// Largest screen dimension, usually the width [px].
#define DISPLAY_WIDTH 1280

/// Buffer where to read the frame to display. Holds only one frame.
extern cv::Mat disp_buffer_0;

/**
 * @brief Task to display YOLOv8n inference results.
 * 
 * This task waits for a result image to be placed in the buffer,
 * then reads the buffer to display the image on the screen. To improve visibility,
 * since the resolution is likely to be low, it first resizes the image
 * using linear interpolation.
 * 
 * @param[in] arg A pointer to any necessary arguments for the camera task.
 * @return A pointer to a result of the task execution.
 */
void* display_task(void* arg);

#endif // DISPLAY_RESULT_H
