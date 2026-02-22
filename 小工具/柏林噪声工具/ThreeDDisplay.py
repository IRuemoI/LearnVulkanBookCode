"""
3D噪声显示组件
用于显示三维柏林噪声的切片图像
"""

from PySide6.QtWidgets import QWidget, QScrollArea
from PySide6.QtCore import Qt
from PySide6.QtGui import QPainter, QPixmap, QPalette
from PIL import Image
from typing import List, Optional
from Noise3DUtil import GLOBAL_SIZE


class ThreeDDisplay(QWidget):
    """
    3D噪声显示组件类
    负责显示三维噪声的切片图像
    """
    
    def __init__(self, parent=None):
        """
        初始化3D显示组件
        
        Args:
            parent: 父组件
        """
        super().__init__(parent)
        self.data: Optional[List[Image.Image]] = None
        self.setMinimumSize(560, 5000)
        self.setBackgroundRole(QPalette.ColorRole.Window)
        
    def paintEvent(self, event):
        """
        绘制事件处理函数
        
        Args:
            event: 绘制事件
        """
        painter = QPainter(self)
        painter.setRenderHint(QPainter.RenderHint.SmoothPixmapTransform)
        
        # 填充白色背景
        painter.fillRect(self.rect(), Qt.GlobalColor.white)
        
        if self.data is None:
            return
        
        count = len(self.data)
        if count == 0:
            return
        
        # 计算每行显示的图像数量
        cols = 560 // (GLOBAL_SIZE + 5)
        if cols <= 0:
            cols = 1
        
        k = 0
        for i in range(count):
            for j in range(cols):
                if k >= len(self.data):
                    return
                    
                if self.data[k] is not None:
                    # 将PIL图像转换为QPixmap
                    pil_image = self.data[k]
                    
                    # 转换为RGB模式（如果需要）
                    if pil_image.mode not in ['RGB', 'RGBA']:
                        pil_image = pil_image.convert('RGB')
                    
                    # 转换为QPixmap
                    import io
                    buffer = io.BytesIO()
                    pil_image.save(buffer, format='PNG')
                    buffer.seek(0)
                    
                    pixmap = QPixmap()
                    pixmap.loadFromData(buffer.getvalue())
                    
                    # 计算绘制位置
                    x_pos = (j * (GLOBAL_SIZE + 5)) + 5
                    y_pos = (GLOBAL_SIZE * i) + (5 * (i + 1))
                    
                    # 绘制图像
                    painter.drawPixmap(x_pos, y_pos, pixmap)
                
                k += 1
                if k >= len(self.data):
                    return
    
    def refresh(self, data: List[Image.Image]):
        """
        刷新显示数据
        
        Args:
            data: 新的图像数据列表
        """
        self.data = data
        self.update()
