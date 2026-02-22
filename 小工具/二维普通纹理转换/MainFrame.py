# -*- coding: utf-8 -*-
"""
二维普通纹理转换工具主界面
使用PySide6构建的图形用户界面
"""

import os
import sys
from pathlib import Path
from PySide6.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout, 
                               QHBoxLayout, QLabel, QLineEdit, QPushButton, 
                               QFileDialog, QMessageBox, QFrame)
from PySide6.QtCore import Qt
from PySide6.QtGui import QFont
from SYDataUtil import SYDataUtil


class MainFrame(QMainWindow):
    """主窗口类"""
    
    def __init__(self):
        """
        初始化主窗口
        """
        super().__init__()
        self.sy_data_util = SYDataUtil()
        self.init_ui()
        
    def init_ui(self):
        """
        初始化用户界面
        """
        self.setWindowTitle("图片转为bntex格式纹理")
        self.setFixedSize(500, 150)
        
        # 创建中央部件
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        
        # 创建主布局
        main_layout = QVBoxLayout(central_widget)
        main_layout.setSpacing(15)
        main_layout.setContentsMargins(20, 20, 20, 20)
        
        # 创建原图文件选择区域
        src_layout = self._create_file_selection_layout(
            "原图文件:", "选择源图像文件", self._select_source_file
        )
        main_layout.addLayout(src_layout)
        
        # 创建纹理目录选择区域
        dst_layout = self._create_file_selection_layout(
            "纹理目录:", "选择目标目录", self._select_destination_directory
        )
        main_layout.addLayout(dst_layout)
        
        # 创建转换按钮
        self.convert_button = QPushButton("转换")
        self.convert_button.setFixedHeight(35)
        self.convert_button.setStyleSheet("""
            QPushButton {
                background-color: #4CAF50;
                color: white;
                border: none;
                border-radius: 5px;
                font-size: 14px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #45a049;
            }
            QPushButton:pressed {
                background-color: #3d8b40;
            }
            QPushButton:disabled {
                background-color: #cccccc;
                color: #666666;
            }
        """)
        self.convert_button.clicked.connect(self._perform_conversion)
        main_layout.addWidget(self.convert_button)
        
        # 初始化文件路径输入框
        self.src_path_edit = None
        self.dst_path_edit = None
        
    def _create_file_selection_layout(self, label_text: str, button_text: str, 
                                    button_callback) -> QHBoxLayout:
        """
        创建文件选择布局
        
        Args:
            label_text (str): 标签文本
            button_text (str): 按钮文本
            button_callback: 按钮点击回调函数
            
        Returns:
            QHBoxLayout: 水平布局
        """
        layout = QHBoxLayout()
        layout.setSpacing(10)
        
        # 创建标签
        label = QLabel(label_text)
        label.setFixedWidth(80)
        label.setFont(QFont("Arial", 10))
        layout.addWidget(label)
        
        # 创建路径输入框
        path_edit = QLineEdit()
        path_edit.setPlaceholderText("请选择文件或目录...")
        path_edit.setStyleSheet("""
            QLineEdit {
                padding: 8px;
                border: 2px solid #ddd;
                border-radius: 4px;
                font-size: 12px;
            }
            QLineEdit:focus {
                border-color: #4CAF50;
            }
        """)
        layout.addWidget(path_edit)
        
        # 创建选择按钮
        select_button = QPushButton(button_text)
        select_button.setFixedWidth(80)
        select_button.setFixedHeight(35)
        select_button.setStyleSheet("""
            QPushButton {
                background-color: #2196F3;
                color: white;
                border: none;
                border-radius: 4px;
                font-size: 12px;
            }
            QPushButton:hover {
                background-color: #1976D2;
            }
            QPushButton:pressed {
                background-color: #1565C0;
            }
        """)
        select_button.clicked.connect(button_callback)
        layout.addWidget(select_button)
        
        # 保存输入框引用
        if "原图" in label_text:
            self.src_path_edit = path_edit
        else:
            self.dst_path_edit = path_edit
            
        return layout
    
    def _select_source_file(self):
        """
        选择源图像文件
        """
        file_path, _ = QFileDialog.getOpenFileName(
            self,
            "选择源图像文件",
            self._get_current_path("picSrc"),
            "图像文件 (*.png *.jpg *.jpeg *.bmp *.gif *.tiff);;所有文件 (*)"
        )
        
        if file_path:
            self.src_path_edit.setText(file_path)
    
    def _select_destination_directory(self):
        """
        选择目标目录
        """
        dir_path = QFileDialog.getExistingDirectory(
            self,
            "选择目标目录",
            self._get_current_path("texDst")
        )
        
        if dir_path:
            self.dst_path_edit.setText(dir_path)
    
    def _get_current_path(self, default_name: str) -> str:
        """
        获取当前路径，如果不存在则返回默认路径
        
        Args:
            default_name (str): 默认目录名
            
        Returns:
            str: 当前路径
        """
        current_path = Path(default_name)
        if current_path.exists():
            return str(current_path.absolute())
        return str(Path.home())
    
    def _perform_conversion(self):
        """
        执行图像转换操作
        """
        # 禁用转换按钮
        self.convert_button.setEnabled(False)
        self.convert_button.setText("转换中...")
        
        try:
            # 获取源文件路径
            src_path = self.src_path_edit.text().strip()
            if not src_path:
                self._show_message("错误", "请选择源图像文件！", QMessageBox.Critical)
                return
            
            # 检查源文件是否存在
            if not os.path.exists(src_path):
                self._show_message("错误", "原图文件不存在！", QMessageBox.Critical)
                return
            
            # 获取目标目录路径
            dst_dir = self.dst_path_edit.text().strip()
            if not dst_dir:
                self._show_message("错误", "请选择目标目录！", QMessageBox.Critical)
                return
            
            # 检查目标目录是否存在
            if not os.path.exists(dst_dir):
                self._show_message("错误", "目标路径不存在！", QMessageBox.Critical)
                return
            
            # 生成目标文件路径
            src_file_name = Path(src_path).stem  # 获取不带扩展名的文件名
            dst_file_path = os.path.join(dst_dir, f"{src_file_name}.bntex")
            
            print(f"目标文件路径: {dst_file_path}")
            
            # 执行转换
            success = self.sy_data_util.change(src_path, dst_file_path)
            
            if success:
                self._show_message("成功", "转换成功！", QMessageBox.Information)
            else:
                self._show_message("失败", "转换失败！", QMessageBox.Critical)
                
        except Exception as e:
            self._show_message("错误", f"转换过程中发生错误: {str(e)}", QMessageBox.Critical)
        finally:
            # 重新启用转换按钮
            self.convert_button.setEnabled(True)
            self.convert_button.setText("转换")
    
    def _show_message(self, title: str, message: str, icon_type):
        """
        显示消息对话框
        
        Args:
            title (str): 对话框标题
            message (str): 消息内容
            icon_type: 图标类型
        """
        msg_box = QMessageBox(self)
        msg_box.setWindowTitle(title)
        msg_box.setText(message)
        msg_box.setIcon(icon_type)
        msg_box.exec()


def main():
    """
    主函数
    """
    app = QApplication(sys.argv)
    
    # 设置应用程序样式
    app.setStyle('Fusion')
    
    # 创建主窗口
    window = MainFrame()
    window.show()
    
    # 运行应用程序
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
