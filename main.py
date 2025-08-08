from libloader import clib
import threading
import yolov8n_inference as yolov8n

EXIT_SUCCESS = 0
EXIT_FAILURE = 1

def main():
    
    if clib.init_board() != EXIT_SUCCESS:
        print("[Error] Abort main program")
        return EXIT_FAILURE
    
    if clib.spawn_threads() != EXIT_SUCCESS:
        print("[Error] Abort main program")
        return EXIT_FAILURE
    
    inference_thread = threading.Thread(target=yolov8n.task)
    inference_thread.start()
    
    clib.join_threads()
    inference_thread.join()
    
    clib.exit_clean()
    return EXIT_SUCCESS

if __name__ == "__main__":
    main()
