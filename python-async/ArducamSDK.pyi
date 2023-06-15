#
# Automatically generated file, do not edit!
#

"""
        ArducamSDK
    """
from __future__ import annotations
import ArducamSDK
import typing

__all__ = [
    "Camera",
    "Control",
    "Device",
    "EVENT_EXIT",
    "EVENT_FRAME_END",
    "EVENT_FRAME_START",
    "EVENT_NONE",
    "EventCode",
    "Format",
    "Frame",
    "I2CMode",
    "I2C_MODE_16_16",
    "I2C_MODE_16_32",
    "I2C_MODE_16_8",
    "I2C_MODE_8_16",
    "I2C_MODE_8_8",
    "LOG_CRITICAL",
    "LOG_DEBUG",
    "LOG_ERR",
    "LOG_INFO",
    "LOG_OFF",
    "LOG_TRACE",
    "LOG_WARN",
    "LoggerLevel",
    "MEM_TYPE_DMA",
    "MEM_TYPE_RAM",
    "MemType",
    "Param",
    "USBSpeed",
    "USB_SPEED_FULL",
    "USB_SPEED_HIGH",
    "USB_SPEED_LOW",
    "USB_SPEED_SUPER",
    "USB_SPEED_SUPER_PLUS",
    "USB_SPEED_UNKNOWN",
    "list_devices"
]


class Camera():
    def __init__(self) -> None: ...
    def close(self) -> int: ...
    def enable_console_log(self, enable: bool = True) -> int: ...
    def init(self) -> int: ...
    def open(self, param: Param) -> int: ...
    def read(self, timeout: int = 2000) -> typing.Optional[Frame]: ...
    def read_reg_16_16(self, ship_addr: int, reg_addr: int) -> typing.Optional[int]: ...
    def read_reg_16_8(self, ship_addr: int, reg_addr: int) -> typing.Optional[int]: ...
    def read_reg_8_16(self, ship_addr: int, reg_addr: int) -> typing.Optional[int]: ...
    def read_reg_8_8(self, ship_addr: int, reg_addr: int) -> typing.Optional[int]: ...
    def read_sensor_reg(self, controlId: int) -> typing.Optional[int]: ...
    def register_control(self, controls: typing.List[Control]) -> int: ...
    @typing.overload
    def reload(self) -> int: ...
    @typing.overload
    def reload(self, mode_id: int) -> int: ...
    def send_vr(self, command: int, direction: int, value: int, index: int, buf: typing.List[int]) -> int: ...
    def set_control(self, controlId: str, value: int) -> int: ...
    def set_error_callback(self, arg0: typing.Optional[typing.Callable[[str], None]]) -> int: ...
    def set_event_callback(self, arg0: typing.Optional[typing.Callable[[EventCode], None]]) -> int: ...
    def set_read_callback(self, arg0: typing.Optional[typing.Callable[[Frame], None]]) -> int: ...
    def start(self) -> int: ...
    def stop(self) -> int: ...
    def write_reg_16_16(self, ship_addr: int, reg_addr: int, value: int) -> int: ...
    def write_reg_16_8(self, ship_addr: int, reg_addr: int, value: int) -> int: ...
    def write_reg_8_16(self, ship_addr: int, reg_addr: int, value: int) -> int: ...
    def write_reg_8_8(self, ship_addr: int, reg_addr: int, value: int) -> int: ...
    def write_sensor_reg(self, controlId: int, value: int) -> int: ...
    @property
    def bandwidth(self) -> int:
        """
        :type: int
        """
    @bandwidth.setter
    def bandwidth(self) -> None:
        pass
    @property
    def bin_config(self) -> dict:
        """
        :type: dict
        """
    @bin_config.setter
    def bin_config(self) -> None:
        pass
    @property
    def bin_config_loaded(self) -> bool:
        """
        :type: bool
        """
    @bin_config_loaded.setter
    def bin_config_loaded(self) -> None:
        pass
    @property
    def bit_width(self) -> int:
        """
        :type: int
        """
    @bit_width.setter
    def bit_width(self, arg1: int) -> None:
        pass
    @property
    def capture_fps(self) -> int:
        """
        :type: int
        """
    @capture_fps.setter
    def capture_fps(self) -> None:
        pass
    @property
    def config_loaded(self) -> bool:
        """
        :type: bool
        """
    @config_loaded.setter
    def config_loaded(self) -> None:
        pass
    @property
    def format(self) -> int:
        """
        :type: int
        """
    @format.setter
    def format(self, arg1: int) -> None:
        pass
    @property
    def height(self) -> int:
        """
        :type: int
        """
    @height.setter
    def height(self, arg1: int) -> None:
        pass
    @property
    def i2c_addr(self) -> int:
        """
        :type: int
        """
    @i2c_addr.setter
    def i2c_addr(self, arg1: int) -> None:
        pass
    @property
    def i2c_mode(self) -> int:
        """
        :type: int
        """
    @i2c_mode.setter
    def i2c_mode(self, arg1: int) -> None:
        pass
    @property
    def log_level(self) -> typing.Optional[LoggerLevel]:
        """
        :type: typing.Optional[LoggerLevel]
        """
    @log_level.setter
    def log_level(self, arg1: LoggerLevel) -> None:
        pass
    @property
    def usb_type(self) -> str:
        """
        :type: str
        """
    @usb_type.setter
    def usb_type(self) -> None:
        pass
    @property
    def width(self) -> int:
        """
        :type: int
        """
    @width.setter
    def width(self, arg1: int) -> None:
        pass
    pass
