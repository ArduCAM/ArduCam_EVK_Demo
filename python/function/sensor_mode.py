import abc
import copy
from gzip import compress as gzip_encode, decompress as gzip_decode
from struct import *
from typing import List, Type, TypeVar, Callable, Union

import json5
from pycrc.algorithms import Crc
from pycrc.models import CrcModels
from pydantic.types import conlist
# from pydantic import BaseModel
from pydantic_yaml import YamlModel as BaseModel, YamlIntEnum as YamlEnum


# Enum definitions

class BayerOrderEnum(YamlEnum):
    BGGR = 0
    GBRG = 1
    GRBG = 2
    RGGB = 3
    GRAY = 4


class YUVOrderEnum(YamlEnum):
    YUYV = 0
    YVYU = 1
    UYVY = 2
    VYUY = 3


class PixFormatTypeEnum(YamlEnum):
    RAW8 = 0x2A
    RAW10 = 0x2B
    RAW12 = 0x2C
    YUV420_8BIT = 0x18
    YUV420_10BIT = 0x19
    YUV422_8BIT = 0x1E


class I2CModeEnum(YamlEnum):
    I2C_MODE_8_8 = 0x11
    I2C_MODE_8_16 = 0x12
    I2C_MODE_16_8 = 0x21
    I2C_MODE_16_16 = 0x22
    I2C_MODE_16_32 = 0x24


class MIPILanesEnum(YamlEnum):
    LANES_1 = 1
    LANES_2 = 2
    LANES_4 = 4


class ParameterTypeEnum(YamlEnum):
    SensorReg = 0
    Delay = 1
    VendorCommand = 2


class CodeTypeEnum(YamlEnum):
    LUA = 0
    C = 1


class HeaderFlagEnum(YamlEnum):
    FLAG_NONE = 0x01
    FLAG_CRC = 0x02
    FLAG_ENCRYPT_DATA = 0x04
    FLAG_COMPRESS_DATA = 0x08
    FLAG_READ_ONLY = 0x10
    FLAG_LITTLE_ENDIAN = 0x20
    FLAG_BIG_ENDIAN = 0x40
    FLAG_DEBUG = 0x80
    FLAG_ENCRYPT_COMMENTS = 0x100
    FLAG_COMPRESS_COMMENTS = 0x200

    FLAG_DEFAULT = FLAG_NONE | FLAG_LITTLE_ENDIAN
    FLAG_RO_DEFAULT = FLAG_READ_ONLY | FLAG_DEFAULT


# utils definitions

class Serializable(abc.ABC):
    @classmethod
    @abc.abstractmethod
    def __serialize__(cls, meta: dict):
        """
        get the struct format string.
        :param meta: metadata.
        :return: struct format string.
        """
        raise NotImplementedError

    @abc.abstractmethod
    def __save__(self, meta: dict):
        """
        fmt = self.__serialize__(meta)
        return pack(fmt, *self.__dict__.values())
        """
        raise NotImplementedError

    @classmethod
    @abc.abstractmethod
    def __load__(cls, meta: dict, data: bytes, offset: int):
        """
        fmt = self.__serialize__(meta)
        tuples = unpack(fmt, data)
        ...
        """
        raise NotImplementedError


T = TypeVar("T", bound=Serializable)


def save(func: Callable[[Type[T], str, dict], bytes]):
    def wrapper(cls: Type[T], meta: dict):
        fmt: str = cls.__serialize__(meta)
        data: bytes = func(cls, fmt, meta)
        meta["data"].extend(data)

    return wrapper


def load(func: Callable[[Type[T], tuple], T]):
    def wrapper(cls: Type[T], meta: dict, data: bytes, offset: int):
        fmt: str = cls.__serialize__(meta)
        temp: tuple = unpack_from(fmt, data, offset)
        return func(cls, temp)

    return wrapper


class Char(int, Serializable):
    @classmethod
    def __serialize__(cls, meta: dict):
        return "B"

    @save
    def __save__(self, fmt: str, meta: dict):
        return pack(fmt, self)

    @classmethod
    @load
    def __load__(cls, data: tuple):
        return cls(data[0])


