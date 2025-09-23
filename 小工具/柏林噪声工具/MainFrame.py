"""
主窗口类
应用程序的主界面，包含控制面板和显示面板
"""

from PySide6.QtWidgets import (QMainWindow, QWidget, QHBoxLayout, 
                               QSplitter, QApplication)
from PySide6.QtCore import Qt
from ControlPanel import ControlPanel
from DisplayPanel import DisplayPanel


class MainFrame(QMainWindow):
    """
    主窗口类
    管理整个应用程序的界面布局
    """
    
    def __init__(self):
        """
        初始化主窗口
        """
        super().__init__()
        self.init_ui()
        
    def init_ui(self):
        """
        初始化用户界面
        """
        # 设置窗口标题
        self.setWindowTitle("Perlin噪声生成工具")
        
        # 创建中央部件
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        
        # 创建水平布局
        layout = QHBoxLayout(central_widget)
        layout.setContentsMargins(0, 0, 0, 0)
        
        # 创建分割器
        self.splitter = QSplitter(Qt.Orientation.Horizontal)
        layout.addWidget(self.splitter)
        
        # 创建控制面板
        self.control_panel = ControlPanel(self)
        self.splitter.addWidget(self.control_panel)
        
        # 创建显示面板
        self.display_panel = DisplayPanel(self)
        self.splitter.addWidget(self.display_panel)
        
        # 设置分割器比例
        self.splitter.setSizes([250, 850])
        self.splitter.setStretchFactor(0, 0)  # 控制面板不拉伸
        self.splitter.setStretchFactor(1, 1)  # 显示面板可拉伸
        
        # 设置窗口大小和位置
        self.setGeometry(10, 100, 1100, 800)
        
        # 设置窗口最小大小
        self.setMinimumSize(800, 600)
