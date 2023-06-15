import cv2
import numpy as np
from ArducamSDK import Frame

cvt_code_map = {
    0: cv2.COLOR_BayerRG2BGR, 1: cv2.COLOR_BayerGR2BGR, 2: cv2.COLOR_BayerGB2BGR,
    3: cv2.COLOR_BayerBG2BGR, 4: cv2.COLOR_GRAY2BGR,
}


def from_buffer(fb: Frame):
    if fb.format.bit_depth == 8:
        frame = fb.data
    else:
        frame = fb.data
        frame.dtype = np.uint16
        l_shift = 16 - fb.format.bit_depth
        frame <<= l_shift

    color_frame = cv2.cvtColor(frame, cvt_code_map[fb.format.format_code & 0xff])

    # if color_frame width > 720, resize it to 720
    if color_frame.shape[1] > 720:
        width = color_frame.shape[1]
        scale = 720.0 / width
        color_frame = cv2.resize(color_frame, None, fx=scale, fy=scale)

    return color_frame


def show_buffer(fb: Frame):
    cv2.imshow("Test", from_buffer(fb))
    cv2.setWindowTitle("Test", "Test " + str(fb.seq))
