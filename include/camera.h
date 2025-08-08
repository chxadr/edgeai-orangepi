#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/opencv.hpp>
#include <iostream>

#include "psig_utils.h"
#include "ipc_elements.h"


#define FRAME_WIDTH 320
#define FRAME_HEIGHT 240
#define FRAME_FPS 120

extern cv::Mat cam_buffer_0;

void* camera_task(void* arg);

#endif  // CAMERA_H