class Array(Serializable):
    def __init__(self, data: List[Serializable]):
        self.data = data

    @classmethod
    def __serialize__(cls, meta: dict):
        return "II"

    def __save__(self, meta: dict):
        if "payload" not in meta:
            temp = dict(meta)
            temp.update({
                "addr": [],  # List[List[int]]
                "data": bytearray(b""),  # bytearray
            })
            meta["payload"] = copy.deepcopy(temp)

        meta["data"].extend(pack("<I", len(self.data)))  # size
        payload = meta["payload"]
        meta["addr"].append([len(meta["data"]), len(payload["data"])])
        meta["data"].extend(pack("<I", 0xcdcdcdcd))  # placeholder(pointer)
        for i in self.data:
            i.__save__(payload)

    @classmethod
    def __load__(cls, meta: dict, data: bytes, offset: int, item_type: Type[T] = None):
        if item_type is None:
            raise ValueError("item_type must be specified")
        fmt = "<II"
        size, ptr = unpack_from(fmt, data, offset)
        result = []
        for i in range(size):
            result.append(item_type.__load__(meta, data, ptr))
            ptr += calcsize(item_type.__serialize__(meta))
            # if item_type has _item_size attribute, use it
            if hasattr(item_type, "item_size"):
                ptr += item_type.item_size
                # delete _item_size attribute
                delattr(item_type, "item_size")
        return result


# class definitions

class SensorReg(BaseModel, Serializable, json_loads=json5.loads, json_dumps=json5.dumps):
    addr: int  # 1-2B
    data: int  # 1-2B

    @classmethod
    def __serialize__(cls, meta: dict):
        assert "i2c_mode" in meta and type(meta["i2c_mode"]) == I2CModeEnum
        if "BigEndian" in meta and meta["BigEndian"]:
            format_ = ">"
        else:
            format_ = "<"
        mode: I2CModeEnum = meta["i2c_mode"]
        if mode == I2CModeEnum.I2C_MODE_8_8:
            format_ += "BB"
        elif mode == I2CModeEnum.I2C_MODE_8_16:
            format_ += "BH"
        elif mode == I2CModeEnum.I2C_MODE_16_8:
            format_ += "HB"
        elif mode == I2CModeEnum.I2C_MODE_16_16:
            format_ += "HH"
        return format_

    @save
    def __save__(self, fmt: str, meta: dict):
        return pack(fmt, self.addr, self.data)

    @classmethod
    @load
    def __load__(cls, data: tuple):
        return cls(addr=data[0], data=data[1])

    def __str__(self):
        return f"{{addr: 0x{self.addr:04X}, data: 0x{self.data:04X}}}"

    def __repr__(self):
        return self.__str__()


class Delay(BaseModel, Serializable, json_loads=json5.loads, json_dumps=json5.dumps):
    delay: int  # 4B

    @classmethod
    def __serialize__(cls, meta: dict):
        return "<I"

    @save
    def __save__(self, fmt: str, meta: dict):
        return pack(fmt, self.delay)

    @classmethod
    @load
    def __load__(cls, data: tuple):
        return cls(delay=data[0])

    def __str__(self):
        return f"{{delay: {self.delay}}}"

    def __repr__(self):
        return self.__str__()


class VendorCommand(BaseModel, Serializable, json_loads=json5.loads, json_dumps=json5.dumps):
    cmd: int  # 1B
    value: int  # 2B
    index: int  # 2B
    data: List[int]  # 8*1B

    @classmethod
    def __serialize__(cls, meta: dict):
        return "<BHHI" + "B" * 8

    @save
    def __save__(self, fmt: str, meta: dict):
        return pack(fmt, self.cmd, self.value, self.index, len(self.data), *self.data,
                    *[0 for _ in range(8 - len(self.data))])

    @classmethod
    @load
    def __load__(cls, data: tuple):
        cmd = data[0]
        value = data[1]
        index = data[2]
        data = list(data[4:4 + data[3]])
        return cls(cmd=cmd, value=value, index=index, data=data)

    def __str__(self):
        return f"{{cmd: 0x{self.cmd:02X}, value: 0x{self.value:04X}, index: 0x{self.index:04X}, data: {self.data}}}"

    def __repr__(self):
        return self.__str__()


