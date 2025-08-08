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

# bool sigint_received(void) signature
clib.sigint_received.argtypes = []
clib.sigint_received.restype = ctypes.c_bool

# void display_frame(uint8_t* data, int width, int height, int channels) signature
clib.display_frame.argtypes = [ctypes.POINTER(ctypes.c_ubyte),
                               ctypes.c_int, ctypes.c_int, ctypes.c_int]
clib.display_frame.restype = None

# uint8_t* get_latest_frame(size_t* out_size) signature
clib.get_latest_frame.argtypes = [ctypes.POINTER(ctypes.c_size_t)]
clib.get_latest_frame.restype = ctypes.POINTER(ctypes.c_ubyte)

# void free_frame(uint8_t* ptr) signature
clib.free_frame.argtypes = [ctypes.POINTER(ctypes.c_ubyte)]
clib.free_frame.restype = None

# void send_abs_pos(d_px_t dx, d_px_t dy) signature
clib.send_abs_pos.argtypes = [ctypes.c_int16, ctypes.c_int16]
clib.send_abs_pos.restype = None

# void circle(d_px_t r, uint8_t n_pts, time_us_t delay_us)
clib.circle_demo.argtypes = [ctypes.c_int16, ctypes.c_ubyte, ctypes.c_uint32]
clib.circle_demo.restype = None

# Export for external use
__all__ = ["clib"]
