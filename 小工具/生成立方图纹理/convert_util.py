"""
数据类型转换工具模块
提供字符串、整数、浮点数与字节数组之间的转换功能
"""

import struct


def from_string_to_bytes(s: str) -> bytes:
    """
    将字符串转换为UTF-8编码的字节数组
    
    Args:
        s (str): 要转换的字符串
        
    Returns:
        bytes: UTF-8编码的字节数组
    """
    return s.encode('utf-8')


def from_float_to_bytes(f: float) -> bytes:
    """
    将浮点数转换为字节数组（小端序）
    
    Args:
        f (float): 要转换的浮点数
        
    Returns:
        bytes: 4字节的字节数组
    """
    return struct.pack('<f', f)


def from_int_to_bytes(k: int) -> bytes:
    """
    将整数转换为字节数组（小端序）
    
    Args:
        k (int): 要转换的整数
        
    Returns:
        bytes: 4字节的字节数组
    """
    return struct.pack('<I', k & 0xFFFFFFFF)


def from_int_to_bytes_for_color(k: int) -> bytes:
    """
    将颜色整数转换为字节数组（BGRA格式）
    
    Args:
        k (int): 要转换的颜色整数（ARGB格式）
        
    Returns:
        bytes: 4字节的字节数组（BGRA格式）
    """
    # 从ARGB转换为BGRA
    b = (k & 0x00FF0000) >> 16  # 红色通道
    g = (k & 0x0000FF00) >> 8   # 绿色通道
    r = (k & 0x000000FF)        # 蓝色通道
    a = (k & 0xFF000000) >> 24  # 透明度通道
    return struct.pack('<BBBB', b, g, r, a)


def from_bytes_to_string(buf_id: bytes) -> str:
    """
    将字节数组转换为UTF-8字符串
    
    Args:
        buf_id (bytes): 要转换的字节数组
        
    Returns:
        str: 解码后的字符串
    """
    return buf_id.decode('utf-8')


def from_bytes_to_int(buff: bytes) -> int:
    """
    将字节数组转换为整数（小端序）
    
    Args:
        buff (bytes): 4字节的字节数组
        
    Returns:
        int: 转换后的整数
    """
    return struct.unpack('<I', buff)[0]


def from_bytes_to_float(buf: bytes) -> float:
    """
    将字节数组转换为浮点数（小端序）
    
    Args:
        buf (bytes): 4字节的字节数组
        
    Returns:
        float: 转换后的浮点数
    """
    return struct.unpack('<f', buf)[0]