class Parameter(BaseModel, Serializable, json_loads=json5.loads, json_dumps=json5.dumps):
    type: ParameterTypeEnum  # 1B
    data: Union[SensorReg, Delay, VendorCommand]  # SensorReg | Delay | VendorCommand  # 2-13B

    @classmethod
    def __serialize__(cls, meta: dict):
        return "<B"

    def __save__(self, meta: dict):
        fmt: str = self.__serialize__(meta)
        meta["data"].extend(pack(fmt, self.type.value))
        data: Serializable = self.data
        data.__save__(meta)

    @classmethod
    def __load__(cls, meta: dict, data: bytes, offset: int):
        fmt = cls.__serialize__(meta)
        type_ = ParameterTypeEnum(unpack_from(fmt, data, offset)[0])
        if type_ == ParameterTypeEnum.SensorReg:
            t: Type[Serializable] = SensorReg
            cls.item_size = calcsize(t.__serialize__(meta))
            return Parameter(type=ParameterTypeEnum.SensorReg,
                             data=t.__load__(meta, data, offset + 1))
        elif type_ == ParameterTypeEnum.Delay:
            t: Type[Serializable] = Delay
            cls.item_size = calcsize(t.__serialize__(meta))
            return Parameter(type=ParameterTypeEnum.Delay,
                             data=t.__load__(meta, data, offset + 1))
        elif type_ == ParameterTypeEnum.VendorCommand:
            t: Type[Serializable] = VendorCommand
            cls.item_size = calcsize(t.__serialize__(meta))
            return Parameter(type=ParameterTypeEnum.VendorCommand,
                             data=t.__load__(meta, data, offset + 1))
        else:
            raise ValueError(f"unknown parameter type: {type_}")

    @classmethod
    def SensorReg(cls, addr: int, data: int):
        return cls(type=ParameterTypeEnum.SensorReg, data=SensorReg(addr=addr, data=data))

    @classmethod
    def Delay(cls, delay: int):
        return cls(type=ParameterTypeEnum.Delay, data=Delay(delay=delay))

    @classmethod
    def VendorCommand(cls, cmd: int, value: int, index: int, data: List[int]):
        return cls(type=ParameterTypeEnum.VendorCommand,
                   data=VendorCommand(cmd=cmd, value=value, index=index, data=data))


class Control(BaseModel, Serializable, json_loads=json5.loads, json_dumps=json5.dumps):
    min_: int  # 8B
    max_: int  # 8B
    def_: int  # 8B
    step: int  # 4B
    flags: int  # 4B
    name: str  # 128B
    func: str  # 128B
    code_type: CodeTypeEnum  # 4B
    code: str  # 4B, 4B

    @classmethod
    def __serialize__(cls, meta: dict):
        return "<QQQII128s128sI" + Array.__serialize__(meta)

    def __save__(self, meta: dict):
        fmt = "<QQQII128s128sI"
        data = pack(fmt, self.min_, self.max_, self.def_, self.step, self.flags, self.name.encode("utf-8"),
                    self.func.encode("utf-8"), self.code_type.value)
        meta["data"].extend(data)
        Array([Char(i) for i in self.code.encode("utf-8")]).__save__(meta)

    @classmethod
    def __load__(cls, meta: dict, data: bytes, offset: int):
        fmt = "<QQQII128s128sI"
        min_, max_, def_, step, flags, name, func, code_type = \
            unpack_from(fmt, data, offset)
        offset += calcsize(fmt)
        name = name.decode("utf-8").strip("\x00")
        func = func.decode("utf-8").strip("\x00")
        code_type = CodeTypeEnum(code_type)
        code: List[Char] = Array.__load__(meta, data, offset, Char)
        code: str = "".join([chr(i) for i in code])
        return cls(min_=min_,
                   max_=max_,
                   def_=def_,
                   step=step,
                   flags=flags,
                   name=name,
                   func=func,
                   code_type=code_type,
                   code=code)