class Control():
    def __init__(self) -> None: ...
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    @property
    def code(self) -> str:
        """
        :type: str
        """
    @code.setter
    def code(self, arg0: str) -> None:
        pass
    @property
    def default(self) -> int:
        """
        :type: int
        """
    @default.setter
    def default(self, arg0: int) -> None:
        pass
    @property
    def flags(self) -> int:
        """
        :type: int
        """
    @flags.setter
    def flags(self, arg0: int) -> None:
        pass
    @property
    def func(self) -> str:
        """
        :type: str
        """
    @func.setter
    def func(self) -> None:
        pass
    @property
    def max(self) -> int:
        """
        :type: int
        """
    @max.setter
    def max(self, arg0: int) -> None:
        pass
    @property
    def min(self) -> int:
        """
        :type: int
        """
    @min.setter
    def min(self, arg0: int) -> None:
        pass
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @name.setter
    def name(self) -> None:
        pass
    @property
    def step(self) -> int:
        """
        :type: int
        """
    @step.setter
    def step(self, arg0: int) -> None:
        pass
    pass
class Device():
    @property
    def id_product(self) -> int:
        """
        :type: int
        """
    @property
    def id_vendor(self) -> int:
        """
        :type: int
        """
    @property
    def in_used(self) -> bool:
        """
        :type: bool
        """
    @property
    def serial_number(self) -> typing.List[int]:
        """
        :type: typing.List[int]
        """
    @property
    def speed(self) -> USBSpeed:
        """
        :type: USBSpeed
        """
    @property
    def usb_type(self) -> int:
        """
        :type: int
        """
    pass
class EventCode():
    """
    Members:

      EVENT_NONE

      EVENT_FRAME_START

      EVENT_FRAME_END

      EVENT_EXIT
    """
    def __eq__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    EVENT_EXIT: ArducamSDK.EventCode # value = <EventCode.EVENT_EXIT: 3>
    EVENT_FRAME_END: ArducamSDK.EventCode # value = <EventCode.EVENT_FRAME_END: 2>
    EVENT_FRAME_START: ArducamSDK.EventCode # value = <EventCode.EVENT_FRAME_START: 1>
    EVENT_NONE: ArducamSDK.EventCode # value = <EventCode.EVENT_NONE: 0>
    __members__: dict # value = {'EVENT_NONE': <EventCode.EVENT_NONE: 0>, 'EVENT_FRAME_START': <EventCode.EVENT_FRAME_START: 1>, 'EVENT_FRAME_END': <EventCode.EVENT_FRAME_END: 2>, 'EVENT_EXIT': <EventCode.EVENT_EXIT: 3>}
    pass
class Format():
    def __init__(self) -> None: ...
    @property
    def bit_depth(self) -> int:
        """
        :type: int
        """
    @bit_depth.setter
    def bit_depth(self, arg0: int) -> None:
        pass
    @property
    def format_code(self) -> int:
        """
        :type: int
        """
    @format_code.setter
    def format_code(self, arg0: int) -> None:
        pass
    @property
    def height(self) -> int:
        """
        :type: int
        """
    @height.setter
    def height(self, arg0: int) -> None:
        pass
    @property
    def width(self) -> int:
        """
        :type: int
        """
    @width.setter
    def width(self, arg0: int) -> None:
        pass
    pass
class Frame():
    def __init__(self) -> None: ...
    @property
    def bad(self) -> bool:
        """
        :type: bool
        """
    @bad.setter
    def bad(self, arg0: bool) -> None:
        pass
    @property
    def data(self) -> numpy.ndarray[numpy.uint8]:
        """
        :type: numpy.ndarray[numpy.uint8]
        """
    @data.setter
    def data(self, arg0: numpy.ndarray[numpy.uint8]) -> None:
        pass
    @property
    def format(self) -> Format:
        """
        :type: Format
        """
    @format.setter
    def format(self, arg0: Format) -> None:
        pass
    @property
    def seq(self) -> int:
        """
        :type: int
        """
    @seq.setter
    def seq(self, arg0: int) -> None:
        pass
    pass
