"""
libloader.py
c_interface Python Bindings.

This module provides a Python interface to the C/C++ functions defined in
`c_interface.h` and implemented in the corresponding shared library. It allows
Python scripts to control C/C++ threads, send and receive data such as image
frames, and communicate with stepper motors.

Typical usage includes:
    - Initializing the board and system resources.
    - Spawning and joining C/C++ worker threads.
    - Sending/receiving camera frames for processing or display.
    - Sending position commands to stepper motors.

References:
    - c_interface.h
    - c_interface.cpp

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

import ctypes

clib = ctypes.CDLL("./lib/libc_interface.so")

"""Initializes the hardware and system setup.

C signature:
    int init_board(void);

Sets up the system GPIOs, verifies that the PWM channels have already been
exported, and initializes all mutexes and semaphores.

Returns:
    int: ``0`` on success, non-zero on failure.
"""
clib.init_board.argtypes = []
clib.init_board.restype = ctypes.c_int

"""Spawns the necessary C/C++ threads for system operation.

C signature:
    int spawn_threads(void);

Creates all required worker threads for tasks such as motor control
and camera capture/display.

Returns:
    int: ``0`` on success, non-zero if any thread creation fails.
"""
clib.spawn_threads.argtypes = []
clib.spawn_threads.restype = ctypes.c_int

"""Joins all spawned C/C++ threads.

C signature:
    void join_threads(void);

Blocks until all worker threads finish execution.
"""
clib.join_threads.argtypes = []
clib.join_threads.restype = None

"""Cleans up the system before exit.

C signature:
    void exit_clean(void);

Releases resources, destroys mutexes and semaphores, and closes GPIOs
before terminating.
"""
clib.exit_clean.argtypes = []
clib.exit_clean.restype = None

"""Marks the current Python thread as ready to exit.

C signature:
    void thread_exit_ready(void);

Warning:
    Must be called by any Python thread before it exits to allow proper
    C/C++ thread lifecycle management.
"""
clib.thread_exit_ready.argtypes = []
clib.thread_exit_ready.restype = None

"""Checks if a termination signal has been received.

C signature:
    bool kill_requested(void);

Returns:
    bool: ``True`` if a kill request was detected, otherwise ``False``.
"""
clib.kill_requested.argtypes = []
clib.kill_requested.restype = ctypes.c_bool

"""Sends a raw image frame to the display buffer.

C signature:
    void send_frame(uint8_t* data, int width, int height);

Args:
    data (ctypes.POINTER(ctypes.c_ubyte)): Pointer to the raw image data.
    width (int): Frame width in pixels.
    height (int): Frame height in pixels.
"""
clib.send_frame.argtypes = [ctypes.POINTER(ctypes.c_ubyte),
                               ctypes.c_int, ctypes.c_int]
clib.send_frame.restype = None

"""Retrieves the latest captured camera frame.

C signature:
    uint8_t* get_latest_frame(size_t* out_size);

Args:
    out_size (ctypes.POINTER(ctypes.c_size_t)): Pointer to store frame size.

Returns:
    ctypes.POINTER(ctypes.c_ubyte): Pointer to a newly allocated frame buffer.

Warning:
    The caller must free the returned buffer with ``free_frame()``.
"""
clib.get_latest_frame.argtypes = [ctypes.POINTER(ctypes.c_size_t)]
clib.get_latest_frame.restype = ctypes.POINTER(ctypes.c_ubyte)

"""Frees memory allocated for a captured frame.

C signature:
    void free_frame(uint8_t* ptr);

Args:
    ptr (ctypes.POINTER(ctypes.c_ubyte)): Pointer to frame data obtained
        from ``get_latest_frame()``.

Warning:
    Must be called by any Python thread copying frames from the camera thread buffer.
"""
clib.free_frame.argtypes = [ctypes.POINTER(ctypes.c_ubyte)]
clib.free_frame.restype = None

"""Sends an absolute position to the motor control system.

C signature:
    void send_abs_pos(d_px_t x, d_px_t y);

Args:
    x (int): Target absolute X coordinate in pixels.
    y (int): Target absolute Y coordinate in pixels.
"""
clib.send_abs_pos.argtypes = [ctypes.c_int16, ctypes.c_int16]
clib.send_abs_pos.restype = None

"""Sends circular motion commands to the motor control system.

C signature:
    void circle_demo(d_px_t r, uint8_t n_pts, time_us_t delay);

Args:
    r (int): Radius of the circle in pixels.
    n_pts (int): Number of points used to approximate the circle.
    delay (int): Delay between movements in microseconds.
"""
clib.circle_demo.argtypes = [ctypes.c_int16, ctypes.c_ubyte, ctypes.c_uint32]
clib.circle_demo.restype = None

# Export for external use.
__all__ = ["clib"]