class SensorMode(BaseModel, Serializable, json_loads=json5.loads, json_dumps=json5.dumps):
    id: int = 0  # 1B
    width: int  # 4B
    height: int  # 4B
    order: Union[BayerOrderEnum, YUVOrderEnum]  # BayerOrderEnum | YUVOrderEnum  # 4B
    bit_depth: int  # 1B
    data_lanes: MIPILanesEnum  # 4B
    pixel_rate: int  # 4B
    hts: int  # 4B
    vts: int  # 4B
    controls: List[Control] = []  # 4B, 4B
    boards: conlist(item_type=List[Parameter], min_items=4, max_items=4)  # 8*4B
    registers: conlist(item_type=List[Parameter], min_items=4, max_items=4)  # 8*4B

    @classmethod
    def __serialize__(cls, meta: dict):
        return "<BIIIBIIII" + Array.__serialize__(meta) * 9

    def __save__(self, meta: dict):
        data: bytes = pack("<BIIIBIIII",
                           self.id,
                           self.width,
                           self.height,
                           self.order.value,
                           self.bit_depth,
                           self.data_lanes.value,
                           self.pixel_rate,
                           self.hts,
                           self.vts)
        meta["data"].extend(data)
        Array(self.controls).__save__(meta)
        for i in self.boards:
            Array(i).__save__(meta)
        for i in self.registers:
            Array(i).__save__(meta)

    @classmethod
    def __load__(cls, meta: dict, data: bytes, offset: int):
        fmt = "<BIIIBIIII"
        id_, width, height, order, bit_depth, data_lanes, pixel_rate, hts, vts = \
            unpack_from(fmt, data, offset)
        offset += calcsize(fmt)
        type_: PixFormatTypeEnum = meta["type"]
        if type_ == PixFormatTypeEnum.RAW8:
            order = BayerOrderEnum(order)
        elif type_ == PixFormatTypeEnum.RAW10:
            order = BayerOrderEnum(order)
        elif type_ == PixFormatTypeEnum.YUV422_8BIT:
            order = YUVOrderEnum(order)
        else:
            raise ValueError("Unknown type")
        data_lanes = MIPILanesEnum(data_lanes)
        controls = Array.__load__(meta, data, offset, Control)
        offset += calcsize(Array.__serialize__(meta))
        boards = []
        for i in range(4):
            boards.append(Array.__load__(meta, data, offset, Parameter))
            offset += calcsize(Array.__serialize__(meta))
        registers = []
        for i in range(4):
            registers.append(Array.__load__(meta, data, offset, Parameter))
            offset += calcsize(Array.__serialize__(meta))
        return cls(id=id_,
                   width=width,
                   height=height,
                   order=order,
                   bit_depth=bit_depth,
                   data_lanes=data_lanes,
                   pixel_rate=pixel_rate,
                   hts=hts,
                   vts=vts,
                   controls=controls,
                   boards=boards,
                   registers=registers)


class PixFormatType(BaseModel, Serializable, json_loads=json5.loads, json_dumps=json5.dumps):
    type: PixFormatTypeEnum  # 4B
    resolutions: List[SensorMode]  # 4B, 4B

    @classmethod
    def __serialize__(cls, meta: dict):
        return "<I" + Array.__serialize__(meta)

    def __save__(self, meta: dict):
        meta["data"].extend(pack("<I", self.type.value))
        Array(self.resolutions).__save__(meta)

    @classmethod
    def __load__(cls, meta: dict, data: bytes, offset: int):
        fmt = "<I"
        type_, = unpack_from(fmt, data, offset)
        offset += calcsize(fmt)
        type_ = PixFormatTypeEnum(type_)
        meta["type"] = type_
        resolutions = Array.__load__(meta, data, offset, SensorMode)
        return cls(type=type_, resolutions=resolutions)


