"""
显示面板组件
包含1D、2D、3D噪声的显示组件，使用标签页进行切换
"""

from PySide6.QtWidgets import (QWidget, QVBoxLayout, QTabWidget, 
                               QScrollArea, QFrame)
from PySide6.QtCore import Qt
from OneDDisplay import OneDDisplay
from TwoDDisplay import TwoDDisplay
from ThreeDDisplay import ThreeDDisplay


class DisplayPanel(QWidget):
    """
    显示面板类
    管理1D、2D、3D噪声的显示组件
    """
    
    def __init__(self, parent=None):
        """
        初始化显示面板
        
        Args:
            parent: 父组件
        """
        super().__init__(parent)
        self.father = parent
        self.init_ui()
        
    def init_ui(self):
        """
        初始化用户界面
        """
        layout = QVBoxLayout(self)
        layout.setContentsMargins(0, 0, 0, 0)
        
        # 创建标签页组件
        self.tab_widget = QTabWidget()
        layout.addWidget(self.tab_widget)
        
        # 创建1D显示组件
        self.one_d_display = OneDDisplay()
        self.scroll_1d = QScrollArea()
        self.scroll_1d.setWidget(self.one_d_display)
        self.scroll_1d.setWidgetResizable(True)
        self.scroll_1d.setHorizontalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAsNeeded)
        self.scroll_1d.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAsNeeded)
        self.tab_widget.addTab(self.scroll_1d, "1D噪声")
        
        # 创建2D显示组件
        self.two_d_display = TwoDDisplay()
        self.scroll_2d = QScrollArea()
        self.scroll_2d.setWidget(self.two_d_display)
        self.scroll_2d.setWidgetResizable(True)
        self.scroll_2d.setHorizontalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAsNeeded)
        self.scroll_2d.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAsNeeded)
        self.tab_widget.addTab(self.scroll_2d, "2D噪声")
        
        # 创建3D显示组件
        self.three_d_display = ThreeDDisplay()
        self.scroll_3d = QScrollArea()
        self.scroll_3d.setWidget(self.three_d_display)
        self.scroll_3d.setWidgetResizable(True)
        self.scroll_3d.setHorizontalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAsNeeded)
        self.scroll_3d.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAsNeeded)
        self.tab_widget.addTab(self.scroll_3d, "3D噪声")
