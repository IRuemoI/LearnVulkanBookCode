"""
3D棋盘纹理生成类
用于生成3D棋盘纹理数据，支持自定义颜色和分段数
"""

from typing import List
from ExportUtil3DTexture import ExportUtil3DTexture


class WhiteAndRed:
    """3D棋盘纹理生成类"""
    
    @staticmethod
    def gen_3d_texture(span: int, color_a: int, color_b: int, path: str) -> bool:
        """
        生成3D棋盘纹理
        
        Args:
            span (int): 每个维度的分段数，决定棋盘格子的数量
            color_a (int): 棋盘颜色1的ARGB值
            color_b (int): 棋盘颜色2的ARGB值
            path (str): 输出文件路径（不包含扩展名）
            
        Returns:
            bool: 生成成功返回True，失败返回False
        """
        try:
            # 创建3D数据数组
            data: List[List[List[int]]] = [[[0 for _ in range(span)] for _ in range(span)] for _ in range(span)]
            
            # 填充3D棋盘纹理数据
            for i in range(span):
                for j in range(span):
                    for k in range(span):
                        # 计算当前位置的棋盘模式
                        # 使用 (i + j + k) % 2 来决定使用哪种颜色
                        which_color = (i + j + k) % 2
                        
                        if which_color == 1:
                            data[i][j][k] = color_a
                        else:
                            data[i][j][k] = color_b
            
            # 导出3D纹理数据到文件
            ExportUtil3DTexture.export(data, path)
            return True
            
        except Exception as e:
            print(f"生成3D棋盘纹理失败: {str(e)}")
            return False
