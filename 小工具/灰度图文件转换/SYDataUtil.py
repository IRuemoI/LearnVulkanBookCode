"""
图像数据处理工具类
提供图像加载、处理和导出为bnhdt格式文件的功能
"""

from PIL import Image
import os
from ConvertUtil import ConvertUtil


class SYDataUtil:
    """图像数据处理工具类"""
    
    # 纹理尺寸变量
    SY_TEX_WIDTH = 0
    SY_TEX_HEIGHT = 0
    
    @staticmethod
    def cal_pixel_hd(color: int) -> int:
        """
        计算像素的灰度值
        
        Args:
            color (int): RGB颜色值
            
        Returns:
            int: 计算出的灰度值 (0-255)
        """
        # 提取RGB分量
        r = (color & 0xFF0000) >> 16
        g = (color & 0x00FF00) >> 8
        b = (color & 0x0000FF)
        
        # 计算灰度值 (简单平均)
        return (r + g + b) // 3
    
    @staticmethod
    def export_data(image: Image.Image, pic_path: str) -> None:
        """
        将图像数据导出为bnhdt格式文件
        
        Args:
            image (Image.Image): PIL图像对象
            pic_path (str): 输出文件路径
            
        Raises:
            Exception: 文件写入失败时抛出异常
        """
        with open(pic_path, 'wb') as fout:
            # 写入图像宽度 (4字节)
            width_bytes = ConvertUtil.from_int_to_bytes(image.width)
            fout.write(width_bytes)
            
            # 写入图像高度 (4字节)
            height_bytes = ConvertUtil.from_int_to_bytes(image.height)
            fout.write(height_bytes)
            
            # 写入像素数据
            for y in range(SYDataUtil.SY_TEX_HEIGHT):
                for x in range(SYDataUtil.SY_TEX_WIDTH):
                    # 获取像素颜色值
                    pixel = image.getpixel((x, y))
                    
                    # 如果是RGB格式，直接使用
                    if isinstance(pixel, tuple):
                        if len(pixel) == 3:  # RGB
                            r, g, b = pixel
                            color = (r << 16) | (g << 8) | b
                        elif len(pixel) == 4:  # RGBA
                            r, g, b, a = pixel
                            color = (a << 24) | (r << 16) | (g << 8) | b
                        else:  # 灰度图
                            color = (pixel << 16) | (pixel << 8) | pixel
                    else:  # 单值灰度
                        color = (pixel << 16) | (pixel << 8) | pixel
                    
                    # 计算灰度值并写入
                    gray_value = SYDataUtil.cal_pixel_hd(color)
                    gray_byte = ConvertUtil.from_int_to_unsigned_byte(gray_value)
                    fout.write(bytes([gray_byte]))
    
    @staticmethod
    def gen_pil_image(image_path: str) -> Image.Image:
        """
        从文件路径生成PIL图像对象
        
        Args:
            image_path (str): 图像文件路径
            
        Returns:
            Image.Image: PIL图像对象
        """
        image = Image.open(image_path)
        
        # 转换为RGB模式以确保兼容性
        if image.mode != 'RGB':
            image = image.convert('RGB')
        
        # 设置全局尺寸变量
        SYDataUtil.SY_TEX_WIDTH = image.width
        SYDataUtil.SY_TEX_HEIGHT = image.height
        
        print(f"图像尺寸: {SYDataUtil.SY_TEX_WIDTH} x {SYDataUtil.SY_TEX_HEIGHT}")
        
        return image
    
    @staticmethod
    def change(src_path: str, dst_path: str) -> bool:
        """
        将源图像文件转换为bnhdt格式文件
        
        Args:
            src_path (str): 源图像文件路径
            dst_path (str): 目标bnhdt文件路径
            
        Returns:
            bool: 转换成功返回True，失败返回False
        """
        try:
            # 检查源文件是否存在
            if not os.path.exists(src_path):
                print(f"源文件不存在: {src_path}")
                return False
            
            # 生成PIL图像对象
            image = SYDataUtil.gen_pil_image(src_path)
            
            # 导出数据到bnhdt文件
            SYDataUtil.export_data(image, dst_path)
            
            print(f"转换成功: {src_path} -> {dst_path}")
            return True
            
        except Exception as e:
            print(f"转换失败: {e}")
            return False
