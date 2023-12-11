from threading import Condition

import ArducamEvkSDK
import numpy as np

COLOR_BayerBG2BGR = 46
COLOR_BayerGB2BGR = 47
COLOR_BayerRG2BGR = 48
COLOR_BayerGR2BGR = 49


def convert_image(data: np.ndarray, cfg: ArducamEvkSDK.Format) -> np.ndarray:
    ...


def from_image(image: ArducamEvkSDK.Frame) -> np.ndarray:
    ...


def show_image(image: ArducamEvkSDK.Frame) -> None:
    ...


class WaitGroup(object):
    """
    WaitGroup is like Go sync.WaitGroup.

    Without all the useful corner cases.
    """
    __cv: Condition
    __count: int

    def __init__(self, init_count: int = 0):
        ...

    def add(self, n: int) -> None:
        ...

    def done(self) -> None:
        ...

    def wait(self) -> None:
        ...