class I2CMode():
    """
    Members:

      I2C_MODE_8_8

      I2C_MODE_8_16

      I2C_MODE_16_8

      I2C_MODE_16_16

      I2C_MODE_16_32
    """
    def __eq__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    I2C_MODE_16_16: ArducamSDK.I2CMode # value = <I2CMode.I2C_MODE_16_16: 3>
    I2C_MODE_16_32: ArducamSDK.I2CMode # value = <I2CMode.I2C_MODE_16_32: 4>
    I2C_MODE_16_8: ArducamSDK.I2CMode # value = <I2CMode.I2C_MODE_16_8: 2>
    I2C_MODE_8_16: ArducamSDK.I2CMode # value = <I2CMode.I2C_MODE_8_16: 1>
    I2C_MODE_8_8: ArducamSDK.I2CMode # value = <I2CMode.I2C_MODE_8_8: 0>
    __members__: dict # value = {'I2C_MODE_8_8': <I2CMode.I2C_MODE_8_8: 0>, 'I2C_MODE_8_16': <I2CMode.I2C_MODE_8_16: 1>, 'I2C_MODE_16_8': <I2CMode.I2C_MODE_16_8: 2>, 'I2C_MODE_16_16': <I2CMode.I2C_MODE_16_16: 3>, 'I2C_MODE_16_32': <I2CMode.I2C_MODE_16_32: 4>}
    pass
class LoggerLevel():
    """
    Members:

      LOG_TRACE

      LOG_DEBUG

      LOG_INFO

      LOG_WARN

      LOG_ERR

      LOG_CRITICAL

      LOG_OFF
    """
    def __eq__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    LOG_CRITICAL: ArducamSDK.LoggerLevel # value = <LoggerLevel.LOG_CRITICAL: 5>
    LOG_DEBUG: ArducamSDK.LoggerLevel # value = <LoggerLevel.LOG_DEBUG: 1>
    LOG_ERR: ArducamSDK.LoggerLevel # value = <LoggerLevel.LOG_ERR: 4>
    LOG_INFO: ArducamSDK.LoggerLevel # value = <LoggerLevel.LOG_INFO: 2>
    LOG_OFF: ArducamSDK.LoggerLevel # value = <LoggerLevel.LOG_OFF: 6>
    LOG_TRACE: ArducamSDK.LoggerLevel # value = <LoggerLevel.LOG_TRACE: 0>
    LOG_WARN: ArducamSDK.LoggerLevel # value = <LoggerLevel.LOG_WARN: 3>
    __members__: dict # value = {'LOG_TRACE': <LoggerLevel.LOG_TRACE: 0>, 'LOG_DEBUG': <LoggerLevel.LOG_DEBUG: 1>, 'LOG_INFO': <LoggerLevel.LOG_INFO: 2>, 'LOG_WARN': <LoggerLevel.LOG_WARN: 3>, 'LOG_ERR': <LoggerLevel.LOG_ERR: 4>, 'LOG_CRITICAL': <LoggerLevel.LOG_CRITICAL: 5>, 'LOG_OFF': <LoggerLevel.LOG_OFF: 6>}
    pass
class MemType():
    """
    Members:

      MEM_TYPE_DMA

      MEM_TYPE_RAM
    """
    def __eq__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    MEM_TYPE_DMA: ArducamSDK.MemType # value = <MemType.MEM_TYPE_DMA: 1>
    MEM_TYPE_RAM: ArducamSDK.MemType # value = <MemType.MEM_TYPE_RAM: 2>
    __members__: dict # value = {'MEM_TYPE_DMA': <MemType.MEM_TYPE_DMA: 1>, 'MEM_TYPE_RAM': <MemType.MEM_TYPE_RAM: 2>}
    pass
class Param():
    def __init__(self) -> None: 
        """
        construct a default param
        """
    @property
    def bin_config(self) -> bool:
        """
        :type: bool
        """
    @bin_config.setter
    def bin_config(self, arg0: bool) -> None:
        pass
    @property
    def config_file_name(self) -> str:
        """
        :type: str
        """
    @config_file_name.setter
    def config_file_name(self, arg0: str) -> None:
        pass
    @property
    def device(self) -> PyDevice:
        """
        :type: PyDevice
        """
    @device.setter
    def device(self, arg0: PyDevice) -> None:
        pass
    @property
    def mem_type(self) -> MemType:
        """
        :type: MemType
        """
    @mem_type.setter
    def mem_type(self, arg0: MemType) -> None:
        pass
    pass
