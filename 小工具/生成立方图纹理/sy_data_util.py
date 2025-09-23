"""
立方图纹理数据处理工具模块
提供图像加载、处理和立方图纹理文件导出功能
"""

import os
from typing import List, Tuple
from PIL import Image
import convert_util


class SYDataUtil:
    """立方图纹理数据处理工具类"""
    
    def __init__(self):
        """初始化工具类"""
        self.sy_tex_width = 0
        self.sy_tex_height = 0
    
    def export_data(self, images: List[Image.Image], path: str) -> None:
        """
        导出立方图纹理数据到文件
        
        Args:
            images (List[Image.Image]): 图像列表，按顺序为左、右、上、下、后、前
            path (str): 输出文件路径（不包含扩展名）
            
        Raises:
            Exception: 文件写入失败时抛出异常
        """
        if not images:
            raise ValueError("图像列表不能为空")
        
        # 确保所有图像尺寸一致
        first_image = images[0]
        width, height = first_image.size
        
        for i, img in enumerate(images):
            if img.size != (width, height):
                raise ValueError(f"第{i+1}张图像尺寸不一致: 期望{width}x{height}, 实际{img.size}")
        
        # 创建输出文件
        output_path = f"{path}.bntexcube"
        
        with open(output_path, 'wb') as fout:
            # 写入图像宽度
            width_bytes = convert_util.from_int_to_bytes(width)
            fout.write(width_bytes)
            
            # 写入图像高度
            height_bytes = convert_util.from_int_to_bytes(height)
            fout.write(height_bytes)
            
            # 写入图像数量
            count_bytes = convert_util.from_int_to_bytes(len(images))
            fout.write(count_bytes)
            
            # 写入每张图像的像素数据
            for img in images:
                # 确保图像为RGBA模式
                if img.mode != 'RGBA':
                    img = img.convert('RGBA')
                
                # 获取像素数据
                pixels = img.load()
                
                for y in range(height):
                    for x in range(width):
                        r, g, b, a = pixels[x, y]
                        # 将RGBA转换为ARGB格式的整数
                        color_int = (a << 24) | (r << 16) | (g << 8) | b
                        # 转换为BGRA字节格式
                        color_bytes = convert_util.from_int_to_bytes_for_color(color_int)
                        fout.write(color_bytes)
    
    def gen_buffered_image(self, image_path: str) -> Image.Image:
        """
        从文件路径生成PIL图像对象
        
        Args:
            image_path (str): 图像文件路径
            
        Returns:
            Image.Image: PIL图像对象
            
        Raises:
            FileNotFoundError: 文件不存在时抛出异常
            Exception: 图像加载失败时抛出异常
        """
        if not os.path.exists(image_path):
            raise FileNotFoundError(f"图像文件不存在: {image_path}")
        
        try:
            img = Image.open(image_path)
            self.sy_tex_width, self.sy_tex_height = img.size
            print(f"加载图像: {self.sy_tex_width}x{self.sy_tex_height}")
            return img
        except Exception as e:
            raise Exception(f"加载图像失败: {image_path}, 错误: {str(e)}")
    
    def change(self, image_paths: List[str], output_path: str) -> bool:
        """
        将多张图像转换为立方图纹理文件
        
        Args:
            image_paths (List[str]): 图像文件路径列表，按顺序为左、右、上、下、后、前
            output_path (str): 输出文件路径（不包含扩展名）
            
        Returns:
            bool: 转换是否成功
        """
        if len(image_paths) != 6:
            print("错误: 立方图必须由六幅纹理构成！")
            return False
        
        try:
            # 加载所有图像
            images = []
            first_width, first_height = None, None
            
            for i, path in enumerate(image_paths):
                if not os.path.exists(path):
                    print(f"错误: 原图文件[{path}]不存在！")
                    return False
                
                img = self.gen_buffered_image(path)
                images.append(img)
                
                # 检查图像尺寸一致性
                if i == 0:
                    first_width, first_height = img.size
                elif img.size != (first_width, first_height):
                    print(f"错误: 第{i+1}张图像尺寸不一致！")
                    return False
            
            # 导出数据
            self.export_data(images, output_path)
            print(f"立方图纹理文件已生成: {output_path}.bntexcube")
            return True
            
        except Exception as e:
            print(f"转换失败: {str(e)}")
            return False
    
    def get_image_info(self, image_path: str) -> Tuple[int, int]:
        """
        获取图像信息
        
        Args:
            image_path (str): 图像文件路径
            
        Returns:
            Tuple[int, int]: (宽度, 高度)
            
        Raises:
            Exception: 获取图像信息失败时抛出异常
        """
        try:
            with Image.open(image_path) as img:
                return img.size
        except Exception as e:
            raise Exception(f"获取图像信息失败: {image_path}, 错误: {str(e)}")


# 创建全局实例
sy_data_util = SYDataUtil()
