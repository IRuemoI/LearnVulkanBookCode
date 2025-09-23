"""
3D纹理导出工具
提供3D噪声数据的导出功能，支持多种格式
"""

import os
import struct
from typing import Union
import numpy as np


def from_int_to_bytes(k: int) -> bytes:
    """
    将整数转换为字节数组
    
    Args:
        k: 要转换的整数
        
    Returns:
        4字节的字节数组
    """
    return struct.pack('<I', k & 0xFFFFFFFF)


def export(data: np.ndarray) -> None:
    """
    导出3D噪声数据为.bn3dtex格式文件
    
    Args:
        data: 三维噪声数据数组
        
    Raises:
        IOError: 文件写入失败时抛出
    """
    # 确保输出目录存在
    os.makedirs("TexData3D", exist_ok=True)
    
    try:
        with open("TexData3D/3dNoise.bn3dtex", "wb") as fout:
            width = data.shape[0]
            height = data.shape[1]
            depth = data.shape[2]
            
            # 写入尺寸信息
            fout.write(from_int_to_bytes(width))
            fout.write(from_int_to_bytes(height))
            fout.write(from_int_to_bytes(depth))
            
            # 写入数据
            for i in range(width):
                for j in range(height):
                    for k in range(depth):
                        gray = int(data[i, j, k])
                        # 写入RGBA格式：灰度值作为RGB，Alpha为255
                        fout.write(struct.pack('BBBB', gray, gray, gray, 255))
                        
    except Exception as e:
        raise IOError(f"导出3D纹理文件失败: {e}")


def export_bpfl(data: np.ndarray) -> None:
    """
    导出各倍频分离的3D噪声数据为.bn3dtex格式文件
    
    Args:
        data: 四维噪声数据数组，最后一维包含RGBA四个通道
        
    Raises:
        IOError: 文件写入失败时抛出
    """
    # 确保输出目录存在
    os.makedirs("TexData3D", exist_ok=True)
    
    try:
        with open("TexData3D/3dNoise.bn3dtex", "wb") as fout:
            width = data.shape[0]
            height = data.shape[1]
            depth = data.shape[2]
            
            # 写入尺寸信息
            fout.write(from_int_to_bytes(width))
            fout.write(from_int_to_bytes(height))
            fout.write(from_int_to_bytes(depth))
            
            # 写入数据
            for i in range(width):
                for j in range(height):
                    for k in range(depth):
                        red = int(data[i, j, k, 0])
                        green = int(data[i, j, k, 1])
                        blue = int(data[i, j, k, 2])
                        alpha = int(data[i, j, k, 3])
                        
                        # 写入RGBA格式
                        fout.write(struct.pack('BBBB', red, green, blue, alpha))
                        
    except Exception as e:
        raise IOError(f"导出3D纹理文件失败: {e}")