class USBSpeed():
    """
    Members:

      USB_SPEED_UNKNOWN

      USB_SPEED_LOW

      USB_SPEED_FULL

      USB_SPEED_HIGH

      USB_SPEED_SUPER

      USB_SPEED_SUPER_PLUS
    """
    def __eq__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    USB_SPEED_FULL: ArducamSDK.USBSpeed # value = <USBSpeed.USB_SPEED_FULL: 2>
    USB_SPEED_HIGH: ArducamSDK.USBSpeed # value = <USBSpeed.USB_SPEED_HIGH: 3>
    USB_SPEED_LOW: ArducamSDK.USBSpeed # value = <USBSpeed.USB_SPEED_LOW: 1>
    USB_SPEED_SUPER: ArducamSDK.USBSpeed # value = <USBSpeed.USB_SPEED_SUPER: 4>
    USB_SPEED_SUPER_PLUS: ArducamSDK.USBSpeed # value = <USBSpeed.USB_SPEED_SUPER_PLUS: 5>
    USB_SPEED_UNKNOWN: ArducamSDK.USBSpeed # value = <USBSpeed.USB_SPEED_UNKNOWN: 0>
    __members__: dict # value = {'USB_SPEED_UNKNOWN': <USBSpeed.USB_SPEED_UNKNOWN: 0>, 'USB_SPEED_LOW': <USBSpeed.USB_SPEED_LOW: 1>, 'USB_SPEED_FULL': <USBSpeed.USB_SPEED_FULL: 2>, 'USB_SPEED_HIGH': <USBSpeed.USB_SPEED_HIGH: 3>, 'USB_SPEED_SUPER': <USBSpeed.USB_SPEED_SUPER: 4>, 'USB_SPEED_SUPER_PLUS': <USBSpeed.USB_SPEED_SUPER_PLUS: 5>}
    pass
def list_devices() -> typing.List[Device]:
    pass
EVENT_EXIT: ArducamSDK.EventCode # value = <EventCode.EVENT_EXIT: 3>
EVENT_FRAME_END: ArducamSDK.EventCode # value = <EventCode.EVENT_FRAME_END: 2>
EVENT_FRAME_START: ArducamSDK.EventCode # value = <EventCode.EVENT_FRAME_START: 1>
EVENT_NONE: ArducamSDK.EventCode # value = <EventCode.EVENT_NONE: 0>
I2C_MODE_16_16: ArducamSDK.I2CMode # value = <I2CMode.I2C_MODE_16_16: 3>
I2C_MODE_16_32: ArducamSDK.I2CMode # value = <I2CMode.I2C_MODE_16_32: 4>
I2C_MODE_16_8: ArducamSDK.I2CMode # value = <I2CMode.I2C_MODE_16_8: 2>
I2C_MODE_8_16: ArducamSDK.I2CMode # value = <I2CMode.I2C_MODE_8_16: 1>
I2C_MODE_8_8: ArducamSDK.I2CMode # value = <I2CMode.I2C_MODE_8_8: 0>
LOG_CRITICAL: ArducamSDK.LoggerLevel # value = <LoggerLevel.LOG_CRITICAL: 5>
LOG_DEBUG: ArducamSDK.LoggerLevel # value = <LoggerLevel.LOG_DEBUG: 1>
LOG_ERR: ArducamSDK.LoggerLevel # value = <LoggerLevel.LOG_ERR: 4>
LOG_INFO: ArducamSDK.LoggerLevel # value = <LoggerLevel.LOG_INFO: 2>
LOG_OFF: ArducamSDK.LoggerLevel # value = <LoggerLevel.LOG_OFF: 6>
LOG_TRACE: ArducamSDK.LoggerLevel # value = <LoggerLevel.LOG_TRACE: 0>
LOG_WARN: ArducamSDK.LoggerLevel # value = <LoggerLevel.LOG_WARN: 3>
MEM_TYPE_DMA: ArducamSDK.MemType # value = <MemType.MEM_TYPE_DMA: 1>
MEM_TYPE_RAM: ArducamSDK.MemType # value = <MemType.MEM_TYPE_RAM: 2>
USB_SPEED_FULL: ArducamSDK.USBSpeed # value = <USBSpeed.USB_SPEED_FULL: 2>
USB_SPEED_HIGH: ArducamSDK.USBSpeed # value = <USBSpeed.USB_SPEED_HIGH: 3>
USB_SPEED_LOW: ArducamSDK.USBSpeed # value = <USBSpeed.USB_SPEED_LOW: 1>
USB_SPEED_SUPER: ArducamSDK.USBSpeed # value = <USBSpeed.USB_SPEED_SUPER: 4>
USB_SPEED_SUPER_PLUS: ArducamSDK.USBSpeed # value = <USBSpeed.USB_SPEED_SUPER_PLUS: 5>
USB_SPEED_UNKNOWN: ArducamSDK.USBSpeed # value = <USBSpeed.USB_SPEED_UNKNOWN: 0>
__version__ = 'dev'
