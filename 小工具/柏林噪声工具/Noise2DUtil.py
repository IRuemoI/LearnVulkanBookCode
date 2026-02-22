"""
2D柏林噪声生成工具
提供二维柏林噪声的生成和图像处理功能
"""

from PIL import Image, ImageFilter
from typing import List
from Constant import *


def pow_func(a: int, b: int) -> int:
    """
    计算a的b次方
    
    Args:
        a: 底数
        b: 指数
        
    Returns:
        a的b次方结果
    """
    result = 1
    for i in range(b):
        result *= a
    return result


def noise(x: int, y: int, amp: float) -> float:
    """
    生成二维噪声值
    
    Args:
        x: X坐标
        y: Y坐标
        amp: 振幅
        
    Returns:
        噪声值
    """
    n = x + (y * 57)
    n2 = (n << 13) ^ n
    result = ((1.0 - ((((n2 * (((n2 * n2) * 15731) + 789221)) + 1376312589) & 0x7FFFFFFF) / 1073741824.0)) + 1.0) / 2.0
    return result * amp


def cal_yjbp(level: int) -> Image.Image:
    """
    计算指定层级的2D噪声图像
    
    Args:
        level: 噪声层级
        
    Returns:
        生成的噪声图像
    """
    size = COUNT * pow_func(2, level - 1)
    temp = Image.new('RGB', (size, size))
    amp = 255.0 / pow_func(2, level)
    
    if ZQDBD_FLAG:
        amp = 255.0
    
    # 生成噪声数据
    for i in range(size):
        for j in range(size):
            global X_CURR, Y_CURR
            X_CURR += 1
            Y_CURR += 1
            val = noise(X_CURR, Y_CURR, amp)
            gray = int(val)
            temp.putpixel((j, i), (gray, gray, gray))
    
    # 缩放到512x512
    result = temp.resize((512, 512), Image.Resampling.LANCZOS)
    return result


def cal_sybp() -> List[Image.Image]:
    """
    计算所有层级的2D噪声图像
    
    Returns:
        包含所有层级噪声图像和合成结果的列表
    """
    result = []
    
    # 计算各层级噪声
    for i in range(1, PLS + 1):
        result.append(cal_yjbp(i))
    
    # 计算合成结果
    if result:
        composite = Image.new('RGB', (512, 512))
        
        for i in range(512):
            for j in range(512):
                gray = 0
                for k in range(1, PLS + 1):
                    r, g, b = result[k - 1].getpixel((i, j))
                    
                    if ZQDBD_FLAG:
                        gray_temp = int(((r + g + b) / 3.0) / pow_func(2, k))
                    else:
                        gray_temp = (r + g + b) // 3
                    
                    gray += gray_temp
                
                composite.putpixel((i, j), (gray, gray, gray))
        
        result.append(composite)
    
    return result
