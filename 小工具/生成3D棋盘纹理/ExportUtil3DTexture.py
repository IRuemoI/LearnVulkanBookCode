"""
3D纹理导出工具类
用于将3D纹理数据导出为.bn3dtex格式的二进制文件
"""

import struct
from typing import List


class ExportUtil3DTexture:
    """3D纹理导出工具类"""
    
    @staticmethod
    def from_int_to_bytes(value: int) -> bytes:
        """
        将32位整数转换为4字节的字节数组
        
        Args:
            value (int): 要转换的32位整数
            
        Returns:
            bytes: 4字节的字节数组，按小端序排列
        """
        return struct.pack('<I', value & 0xFFFFFFFF)
    
    @staticmethod
    def export(data: List[List[List[int]]], path: str) -> None:
        """
        将3D纹理数据导出为.bn3dtex格式的二进制文件
        
        Args:
            data (List[List[List[int]]]): 3D纹理数据，格式为[width][height][depth]
            path (str): 输出文件路径（不包含扩展名）
            
        Raises:
            IOError: 文件写入失败时抛出异常
        """
        try:
            # 获取3D数据的维度
            width = len(data)
            height = len(data[0]) if width > 0 else 0
            depth = len(data[0][0]) if height > 0 else 0
            
            # 打开输出文件
            with open(f"{path}.bn3dtex", 'wb') as fout:
                # 写入文件头：宽度、高度、深度
                fout.write(ExportUtil3DTexture.from_int_to_bytes(width))
                fout.write(ExportUtil3DTexture.from_int_to_bytes(height))
                fout.write(ExportUtil3DTexture.from_int_to_bytes(depth))
                
                # 写入3D纹理数据
                for i in range(width):
                    for j in range(height):
                        for k in range(depth):
                            fout.write(ExportUtil3DTexture.from_int_to_bytes(data[i][j][k]))
                            
        except Exception as e:
            raise IOError(f"导出3D纹理文件失败: {str(e)}")
