"""
1D噪声显示组件
用于显示一维柏林噪声的波形图
"""

from PySide6.QtWidgets import QWidget, QScrollArea
from PySide6.QtCore import Qt
from PySide6.QtGui import QPainter, QPen, QBrush, QColor, QPalette
from typing import List, Optional
from Constant import *


class OneDDisplay(QWidget):
    """
    1D噪声显示组件类
    负责绘制一维噪声的波形图
    """
    
    def __init__(self, parent=None):
        """
        初始化1D显示组件
        
        Args:
            parent: 父组件
        """
        super().__init__(parent)
        self.line_data: Optional[List[List[List[float]]]] = None
        self.setMinimumSize(550, 900)
        self.setBackgroundRole(QPalette.ColorRole.Window)
        
    def paintEvent(self, event):
        """
        绘制事件处理函数
        
        Args:
            event: 绘制事件
        """
        painter = QPainter(self)
        painter.setRenderHint(QPainter.RenderHint.Antialiasing)
        
        # 填充黑色背景
        painter.fillRect(self.rect(), QColor(0, 0, 0))
        
        if self.line_data is None:
            return
        
        count = len(self.line_data)
        if count == 0 or len(self.line_data[0]) == 0:
            return
            
        x_span = X_ARRANGE / len(self.line_data[0])
        
        for i in range(count):
            # 绘制白色背景矩形
            painter.setBrush(QBrush(QColor(255, 255, 255)))
            painter.setPen(Qt.PenStyle.NoPen)
            
            if i == count - 1:
                # 最后一层（合成结果）使用更大的高度
                rect_height = int(BASE_AMPLITUDE) + 100
            else:
                rect_height = int(BASE_AMPLITUDE)
            
            y_start = (5 * (i + 1)) + (int(BASE_AMPLITUDE) * i)
            painter.drawRect(5, y_start, int(X_ARRANGE), rect_height)
            
            # 绘制噪声波形
            points = self.line_data[i]
            if len(points) < 2:
                continue
                
            painter.setPen(QPen(QColor(0, 0, 0), 1))
            
            for j in range(len(points) - 1):
                x1 = int(x_span * j) + 5
                y1 = int(points[j][1] * BASE_AMPLITUDE * (i + 1 if i != count - 1 else 1)) + y_start
                x2 = int((x_span * j) + x_span) + 5
                y2 = int(points[j + 1][1] * BASE_AMPLITUDE * (i + 1 if i != count - 1 else 1)) + y_start
                
                painter.drawLine(x1, y1, x2, y2)
    
    def refresh(self, line_data: List[List[List[float]]]):
        """
        刷新显示数据
        
        Args:
            line_data: 新的噪声数据
        """
        self.line_data = line_data
        self.update()
