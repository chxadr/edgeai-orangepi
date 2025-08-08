import ctypes

clib = ctypes.CDLL("./lib/libc_interface.so")

# int init_board(void) signature
clib.init_board.argtypes = []
clib.init_board.restype = ctypes.c_int

# int spawn_threads(void) signature
clib.spawn_threads.argtypes = []
clib.spawn_threads.restype = ctypes.c_int

# void join_threads(void) signature
clib.join_threads.argtypes = []
clib.join_threads.restype = None

# void exit_clean(void) signature
clib.exit_clean.argtypes = []
clib.exit_clean.restype = None

# void thread_exit_ready(void) signature
clib.thread_exit_ready.argtypes = []
clib.thread_exit_ready.restype = None

# bool kill_requested(void) signature
clib.kill_requested.argtypes = []
clib.kill_requested.restype = ctypes.c_bool

# void send_frame(uint8_t* data, int width, int height) signature
clib.send_frame.argtypes = [ctypes.POINTER(ctypes.c_ubyte),
                               ctypes.c_int, ctypes.c_int]
clib.send_frame.restype = None

# uint8_t* get_latest_frame(size_t* out_size) signature
clib.get_latest_frame.argtypes = [ctypes.POINTER(ctypes.c_size_t)]
clib.get_latest_frame.restype = ctypes.POINTER(ctypes.c_ubyte)

# void free_frame(uint8_t* ptr) signature
clib.free_frame.argtypes = [ctypes.POINTER(ctypes.c_ubyte)]
clib.free_frame.restype = None

# void send_abs_pos(d_px_t x, d_px_t y) signature
clib.send_abs_pos.argtypes = [ctypes.c_int16, ctypes.c_int16]
clib.send_abs_pos.restype = None

# void circle_demo(d_px_t r, uint8_t n_pts, time_us_t delay)
clib.circle_demo.argtypes = [ctypes.c_int16, ctypes.c_ubyte, ctypes.c_uint32]
clib.circle_demo.restype = None

# Export for external use
__all__ = ["clib"]
