/**
 * @file camera.h
 * @author Adrien Chevrier
 * 
 * @brief Header file for the Camera module.
 * 
 * This file defines the interface for the Camera module, which handles capturing
 * frames from a camera using OpenCV. It includes macros to define the camera
 * operating resolution and frame rate, output image buffer definition,
 * and camera task declaration for multithreading.
 *
 * @see camera.cpp
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

#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/opencv.hpp>
#include <iostream>

#include "psig_utils.h"
#include "ipc_elements.h"

// Constant definitions for frame width, height, and frame rate.
#define FRAME_WIDTH 320     ///< Frame width [px].
#define FRAME_HEIGHT 240    ///< Frame height [px].
#define FRAME_FPS 120       ///< Frame rate [FPS].

// External variable holding the camera frame buffer.
extern cv::Mat cam_buffer_0;    ///< Camera frame buffer, holds a single image.

/**
 * @brief Camera task to capture an store frames from the camera.
 * 
 * This task initializes the camera
 * and continually captures frames from it.
 * Each time a valide frame is captured, it is copyied in a buffer,
 * effectively erasing the previous stored frame.
 * 
 * @param[in] arg A pointer to any necessary arguments for the camera task.
 * @return A pointer to a result of the task execution.
 */
void* camera_task(void* arg);

#endif  // CAMERA_H
