"""
2D噪声显示组件
用于显示二维柏林噪声的图像
"""

from PySide6.QtWidgets import QWidget, QScrollArea
from PySide6.QtCore import Qt
from PySide6.QtGui import QPainter, QPixmap, QPalette
from PIL import Image
from typing import List, Optional


class TwoDDisplay(QWidget):
    """
    2D噪声显示组件类
    负责显示二维噪声图像
    """
    
    def __init__(self, parent=None):
        """
        初始化2D显示组件
        
        Args:
            parent: 父组件
        """
        super().__init__(parent)
        self.data: Optional[List[Image.Image]] = None
        self.setMinimumSize(550, 8000)
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
        
        for i in range(count):
            if i < len(self.data) and self.data[i] is not None:
                # 将PIL图像转换为QPixmap
                pil_image = self.data[i]
                
                # 转换为RGB模式（如果需要）
                if pil_image.mode != 'RGB':
                    pil_image = pil_image.convert('RGB')
                
                # 转换为QPixmap
                import io
                buffer = io.BytesIO()
                pil_image.save(buffer, format='PNG')
                buffer.seek(0)
                
                pixmap = QPixmap()
                pixmap.loadFromData(buffer.getvalue())
                
                # 绘制图像
                y_pos = (512 * i) + (5 * (i + 1))
                painter.drawPixmap(5, y_pos, pixmap)
    
    def refresh(self, data: List[Image.Image]):
        """
        刷新显示数据
        
        Args:
            data: 新的图像数据列表
        """
        self.data = data
        self.update()
