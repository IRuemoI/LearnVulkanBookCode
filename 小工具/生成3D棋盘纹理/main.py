"""
3D棋盘纹理生成器
程序入口文件

这是一个用于生成3D棋盘纹理的工具，支持自定义颜色和分段数。
生成的纹理文件为.bn3dtex格式的二进制文件。

作者: 转换自Java版本
版本: 1.0.0
"""

import sys
import os

# 添加当前目录到Python路径，确保可以导入模块
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from MainFrame import main

if __name__ == "__main__":
    """
    程序入口点
    启动3D棋盘纹理生成器的图形界面
    """
    main()
