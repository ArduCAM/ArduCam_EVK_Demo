import threading

import ArducamEvkSDK
import cv2
import numpy as np
import arducam_rgbir_remosaic

COLOR_BayerBG2BGR = 46
COLOR_BayerGB2BGR = 47
COLOR_BayerRG2BGR = 48
COLOR_BayerGR2BGR = 49

rgbir_format = [
    arducam_rgbir_remosaic.BGGI,
    arducam_rgbir_remosaic.GIRG,
    arducam_rgbir_remosaic.RGGI,
    arducam_rgbir_remosaic.GRIG,
    arducam_rgbir_remosaic.IGGB,
    arducam_rgbir_remosaic.GBIG,
    arducam_rgbir_remosaic.GIBG,
]

def JPGToMat(data, datasize):
    image = np.frombuffer(data, np.uint8, count=datasize)
    return cv2.imdecode(image, cv2.IMREAD_ANYCOLOR)


def YUVToMat(data, Width, Height, color_mode):
    codeMap = {
        0: cv2.COLOR_YUV2BGR_YUYV,
        1: cv2.COLOR_YUV2BGR_YVYU,
        2: cv2.COLOR_YUV2BGR_UYVY,
        3: cv2.COLOR_YUV2BGR_Y422,
    }
    image = np.frombuffer(data, np.uint8).reshape(Height, Width, 2)
    return cv2.cvtColor(image, codeMap[color_mode])


def RGB565ToMat(data, Width, Height):
    arr = np.frombuffer(data, dtype=np.uint16).astype(np.uint32)
    arr = ((arr & 0xFF00) >> 8) + ((arr & 0x00FF) << 8)
    arr = 0xFF000000 + ((arr & 0xF800) << 8) + ((arr & 0x07E0) << 5) + ((arr & 0x001F) << 3)

    arr = np.frombuffer(arr, np.uint8)
    image = arr.reshape(Height, Width, 4)
    return cv2.flip(image, 0)


def dBytesToMat(data, bitWidth, Width, Height):
    arr = np.frombuffer(data, dtype=np.uint16)
    arr = (arr >> (bitWidth - 8))
    arr = arr.astype(np.uint8)
    image = arr.reshape(Height, Width, 1)
    return image


def separationImage(data, Width, Height):
    arr = np.frombuffer(data, dtype=np.uint16)
    arr1 = arr >> 8
    arr1 = arr1.astype(np.uint8)
    arr2 = arr.astype(np.uint8)

    image1 = arr1.reshape(Height, Width, 1)
    image2 = arr2.reshape(Height, Width, 1)

    image = np.concatenate([image1, image2], axis=1)
    return image


def convert_color(image, color_mode):
    if color_mode == 0:
        image = cv2.cvtColor(image, COLOR_BayerRG2BGR)
    if color_mode == 1:
        image = cv2.cvtColor(image, COLOR_BayerGR2BGR)
    if color_mode == 2:
        image = cv2.cvtColor(image, COLOR_BayerGB2BGR)
    if color_mode == 3:
        image = cv2.cvtColor(image, COLOR_BayerBG2BGR)
    if 0 > color_mode > 3:
        image = cv2.cvtColor(image, COLOR_BayerGB2BGR)
    return image

def RGBIRToMat(data, color_mode):
    bayer, ir = arducam_rgbir_remosaic.rgbir_remosaic(data, rgbir_format[color_mode])
    color = cv2.cvtColor(bayer, cv2.COLOR_BayerRG2BGRA)
    ir_color = cv2.cvtColor(ir, cv2.COLOR_GRAY2BGRA)
    ir_resize = cv2.resize(ir_color, (bayer.shape[1], bayer.shape[0]))
    image = cv2.hconcat([color, ir_resize])
    return image

def convert_image(data, cfg):
    Width = cfg.width
    Height = cfg.height
    bitWidth = cfg.bit_depth
    datasize = data.size
    color_mode = cfg.format_code & 0xff
    emImageFmtMode = cfg.format_code >> 8
    global COLOR_BayerGB2BGR, COLOR_BayerRG2BGR, COLOR_BayerGR2BGR, COLOR_BayerBG2BGR
    image = None

    if emImageFmtMode == ArducamEvkSDK.JPG:
        image = JPGToMat(data, datasize)
    if emImageFmtMode == ArducamEvkSDK.YUV:
        image = YUVToMat(data, Width, Height, color_mode)
    if emImageFmtMode == ArducamEvkSDK.RGB:
        image = RGB565ToMat(data, Width, Height)
    if emImageFmtMode == ArducamEvkSDK.MON:
        if 8 < bitWidth <= 2 * 8:
            image = dBytesToMat(data, bitWidth, Width, Height)
        else:
            image = np.frombuffer(data, np.uint8).reshape(Height, Width, 1)
    if emImageFmtMode == ArducamEvkSDK.RAW:
        if 8 < bitWidth <= 2 * 8:
            image = dBytesToMat(data, bitWidth, Width, Height)
        else:
            image = np.frombuffer(data, np.uint8).reshape(Height, Width, 1)
        image = convert_color(image, color_mode)
    if emImageFmtMode == ArducamEvkSDK.RAW_D:
        image = separationImage(data, Width, Height)
        image = convert_color(image, color_mode)
        pass
    if emImageFmtMode == ArducamEvkSDK.MON_D:
        image = separationImage(data, Width, Height)
        pass
    if emImageFmtMode == ArducamEvkSDK.RGB_IR:
        if bitWidth > 8:
            data = np.frombuffer(data, np.uint16).reshape(Height, Width)
            data = (data >> (bitWidth - 8)).astype(np.uint8)
        else:
            data = np.frombuffer(data, np.uint8).reshape(Height, Width)
        image = RGBIRToMat(data, color_mode)
        
    return image


def from_image(image):
    color_frame = convert_image(image.data, image.format)
    if color_frame is None:
        return None

    # if color_frame width > 720, resize it to 720
    if color_frame.shape[1] > 720:
        width = color_frame.shape[1]
        scale = 720.0 / width
        color_frame = cv2.resize(color_frame, None, fx=scale, fy=scale)

    return color_frame


def show_image(image):
    img = from_image(image)
    if img is not None:
        cv2.imshow("Test", img)
    cv2.setWindowTitle("Test", "Test " + str(image.seq))


class WaitGroup(object):
    """
    WaitGroup is like Go sync.WaitGroup.

    Without all the useful corner cases.
    """

    def __init__(self, init_count=0):
        self.__count = init_count
        self.__cv = threading.Condition()

    def add(self, n):
        self.__cv.acquire()
        self.__count += n
        self.__cv.release()

    def done(self):
        self.__cv.acquire()
        self.__count -= 1
        if self.__count == 0:
            self.__cv.notify_all()
        self.__cv.release()

    def wait(self):
        self.__cv.acquire()
        while self.__count > 0:
            self.__cv.wait()
        self.__cv.release()
