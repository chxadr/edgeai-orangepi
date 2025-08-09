"""
yolov8n_inference.py
YOLOv8n inference task for Orange Pi hardware integration.

This script runs the YOLOv8n object detection model on frames provided by the
C++ camera thread. It processes detections, optionally sends annotated frames
back to the C++ display thread, and communicates object positions to the motor
control system (C threads).

References:
    - libloader.py
    - ultralytics (YOLOv8)
    - numpy

Author:
    Adrien Chevrier

Version:
    0.1 (2025-07-15)
    
Copyright:
    Copyright (c) 2025 Adrien Chevrier

License:
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
"""

from ultralytics import YOLO
import ctypes
import numpy as np

from libloader import clib

# Camera image buffer size.
FRAME_WIDTH = 320
FRAME_HEIGHT = 240

# Confidence threshold for inferences.
CONF = 0.65

def task():
    """
    Task body t run the YOLOv8n object detection loop.

    Loads the YOLOv8n model and processes camera frames obtained via the
    C interface. Sends detected object positions to the motor control system
    and optionally sends annotated frames to the display.

    Notes:
        - The task terminates when `kill_requested()` returns True.
        - Uses the Ultralytics YOLOv8 API for inference.
        - User is prompted for manual reference calibration on first detection.

    Raises:
        Exception: If model loading or inference fails.

    Returns:
        None
    """
    
    print("[Info] Start YOLOv8n inference task")
    try:
        # Load YOLOv8n model from a quantized EdgeTPU TFLite file.
        model = YOLO("models/best_full_integer_quant_edgetpu.tflite", task="detect", verbose=False)
        print("[Info] Model loaded successfully.")
    except Exception as e:
        print(f"[Error] Failed to load model: {e}")
        clib.thread_exit_ready()
        return
    
    # Calibration coordinates to send to the motors.
    x0_px = None
    y0_px = None
    
    # Inference loop that continues until a termination signal is received.
    while not clib.kill_requested():
        
        # Get latest frame from the C++ camera thread.
        size = ctypes.c_size_t()
        ptr_in = clib.get_latest_frame(ctypes.byref(size))
        buf_in = ctypes.string_at(ptr_in, size.value)
        frame = np.frombuffer(buf_in, dtype=np.uint8).reshape((FRAME_HEIGHT, FRAME_WIDTH, 3))
        
        # Free created C pointer.
        clib.free_frame(ptr_in)
        
        try:
            # Run inference on the captured frame at defined confidence threshold.
            results = model.predict(frame, imgsz=224, conf=CONF, verbose=False)
            result = results[0]
        except Exception as e:
            print(f"[Error] Inference failed: {e}")
            break

        # Uncomment the following to display result on OrangePi screen
        # Also modify ipc_elements.h, ipc_elements.c, c_interface.cpp
        # and yolov8n_inference.py accordingly.
        # =======================================================================
        ##try:
        ##    img = result.plot()
        ##    img_contig = np.ascontiguousarray(img)
        ##    ptr_out = img_contig.ctypes.data_as(ctypes.POINTER(ctypes.c_uint8))
        ##    clib.send_frame(ptr_out, img.shape[1], img.shape[0])
        ##except Exception as e:
        ##    print(f"[Error] Failed to send result: {e}")
        # =======================================================================

        try:
            # Get inference result attributes.
            boxes = result.boxes                            # Bounding boxes.
            boxes_xyxy = boxes.xyxy.cpu().numpy()           # Bounding boxes coordinates.
            confidences = boxes.conf.cpu().numpy()          # Bounding boxes confidence scores.
            class_ids = boxes.cls.cpu().numpy().astype(int) # Identified class-indexes.
            class_names = result.names                      # Identified class-names.

            # For each identified objects:
            for i in range(len(boxes_xyxy)):
                # Compute inference speed [FPS]
                inference_time_ms = result.speed.get('inference')
                fps = 1000.0 / inference_time_ms

                # Get object's center coordinates.
                x1, y1, x2, y2 = boxes_xyxy[i]
                cx = int((x1 + x2)) // 2
                cy = int((y1 + y2)) // 2

                # Get object's attributes.
                conf = confidences[i]           # Confidence score.
                cls_id = class_ids[i]           # Class index.
                cls_name = class_names[cls_id]  # Class name.

                print(f"[Info] Detections at {inference_time_ms:.2f} ms ({fps:.2f} FPS)"
                    f" Class: {cls_name} ({cls_id}), Conf: {conf:.2f}, Center: ({cx},{cy})")

                # If the motors have not been calibrated yet, pause inference loop.
                if x0_px is None or y0_px is None:
                    
                    # Prepare current detected object's position as reference for calibration.
                    print(f"[Info] About to set ref to x0={cx} y0={cy}")
                    print("[Info] Press 'y' then Enter to validate, or just Enter to retry")
                    user_input = input().strip().lower()
                    
                    # Save and send calibration position to motors when pressing `y`.
                    if user_input == "y":
                        x0_px = cx
                        y0_px = cy
                        clib.send_abs_pos(x0_px, y0_px)
                        print(f"[Info] YOLOv8n reference position set to x0={x0_px}, y0={y0_px}")
                    # Abort calibration: make another inference and ask for calibration again.
                    else:
                        print("[Info] Retry calibration, moving to next detection.")
                
                # Otherwise, if the detected class corresponds to an alien, send object's position to motors.
                elif cls_id == 0:
                    clib.send_abs_pos(cx, cy)
        
        except Exception as e:
            print(f"[Error] Detection processing failed: {e}")
            continue
    
    # Indicate the task is complete and release resources.
    clib.thread_exit_ready()
    print("[Info] Stopping YOLOv8n inference task")
    return
