import os, ctypes
import numpy as np
from ctypes import c_int8, c_uint8, c_int16, c_uint16, c_int32, c_uint32, c_int64, c_uint64
from ctypes import c_char_p, c_void_p, c_bool, c_float, c_double, POINTER, byref
try:
    from ctypes import WinDLL
except ImportError:
    from ctypes import CDLL

dtype2ctype = {
    np.dtype('bool'): ctypes.c_bool,
    np.dtype('int8'): ctypes.c_int8, np.dtype('uint8'): ctypes.c_uint8,
    np.dtype('int16'): ctypes.c_int16, np.dtype('uint16'): ctypes.c_uint16,
    np.dtype('int32'): ctypes.c_int32, np.dtype('uint32'): ctypes.c_uint32,
    np.dtype('int64'): ctypes.c_int64, np.dtype('uint64'): ctypes.c_uint64,
    np.dtype('float32'): ctypes.c_float, np.dtype('float64'): ctypes.c_double,
}


def ez_load_dynamic_library(path, name):
    if os.name == 'nt':
        import win32api, win32con
        dll_path = os.path.join(path, name + '.dll')
        return WinDLL(dll_path, handle=win32api.LoadLibraryEx(dll_path, 0, win32con.LOAD_WITH_ALTERED_SEARCH_PATH))
    else:
        return CDLL(os.path.join(path, name + '.so'))


def ct_array_ptr(ndarray, forced_ctype=None):
    if ndarray is None:
        return None
    elif not ndarray.flags['C_CONTIGUOUS']:
        raise ValueError("ndarray must be C_CONTIGUOUS, else shit's gonna happen.")
    elif forced_ctype:
        return ndarray.ctypes.data_as(ctypes.POINTER(forced_ctype))
    else:
        return ndarray.ctypes.data_as(ctypes.POINTER(dtype2ctype[ndarray.dtype]))