class SubDevInfo(BaseModel, Serializable, json_loads=json5.loads, json_dumps=json5.dumps):
    i2c_addr: int  # 2B
    i2c_mode: I2CModeEnum  # 4B
    regs: List[SensorReg]  # 4B, 4B

    @classmethod
    def __serialize__(cls, meta: dict):
        return "<HI" + Array.__serialize__(meta)

    def __save__(self, meta: dict):
        meta["data"].extend(pack("<HI", self.i2c_addr, self.i2c_mode.value))
        Array(self.regs).__save__(meta)

    @classmethod
    def __load__(cls, meta: dict, data: bytes, offset: int):
        fmt = "<HI"
        i2c_addr, i2c_mode = unpack_from(fmt, data, offset)
        offset += calcsize(fmt)
        i2c_mode = I2CModeEnum(i2c_mode)
        regs = Array.__load__(meta, data, offset, SensorReg)
        return cls(i2c_addr=i2c_addr, i2c_mode=i2c_mode, regs=regs)


class SensorInfo(BaseModel, Serializable, json_loads=json5.loads, json_dumps=json5.dumps):
    name: str  # 32B
    i2c_mode: I2CModeEnum  # 4B
    i2c_addr: int  # 2B
    sensor_id: int = 0  # 4B
    flips_dont_change_bayer_order: int = 0  # 4B
    pix_formats: List[PixFormatType]  # 4B, 4B
    start: List[SensorReg]  # 4B, 4B
    stop: List[SensorReg]  # 4B, 4B
    sub_devs: List[SubDevInfo]  # 4B, 4B
    common_reg: List[SensorReg]  # 4B, 4B

    @classmethod
    def __serialize__(cls, meta: dict):
        return "<32sIHII" + Array.__serialize__(meta) * 5

    def __save__(self, meta: dict):
        meta["i2c_mode"] = self.i2c_mode
        meta["data"].extend(pack("<32sIHII",
                                 self.name.encode(),
                                 self.i2c_mode.value,
                                 self.i2c_addr,
                                 self.sensor_id,
                                 self.flips_dont_change_bayer_order))
        Array(self.pix_formats).__save__(meta)
        Array(self.start).__save__(meta)
        Array(self.stop).__save__(meta)
        Array(self.sub_devs).__save__(meta)
        Array(self.common_reg).__save__(meta)

    @classmethod
    def __load__(cls, meta: dict, data: bytes, offset: int):
        fmt = "<32sIHII"
        name, i2c_mode, i2c_addr, sensor_id, flips_dont_change_bayer_order = \
            unpack_from(fmt, data, offset)
        offset += calcsize(fmt)
        i2c_mode = I2CModeEnum(i2c_mode)
        meta["i2c_mode"] = i2c_mode
        pix_formats = Array.__load__(meta, data, offset, PixFormatType)
        offset += calcsize(Array.__serialize__(meta))
        start = Array.__load__(meta, data, offset, SensorReg)
        offset += calcsize(Array.__serialize__(meta))
        stop = Array.__load__(meta, data, offset, SensorReg)
        offset += calcsize(Array.__serialize__(meta))
        sub_devs = Array.__load__(meta, data, offset, SubDevInfo)
        offset += calcsize(Array.__serialize__(meta))
        common_reg = Array.__load__(meta, data, offset, SensorReg)
        return cls(name=name.decode().rstrip("\0"),
                   i2c_mode=i2c_mode,
                   i2c_addr=i2c_addr,
                   sensor_id=sensor_id,
                   flips_dont_change_bayer_order=flips_dont_change_bayer_order,
                   pix_formats=pix_formats,
                   start=start,
                   stop=stop,
                   sub_devs=sub_devs,
                   common_reg=common_reg)


