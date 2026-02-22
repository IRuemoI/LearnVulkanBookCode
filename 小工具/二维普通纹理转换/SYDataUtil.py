# -*- coding: utf-8 -*-
"""
图像处理和纹理导出工具类
负责将图片文件转换为bntex格式的纹理文件
"""

import os
from PIL import Image
from ConvertUtil import ConvertUtil


class SYDataUtil:
    """图像处理和纹理导出工具类"""
    
    def __init__(self):
        """初始化纹理尺寸变量"""
        self.sy_tex_width = 0
        self.sy_tex_height = 0
    
    def _export_data(self, image: Image.Image, pic_path: str) -> None:
        """
        将图像数据导出为bntex格式文件
        
        Args:
            image (Image.Image): PIL图像对象
            pic_path (str): 输出文件路径
        """
        with open(pic_path, 'wb') as fout:
            # 写入图像宽度（4字节）
            width_bytes = ConvertUtil.from_int_to_bytes(image.width)
            fout.write(width_bytes)
            
            # 写入图像高度（4字节）
            height_bytes = ConvertUtil.from_int_to_bytes(image.height)
            fout.write(height_bytes)
            
            # 写入像素数据
            for y in range(self.sy_tex_height):
                for x in range(self.sy_tex_width):
                    # 获取像素颜色（RGBA格式）
                    pixel = image.getpixel((x, y))
                    
                    # 如果是灰度图像，转换为RGBA
                    if isinstance(pixel, int):
                        pixel = (pixel, pixel, pixel, 255)
                    elif len(pixel) == 3:  # RGB格式
                        pixel = (pixel[0], pixel[1], pixel[2], 255)
                    
                    # 将RGBA转换为ARGB格式的整数
                    argb_color = (pixel[3] << 24) | (pixel[0] << 16) | (pixel[1] << 8) | pixel[2]
                    
                    # 转换为颜色字节数组并写入
                    color_bytes = ConvertUtil.from_int_to_bytes_for_color(argb_color)
                    fout.write(color_bytes)
    
    def _gen_pil_image(self, path: str) -> Image.Image:
        """
        从文件路径生成PIL图像对象
        
        Args:
            path (str): 图像文件路径
            
        Returns:
            Image.Image: PIL图像对象
        """
        try:
            image = Image.open(path)
            # 确保图像是RGBA模式
            if image.mode != 'RGBA':
                image = image.convert('RGBA')
            
            self.sy_tex_width = image.width
            self.sy_tex_height = image.height
            
            print(f"图像尺寸: {self.sy_tex_width} x {self.sy_tex_height}")
            return image
            
        except Exception as e:
            print(f"读取图像文件失败: {e}")
            raise e
    
    def change(self, src_path: str, dst_path: str) -> bool:
        """
        将源图像文件转换为bntex格式纹理文件
        
        Args:
            src_path (str): 源图像文件路径
            dst_path (str): 目标纹理文件路径
            
        Returns:
            bool: 转换是否成功
        """
        try:
            # 生成PIL图像对象
            image = self._gen_pil_image(src_path)
            
            # 导出数据到bntex文件
            self._export_data(image, dst_path)
            
            return True
            
        except Exception as e:
            print(f"转换失败: {e}")
            return False
