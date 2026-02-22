# -*- coding: utf-8 -*-
"""
数据类型转换工具类
提供字符串、整数、浮点数与字节数组之间的转换功能
"""

import struct


class ConvertUtil:
    """数据类型转换工具类"""
    
    @staticmethod
    def from_string_to_bytes(s: str) -> bytes:
        """
        将字符串转换为UTF-8编码的字节数组
        
        Args:
            s (str): 要转换的字符串
            
        Returns:
            bytes: UTF-8编码的字节数组
        """
        return s.encode('utf-8')
    
    @staticmethod
    def from_float_to_bytes(f: float) -> bytes:
        """
        将浮点数转换为字节数组（小端序）
        
        Args:
            f (float): 要转换的浮点数
            
        Returns:
            bytes: 4字节的字节数组
        """
        return struct.pack('<f', f)
    
    @staticmethod
    def from_int_to_bytes(k: int) -> bytes:
        """
        将整数转换为字节数组（小端序）
        
        Args:
            k (int): 要转换的整数
            
        Returns:
            bytes: 4字节的字节数组
        """
        return struct.pack('<I', k & 0xFFFFFFFF)
    
    @staticmethod
    def from_int_to_bytes_for_color(k: int) -> bytes:
        """
        将整数转换为颜色字节数组（BGRA格式）
        
        Args:
            k (int): 要转换的颜色整数（ARGB格式）
            
        Returns:
            bytes: 4字节的字节数组（BGRA格式）
        """
        # 从ARGB格式转换为BGRA格式
        a = (k >> 24) & 0xFF
        r = (k >> 16) & 0xFF
        g = (k >> 8) & 0xFF
        b = k & 0xFF
        return struct.pack('<BBBB', b, g, r, a)
    
    @staticmethod
    def from_bytes_to_string(buf_id: bytes) -> str:
        """
        将字节数组转换为UTF-8字符串
        
        Args:
            buf_id (bytes): 要转换的字节数组
            
        Returns:
            str: 解码后的字符串
        """
        return buf_id.decode('utf-8')
    
    @staticmethod
    def from_bytes_to_int(buff: bytes) -> int:
        """
        将字节数组转换为整数（小端序）
        
        Args:
            buff (bytes): 4字节的字节数组
            
        Returns:
            int: 转换后的整数
        """
        return struct.unpack('<I', buff)[0]
    
    @staticmethod
    def from_bytes_to_float(buf: bytes) -> float:
        """
        将字节数组转换为浮点数（小端序）
        
        Args:
            buf (bytes): 4字节的字节数组
            
        Returns:
            float: 转换后的浮点数
        """
        return struct.unpack('<f', buf)[0]
