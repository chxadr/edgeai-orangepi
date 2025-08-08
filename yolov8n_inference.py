from ultralytics import YOLO
import ctypes
import numpy as np

from libloader import clib

FRAME_WIDTH = 320
FRAME_HEIGHT = 240
CONF = 0.65

def task():
    
    print("[Info] Start YOLOv8n inference task")
    try:
        model = YOLO("models/best_full_integer_quant_edgetpu.tflite", task="detect", verbose=False)
        print("[Info] Model loaded successfully.")
    except Exception as e:
        print(f"[Error] Failed to load model: {e}")
        clib.thread_exit_ready()
        return
    
    x0_px = None
    y0_px = None
    
    while not clib.kill_requested():
        
        size = ctypes.c_size_t()
        ptr_in = clib.get_latest_frame(ctypes.byref(size))
        buf_in = ctypes.string_at(ptr_in, size.value)
        
        frame = np.frombuffer(buf_in, dtype=np.uint8).reshape((FRAME_HEIGHT, FRAME_WIDTH, 3))
        clib.free_frame(ptr_in)
        
        try:
            results = model.predict(frame, imgsz=224, conf=CONF, verbose=False)
            result = results[0]
        except Exception as e:
            print(f"[Error] Inference failed: {e}")
            break

        # Uncomment the following to display result on OrangePi screen
        ##try:
        ##    img = result.plot()
        ##    img_contig = np.ascontiguousarray(img)
        ##    ptr_out = img_contig.ctypes.data_as(ctypes.POINTER(ctypes.c_uint8))
        ##    clib.send_frame(ptr_out, img.shape[1], img.shape[0])
        ##except Exception as e:
        ##    print(f"[Error] Failed to send result: {e}")

        try:
            boxes = result.boxes
            boxes_xyxy = boxes.xyxy.cpu().numpy()
            confidences = boxes.conf.cpu().numpy()
            class_ids = boxes.cls.cpu().numpy().astype(int)
            class_names = result.names

            for i in range(len(boxes_xyxy)):
                inference_time_ms = result.speed.get('inference')
                fps = 1000.0 / inference_time_ms

                x1, y1, x2, y2 = boxes_xyxy[i]
                cx = int((x1 + x2)) // 2
                cy = int((y1 + y2)) // 2

                conf = confidences[i]
                cls_id = class_ids[i]
                cls_name = class_names[cls_id]

                print(f"[Info] Detections at {inference_time_ms:.2f} ms ({fps:.2f} FPS)"
                    f" Class: {cls_name} ({cls_id}), Conf: {conf:.2f}, Center: ({cx},{cy})")

                if x0_px is None or y0_px is None:
                    print(f"[Info] About to set ref to x0={cx} y0={cy}")
                    print("[Info] Press 'y' then Enter to validate, or just Enter to retry")
                    user_input = input().strip().lower()
                    if user_input == "y":
                        x0_px = cx
                        y0_px = cy
                        clib.send_abs_pos(x0_px, y0_px)
                        print(f"[Info] YOLOv8n reference position set to x0={x0_px}, y0={y0_px}")
                    else:
                        print("[Info] Retry calibration, moving to next detection.")
                elif cls_id == 0:
                    clib.send_abs_pos(cx, cy)
        except Exception as e:
            print(f"[Error] Detection processing failed: {e}")
            continue
    
    clib.thread_exit_ready()
    print("[Info] Stopping YOLOv8n inference task")
    return
