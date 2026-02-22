"""
柏林噪声工具常量定义
包含程序中使用的各种常量和全局变量
"""

from PIL import Image
from typing import List

# 基础振幅，用于1D噪声显示
BASE_AMPLITUDE: float = 80.0

# X轴排列范围，用于1D噪声显示
X_ARRANGE: float = 800.0

# 2D噪声各分频是否增强对比度标志
ZQDBD_FLAG: bool = True

# 基础倍频数
BP: int = 2

# 噪声频率数（层数）
PLS: int = 5

# 基础计数
COUNT: int = 16

# X轴跨度
X_SPAN: float = 1.0

# 当前X坐标种子
X_CURR: int = 0

# 当前Y坐标种子
Y_CURR: int = 0

# 当前Z坐标种子
Z_CURR: int = 0

# 3D噪声各层图像数组
D3Each: List[Image.Image] = []
