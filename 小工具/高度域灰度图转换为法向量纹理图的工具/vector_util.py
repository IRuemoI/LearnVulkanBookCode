"""
向量计算工具模块
提供向量叉积、向量归一化等数学计算功能
"""

import math
from typing import List, Tuple


def get_cross_product(x1: float, y1: float, z1: float, 
                     x2: float, y2: float, z2: float) -> List[float]:
    """
    计算两个三维向量的叉积
    
    Args:
        x1, y1, z1: 第一个向量的x、y、z分量
        x2, y2, z2: 第二个向量的x、y、z分量
    
    Returns:
        List[float]: 叉积结果向量 [A, B, C]
    """
    A = (y1 * z2) - (y2 * z1)
    B = (z1 * x2) - (z2 * x1)
    C = (x1 * y2) - (x2 * y1)
    return [A, B, C]


def vector_normal(vector: List[float]) -> List[float]:
    """
    将向量归一化（单位化）
    
    Args:
        vector: 输入向量 [x, y, z]
    
    Returns:
        List[float]: 归一化后的向量
    """
    module = math.sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2])
    return [vector[0] / module, vector[1] / module, vector[2] / module]


def mould(vec: List[float]) -> float:
    """
    计算向量的模长
    
    Args:
        vec: 输入向量 [x, y, z]
    
    Returns:
        float: 向量的模长
    """
    return math.sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2])
