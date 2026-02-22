"""
数据类型转换工具类
提供各种数据类型之间的转换功能，包括字节、整数、浮点数、字符串等
"""

import struct


class ConvertUtil:
    """数据类型转换工具类"""
    
    @staticmethod
    def from_int_to_unsigned_byte(data: int) -> int:
        """
        将整数转换为无符号字节
        
        Args:
            data (int): 输入的整数
            
        Returns:
            int: 转换后的无符号字节值 (0-255)
        """
        return data & 255
    
    @staticmethod
    def from_unsigned_byte_to_int(signed_byte: int) -> int:
        """
        将无符号字节转换为整数
        
        Args:
            signed_byte (int): 输入的无符号字节值
            
        Returns:
            int: 转换后的整数
        """
        return 255 & signed_byte
    
    @staticmethod
    def from_string_to_bytes(s: str) -> bytes:
        """
        将字符串转换为UTF-8编码的字节数组
        
        Args:
            s (str): 输入的字符串
            
        Returns:
            bytes: UTF-8编码的字节数组
        """
        return s.encode('utf-8')
    
    @staticmethod
    def from_float_to_bytes(f: float) -> bytes:
        """
        将浮点数转换为字节数组
        
        Args:
            f (float): 输入的浮点数
            
        Returns:
            bytes: 4字节的字节数组
        """
        return struct.pack('<f', f)
    
    @staticmethod
    def from_int_to_bytes(k: int) -> bytes:
        """
        将整数转换为小端序字节数组
        
        Args:
            k (int): 输入的整数
            
        Returns:
            bytes: 4字节的小端序字节数组
        """
        return struct.pack('<I', k)
    
    @staticmethod
    def from_int_to_bytes_for_color(k: int) -> bytes:
        """
        将整数转换为颜色格式的字节数组 (BGRA格式)
        
        Args:
            k (int): 输入的整数颜色值
            
        Returns:
            bytes: 4字节的颜色格式字节数组
        """
        # 提取RGBA分量
        r = (k & 0xFF0000) >> 16
        g = (k & 0x00FF00) >> 8
        b = (k & 0x0000FF)
        a = (k & 0xFF000000) >> 24
        
        # 返回BGRA格式的字节数组
        return bytes([b, g, r, a])
    
    @staticmethod
    def from_bytes_to_string(buf_id: bytes) -> str:
        """
        将字节数组转换为UTF-8字符串
        
        Args:
            buf_id (bytes): 输入的字节数组
            
        Returns:
            str: 解码后的字符串
        """
        return buf_id.decode('utf-8')
    
    @staticmethod
    def from_bytes_to_int(buff: bytes) -> int:
        """
        将字节数组转换为整数 (小端序)
        
        Args:
            buff (bytes): 输入的4字节数组
            
        Returns:
            int: 转换后的整数
        """
        return struct.unpack('<I', buff)[0]
    
    @staticmethod
    def from_bytes_to_float(buf: bytes) -> float:
        """
        将字节数组转换为浮点数
        
        Args:
            buf (bytes): 输入的4字节数组
            
        Returns:
            float: 转换后的浮点数
        """
        return struct.unpack('<f', buf)[0]
