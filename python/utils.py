import threading

import cv2
import numpy as np
from ArducamEvkSDK import Frame

cvt_code_map = {
    0: cv2.COLOR_BayerRG2BGR, 1: cv2.COLOR_BayerGR2BGR, 2: cv2.COLOR_BayerGB2BGR,
    3: cv2.COLOR_BayerBG2BGR, 4: cv2.COLOR_GRAY2BGR,
}


def from_image(image: Frame):
    if image.format.bit_depth == 8:
        frame = image.data
    else:
        frame = image.data
        frame.dtype = np.uint16
        l_shift = 16 - image.format.bit_depth
        frame <<= l_shift

    color_frame = cv2.cvtColor(frame, cvt_code_map[image.format.format_code & 0xff])

    # if color_frame width > 720, resize it to 720
    if color_frame.shape[1] > 720:
        width = color_frame.shape[1]
        scale = 720.0 / width
        color_frame = cv2.resize(color_frame, None, fx=scale, fy=scale)

    return color_frame


def show_image(image: Frame):
    cv2.imshow("Test", from_image(image))
    cv2.setWindowTitle("Test", "Test " + str(image.seq))


class WaitGroup(object):
    """WaitGroup is like Go sync.WaitGroup.

    Without all the useful corner cases.
    """

    def __init__(self, init_count: int = 0):
        self.count = init_count
        self.cv = threading.Condition()

    def add(self, n):
        self.cv.acquire()
        self.count += n
        self.cv.release()

    def done(self):
        self.cv.acquire()
        self.count -= 1
        if self.count == 0:
            self.cv.notify_all()
        self.cv.release()

    def wait(self):
        self.cv.acquire()
        while self.count > 0:
            self.cv.wait()
        self.cv.release()
