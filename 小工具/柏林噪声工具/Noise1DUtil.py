"""
1D柏林噪声生成工具
提供一维柏林噪声的生成和计算功能
"""

import math
from typing import List, Tuple
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


def noise(x: int, amp: float) -> float:
    """
    生成一维噪声值
    
    Args:
        x: 输入坐标
        amp: 振幅
        
    Returns:
        噪声值
    """
    x2 = (x << 13) ^ x
    result = ((1.0 - ((((x2 * (((x2 * x2) * 15731) + 789221)) + 1376312589) & 0x7FFFFFFF) / 1073741824.0)) + 1.0) / 2.0
    return result * amp


def cos_in(a: float, b: float, x: float) -> float:
    """
    使用余弦插值在a和b之间进行插值
    
    Args:
        a: 起始值
        b: 结束值
        x: 插值参数 (0-1)
        
    Returns:
        插值结果
    """
    ft = x * math.pi
    f = (1.0 - math.cos(ft)) * 0.5
    return (a * (1.0 - f)) + (b * f)


def cal_yjbp(level: int) -> List[List[float]]:
    """
    计算指定层级的1D噪声数据
    
    Args:
        level: 噪声层级
        
    Returns:
        包含坐标和噪声值的二维数组
    """
    tc = COUNT * pow_func(BP, level - 1)
    cp = [0.0] * (tc + 1)
    x_span = X_SPAN / pow_func(BP, level - 1)
    amp = 1.0 / pow_func(BP, level - 1)
    
    # 生成控制点
    for i in range(tc + 1):
        global X_CURR
        X_CURR += 1
        cp[i] = noise(X_CURR, amp)
    
    # 计算插值点
    qfs = pow_func(BP, 8 - level)
    x_span_qf = x_span / qfs
    result = []
    
    for i in range(tc):
        a = cp[i]
        b = cp[i + 1]
        result.append([i * x_span, a])
        
        for j in range(1, qfs):
            x_in = (1.0 / qfs) * j
            result.append([(i * x_span) + (x_span_qf * j), cos_in(a, b, x_in)])
    
    result.append([COUNT, cp[tc]])
    return result


def cal_sybp() -> List[List[List[float]]]:
    """
    计算所有层级的1D噪声数据
    
    Returns:
        三维数组，包含所有层级的噪声数据和合成结果
    """
    result = []
    
    # 计算各层级噪声
    for i in range(1, PLS + 1):
        result.append(cal_yjbp(i))
    
    # 计算合成结果
    if result:
        composite = []
        for i in range(len(result[0])):
            x = result[0][i][0]
            temp = 0.0
            for j in range(PLS):
                temp += result[j][i][1]
            composite.append([x, temp])
        result.append(composite)
    
    return result
