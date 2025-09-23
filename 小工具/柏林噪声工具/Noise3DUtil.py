"""
3D柏林噪声生成工具
提供三维柏林噪声的生成和处理功能
"""

import numpy as np
from PIL import Image
from typing import List, Tuple
from Constant import *


# 全局3D噪声尺寸
GLOBAL_SIZE = 64


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


def noise(x: int, y: int, z: int, amp: float) -> float:
    """
    生成三维噪声值
    
    Args:
        x: X坐标
        y: Y坐标
        z: Z坐标
        amp: 振幅
        
    Returns:
        噪声值
    """
    n = x + (y * 57) + (z * 3249)
    n2 = (n << 13) ^ n
    result = ((1.0 - ((((n2 * (((n2 * n2) * 15731) + 789221)) + 1376312589) & 0x7FFFFFFF) / 1073741824.0)) + 1.0) / 2.0
    return result * amp


def cal_yjbp(level: int) -> np.ndarray:
    """
    计算指定层级的3D噪声数据
    
    Args:
        level: 噪声层级
        
    Returns:
        三维噪声数据数组
    """
    size = pow_func(2, level)
    result = np.zeros((size, size, size), dtype=np.int32)
    
    # 生成基础噪声数据
    for i in range(size):
        for j in range(size):
            for k in range(size):
                global X_CURR, Y_CURR, Z_CURR
                X_CURR += 1
                Y_CURR += 1
                Z_CURR += 1
                val = noise(X_CURR, Y_CURR, Z_CURR, 255.0)
                result[i, j, k] = int(val)
    
    # 缩放到全局尺寸
    result_help = np.zeros((GLOBAL_SIZE, GLOBAL_SIZE, GLOBAL_SIZE), dtype=np.int32)
    scale = GLOBAL_SIZE // size
    
    for i in range(GLOBAL_SIZE):
        for j in range(GLOBAL_SIZE):
            for k in range(GLOBAL_SIZE):
                result_help[i, j, k] = result[i // scale, j // scale, k // scale]
    
    # 应用平滑滤波
    result_final = np.zeros((GLOBAL_SIZE, GLOBAL_SIZE, GLOBAL_SIZE), dtype=np.int32)
    half = scale // 2
    
    for i in range(GLOBAL_SIZE):
        for j in range(GLOBAL_SIZE):
            for k in range(GLOBAL_SIZE):
                count = 0.0
                temp = 0
                
                for a in range(-half, half + 1):
                    for b in range(-half, half + 1):
                        for c in range(-half, half + 1):
                            index_x = i + a
                            index_y = j + b
                            index_z = k + c
                            
                            if (0 <= index_x < GLOBAL_SIZE and 
                                0 <= index_y < GLOBAL_SIZE and 
                                0 <= index_z < GLOBAL_SIZE):
                                temp += result_help[index_x, index_y, index_z]
                                count += 1.0
                
                result_final[i, j, k] = int(temp / count) if count > 0 else 0
    
    return result_final


def cal_sybp_bpfl() -> np.ndarray:
    """
    计算各倍频分离的3D噪声数据（RGBA各一个倍频）
    
    Returns:
        四维数组，最后一维包含RGBA四个通道
    """
    result_final_l = np.zeros((GLOBAL_SIZE, GLOBAL_SIZE, GLOBAL_SIZE, 4), dtype=np.int32)
    
    # 计算4个倍频
    for l in range(1, 5):
        result_temp = cal_yjbp(l)
        result_final_l[:, :, :, l - 1] = result_temp
    
    # 生成各层图像
    global D3Each
    D3Each = []
    
    for d in range(GLOBAL_SIZE):
        img = Image.new('RGBA', (GLOBAL_SIZE, GLOBAL_SIZE))
        
        for i in range(GLOBAL_SIZE):
            for j in range(GLOBAL_SIZE):
                red = result_final_l[d, i, j, 0]
                green = result_final_l[d, i, j, 1]
                blue = result_final_l[d, i, j, 2]
                alpha = result_final_l[d, i, j, 3]
                
                img.putpixel((i, j), (red, green, blue, alpha))
        
        D3Each.append(img)
    
    return result_final_l


def cal_sybp() -> np.ndarray:
    """
    计算合成3D噪声数据
    
    Returns:
        三维噪声数据数组
    """
    result_final = np.zeros((GLOBAL_SIZE, GLOBAL_SIZE, GLOBAL_SIZE), dtype=np.int32)
    
    # 合成各层级噪声
    for l in range(1, PLS + 1):
        result_temp = cal_yjbp(l)
        result_final = (result_final * 2) + result_temp
    
    # 归一化到0-255范围
    max_val = np.max(result_final)
    if max_val > 0:
        result_final = (result_final * 256) // max_val
    
    # 生成各层图像
    global D3Each
    D3Each = []
    
    for d in range(GLOBAL_SIZE):
        img = Image.new('RGB', (GLOBAL_SIZE, GLOBAL_SIZE))
        
        for i in range(GLOBAL_SIZE):
            for j in range(GLOBAL_SIZE):
                gray = result_final[d, i, j]
                img.putpixel((i, j), (gray, gray, gray))
        
        D3Each.append(img)
    
    return result_final
