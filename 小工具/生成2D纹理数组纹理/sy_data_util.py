# -*- coding: utf-8 -*-
"""
图像数据处理工具类
提供图像加载、处理和纹理数组导出功能
"""

import os
from typing import List
from PIL import Image
from convert_util import ConvertUtil


class SYDataUtil:
    """图像数据处理工具类"""
    
    # 纹理尺寸常量
    SY_TEX_WIDTH = 0
    SY_TEX_HEIGHT = 0
    
    @staticmethod
    def export_data(bia: List[Image.Image], path: str) -> None:
        """
        导出纹理数组数据到.bntexa文件
        
        Args:
            bia (List[Image.Image]): 图像列表
            path (str): 输出文件路径（不含扩展名）
            
        Raises:
            Exception: 文件写入失败时抛出异常
        """
        output_path = f"{path}.bntexa"
        
        with open(output_path, 'wb') as fout:
            # 写入纹理宽度
            buf = ConvertUtil.from_int_to_bytes(bia[0].width)
            fout.write(buf)
            
            # 写入纹理高度
            buf2 = ConvertUtil.from_int_to_bytes(bia[0].height)
            fout.write(buf2)
            
            # 写入纹理数量
            buf3 = ConvertUtil.from_int_to_bytes(len(bia))
            fout.write(buf3)
            
            # 写入每个纹理的像素数据
            for buffered_image in bia:
                # 确保图像是RGBA模式
                if buffered_image.mode != 'RGBA':
                    buffered_image = buffered_image.convert('RGBA')
                
                for i in range(SYDataUtil.SY_TEX_HEIGHT):
                    for j in range(SYDataUtil.SY_TEX_WIDTH):
                        # 获取像素颜色
                        r, g, b, a = buffered_image.getpixel((j, i))
                        # 将RGBA转换为ARGB格式的整数
                        color = (a << 24) | (r << 16) | (g << 8) | b
                        # 转换为字节数组并写入
                        buf4 = ConvertUtil.from_int_to_bytes_for_color(color)
                        fout.write(buf4)
    
    @staticmethod
    def gen_buffered_image(image_path: str) -> Image.Image:
        """
        从图像文件路径生成PIL图像对象
        
        Args:
            image_path (str): 图像文件路径
            
        Returns:
            Image.Image: PIL图像对象
        """
        image = Image.open(image_path)
        SYDataUtil.SY_TEX_WIDTH = image.width
        SYDataUtil.SY_TEX_HEIGHT = image.height
        print(f"{SYDataUtil.SY_TEX_WIDTH}|{SYDataUtil.SY_TEX_HEIGHT}")
        
        # 确保图像是RGBA模式
        if image.mode != 'RGBA':
            image = image.convert('RGBA')
        
        return image
    
    @staticmethod
    def change(v_src: List[str], path: str) -> bool:
        """
        将多个图像文件转换为2D纹理数组纹理
        
        Args:
            v_src (List[str]): 源图像文件路径列表
            path (str): 输出文件路径（不含扩展名）
            
        Returns:
            bool: 转换是否成功
        """
        try:
            bia = []
            width = 0
            height = 0
            is_first = True
            
            # 加载所有图像并检查尺寸一致性
            for i, src_path in enumerate(v_src):
                image = SYDataUtil.gen_buffered_image(src_path)
                bia.append(image)
                
                if is_first:
                    width = image.width
                    height = image.height
                    is_first = False
                elif width != image.width or height != image.height:
                    print(f"图像尺寸不一致: {src_path} ({image.width}x{image.height}) vs 第一张 ({width}x{height})")
                    return False
            
            # 导出数据
            SYDataUtil.export_data(bia, path)
            return True
            
        except Exception as e:
            print(f"转换过程中发生错误: {e}")
            return False
