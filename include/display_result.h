#ifndef DISPLAY_RESULTS_H
#define DISPLAY_RESULTS_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>

#include "psig_utils.h"
#include "ipc_elements.h"
#include "camera.h"


#define DISPLAY_WIDTH 1280

extern cv::Mat disp_buffer_0;

void* display_task(void* arg);

#endif // DISPLAY_RESULTS_H
