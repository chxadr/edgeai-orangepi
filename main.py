"""
main.py
Main entry point for the Python–C/C++ integrated application.

This script initializes the hardware and system resources via the C interface,
spawns the C/C++ threads for core functionality (motor control, camera, etc.),
and starts the Python-based YOLOv8n inference thread.

References:
    - yolov8n_inference.py
    - libloader.py

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

from libloader import clib
import threading
import yolov8n_inference as yolov8n

EXIT_SUCCESS = 0
EXIT_FAILURE = 1

def main():
    
    # Hardware and IPC initialization.
    if clib.init_board() != EXIT_SUCCESS:
        print("[Error] Abort main program")
        return EXIT_FAILURE
    
    # Spawm C/C++ threads: camera, motors, and optionally display.
    if clib.spawn_threads() != EXIT_SUCCESS:
        print("[Error] Abort main program")
        return EXIT_FAILURE
    
    # Spawn YOLOv8n inference Python thread.
    inference_thread = threading.Thread(target=yolov8n.task)
    inference_thread.start()
    
    # Wait for all threads to terminate.
    clib.join_threads()
    inference_thread.join()
    
    # Release resources and hardware.
    clib.exit_clean()
    return EXIT_SUCCESS

if __name__ == "__main__":
    main()