class Header(BaseModel, Serializable, json_loads=json5.loads, json_dumps=json5.dumps):
    magic: bytes = b"ARDU"  # 4B
    length: int  # 4B
    data_offset: int  # 4B
    comment_offset: int  # 4B
    version: conlist(int, min_items=4, max_items=4)  # 4 * 1B
    flag: int  # 8B
    time: int  # 8B
    git_hash: bytes  # 20B

    @classmethod
    def __serialize__(cls, meta: dict):
        return "<4sIIIBBBBQQ20s"

    @save
    def __save__(self, fmt: str, meta: dict):
        return pack(fmt,
                    self.magic,
                    self.length,
                    self.data_offset,
                    self.comment_offset,
                    *self.version,
                    self.flag,
                    self.time,
                    self.git_hash)

    @classmethod
    @load
    def __load__(cls, data: tuple):
        magic, length, data_offset, comment_offset, *version, flag, time, git_hash = data
        assert magic == b"ARDU"
        assert len(version) == 4
        return cls(length=length,
                   data_offset=data_offset,
                   comment_offset=comment_offset,
                   version=version,
                   flag=flag,
                   time=time,
                   git_hash=git_hash)


class Blob(BaseModel, Serializable, json_loads=json5.loads, json_dumps=json5.dumps):
    name: str  # 32B
    data: bytes  # 4B, 4B

    @classmethod
    def __serialize__(cls, meta: dict):
        return "<32s" + Array.__serialize__(meta)

    def __save__(self, meta: dict):
        meta["data"].extend(pack("<32s",
                                 self.name.encode()))
        Array([Char(i) for i in self.data]).__save__(meta)

    @classmethod
    def __load__(cls, meta: dict, data: bytes, offset: int):
        fmt = "<32s"
        name, = unpack_from(fmt, data, offset)
        offset += calcsize(fmt)
        data = Array.__load__(meta, data, offset, Char)
        return cls(name=name.decode().rstrip("\0"),
                   data=bytes(data))


class Blobs(BaseModel, Serializable, json_loads=json5.loads, json_dumps=json5.dumps):
    blobs: List[Blob] = []  # 4B, 4B

    @classmethod
    def __serialize__(cls, meta: dict):
        return Array.__serialize__(meta)

    def __save__(self, meta: dict):
        Array(self.blobs).__save__(meta)

    @classmethod
    def __load__(cls, meta: dict, data: bytes, offset: int):
        blobs = Array.__load__(meta, data, offset, Blob)
        return cls(blobs=blobs)


def to_bytearray(obj: Serializable) -> bytearray:
    meta = {
        "addr": [],
        "data": bytearray(),
    }
    obj.__save__(meta)
    data = bytearray()

    def add_block(b):
        curr = b["data"]
        size = len(data) + len(curr)
        for offset, ptr in b['addr']:
            # print(f"addr: {offset}, ptr: {ptr}")
            pack_into("<I", curr, offset, size + ptr)
        data.extend(curr)

    while "payload" in meta:
        add_block(meta)
        meta = meta["payload"]

    add_block(meta)

    return data


def get_crc(data: bytearray) -> int:
    models = CrcModels()
    model = models.get_params('crc-16')
    crc = Crc(
        width=model["width"],
        poly=model["poly"],
        reflect_in=model["reflect_in"],
        xor_in=model["xor_in"],
        reflect_out=model["reflect_out"],
        xor_out=model["xor_out"],
    )
    return crc.table_driven(data)


def compress(data: bytes) -> bytes:
    return gzip_encode(data)


def decompress(data: bytes) -> bytes:
    return gzip_decode(data)


__all__ = [
    "to_bytearray",
    "get_crc",
    "compress",
    "decompress",
    "BayerOrderEnum",
    "YUVOrderEnum",
    "PixFormatTypeEnum",
    "I2CModeEnum",
    "MIPILanesEnum",
    "ParameterTypeEnum",
    "CodeTypeEnum",
    "HeaderFlagEnum",
    "SensorReg",
    "Delay",
    "VendorCommand",
    "Parameter",
    "Control",
    "SensorMode",
    "PixFormatType",
    "SubDevInfo",
    "SensorInfo",
    "Header",
    "Blob",
    "Blobs",
]
