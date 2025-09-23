# -*- coding: utf-8 -*-
"""
主界面类
使用PySide6创建图形用户界面，提供图片组转2D纹理数组纹理的功能
"""

import os
import sys
from typing import List
from PySide6.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout, 
                               QHBoxLayout, QLabel, QLineEdit, QPushButton, 
                               QListWidget, QFileDialog, QMessageBox, QFrame)
from PySide6.QtCore import Qt, QSize
from PySide6.QtGui import QPixmap
from PIL import Image
from sy_data_util import SYDataUtil


class MainFrame(QMainWindow):
    """主窗口类"""
    
    def __init__(self):
        """
        初始化主窗口
        """
        super().__init__()
        self.v_src = []  # 源图像文件路径列表
        self.init_ui()
        self.setup_connections()
    
    def init_ui(self) -> None:
        """
        初始化用户界面
        """
        self.setWindowTitle("图片组转为2D纹理数组纹理")
        self.setFixedSize(460, 330)
        
        # 创建中央部件
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        
        # 创建主布局
        main_layout = QVBoxLayout(central_widget)
        main_layout.setSpacing(10)
        main_layout.setContentsMargins(10, 10, 10, 10)
        
        # 纹理输出目录
        dst_layout = QHBoxLayout()
        dst_label = QLabel("纹理输出目录:")
        dst_label.setFixedWidth(80)
        self.dst_line_edit = QLineEdit()
        self.dst_button = QPushButton("选择")
        self.dst_button.setFixedSize(60, 25)
        
        dst_layout.addWidget(dst_label)
        dst_layout.addWidget(self.dst_line_edit)
        dst_layout.addWidget(self.dst_button)
        main_layout.addLayout(dst_layout)
        
        # 纹理文件名称
        name_layout = QHBoxLayout()
        name_label = QLabel("纹理文件名称:")
        name_label.setFixedWidth(80)
        self.name_line_edit = QLineEdit()
        
        name_layout.addWidget(name_label)
        name_layout.addWidget(self.name_line_edit)
        main_layout.addLayout(name_layout)
        
        # 原图文件列表
        src_layout = QHBoxLayout()
        src_label = QLabel("原图文件列表:")
        src_label.setFixedWidth(80)
        src_label.setAlignment(Qt.AlignTop)
        
        # 左侧文件列表
        left_layout = QVBoxLayout()
        self.src_list_widget = QListWidget()
        self.src_list_widget.setFixedSize(280, 200)
        left_layout.addWidget(self.src_list_widget)
        
        # 右侧按钮
        right_layout = QVBoxLayout()
        self.add_button = QPushButton("添加")
        self.add_button.setFixedSize(60, 25)
        self.del_button = QPushButton("删除")
        self.del_button.setFixedSize(60, 25)
        
        right_layout.addWidget(self.add_button)
        right_layout.addWidget(self.del_button)
        right_layout.addStretch()
        
        src_layout.addWidget(src_label)
        src_layout.addLayout(left_layout)
        src_layout.addLayout(right_layout)
        main_layout.addLayout(src_layout)
        
        # 生成按钮
        self.generate_button = QPushButton("生成2D纹理数组纹理")
        self.generate_button.setFixedSize(160, 25)
        main_layout.addWidget(self.generate_button)
        
        # 添加弹性空间
        main_layout.addStretch()
    
    def setup_connections(self) -> None:
        """
        设置信号连接
        """
        self.dst_button.clicked.connect(self.select_dst_directory)
        self.add_button.clicked.connect(self.add_source_file)
        self.del_button.clicked.connect(self.delete_source_file)
        self.generate_button.clicked.connect(self.generate_texture_array)
    
    def select_dst_directory(self) -> None:
        """
        选择输出目录
        """
        directory = QFileDialog.getExistingDirectory(
            self, 
            "选择输出目录", 
            self.get_current_path("texDst")
        )
        if directory:
            self.dst_line_edit.setText(directory)
    
    def add_source_file(self) -> None:
        """
        添加源图像文件
        """
        file_path, _ = QFileDialog.getOpenFileName(
            self,
            "选择图像文件",
            self.get_current_path("picSrc"),
            "图像文件 (*.png *.jpg *.jpeg *.bmp *.gif *.tiff)"
        )
        
        if file_path:
            # 检查图像尺寸是否一致
            if self.is_same_size(file_path):
                self.v_src.append(file_path)
                self.update_source_list()
            else:
                QMessageBox.warning(
                    self, 
                    "提示", 
                    "纹理数组中所有的图必须尺寸一致！"
                )
    
    def delete_source_file(self) -> None:
        """
        删除选中的源文件
        """
        current_row = self.src_list_widget.currentRow()
        if current_row >= 0:
            del self.v_src[current_row]
            self.update_source_list()
    
    def generate_texture_array(self) -> None:
        """
        生成2D纹理数组纹理
        """
        self.generate_button.setEnabled(False)
        
        try:
            # 验证输入
            if not self.validate_inputs():
                return
            
            # 检查所有源文件是否存在
            if not self.is_all_exists():
                return
            
            # 生成输出路径
            output_path = os.path.join(
                self.dst_line_edit.text().strip(),
                self.name_line_edit.text().strip()
            )
            
            # 执行转换
            success = SYDataUtil.change(self.v_src, output_path)
            
            if success:
                QMessageBox.information(self, "提示", "转换成功！")
            else:
                QMessageBox.critical(self, "提示", "转换失败！")
                
        finally:
            self.generate_button.setEnabled(True)
    
    def validate_inputs(self) -> bool:
        """
        验证输入参数
        
        Returns:
            bool: 验证是否通过
        """
        # 检查输出目录
        dst_path = self.dst_line_edit.text().strip()
        if not dst_path or not os.path.exists(dst_path):
            QMessageBox.warning(self, "提示", "目标路径不存在！")
            return False
        
        # 检查文件名
        if not self.name_line_edit.text().strip():
            QMessageBox.warning(self, "提示", "请输入纹理文件名称！")
            return False
        
        # 检查源文件列表
        if not self.v_src:
            QMessageBox.warning(self, "提示", "纹理数组中至少有一幅纹理！")
            return False
        
        return True
    
    def is_all_exists(self) -> bool:
        """
        检查所有源文件是否存在
        
        Returns:
            bool: 所有文件是否存在
        """
        for src_path in self.v_src:
            if not os.path.exists(src_path):
                QMessageBox.warning(
                    self, 
                    "提示", 
                    f"原图文件[{src_path}]不存在！"
                )
                return False
        return True
    
    def is_same_size(self, new_file_path: str) -> bool:
        """
        检查新添加的文件尺寸是否与现有文件一致
        
        Args:
            new_file_path (str): 新文件路径
            
        Returns:
            bool: 尺寸是否一致
        """
        try:
            # 获取新图像的尺寸
            new_image = Image.open(new_file_path)
            new_width, new_height = new_image.size
            
            # 如果是第一张图片，直接返回True
            if not self.v_src:
                return True
            
            # 获取第一张图片的尺寸作为参考
            first_image = Image.open(self.v_src[0])
            first_width, first_height = first_image.size
            
            print(f"新图片尺寸: {new_width}x{new_height}")
            print(f"参考尺寸: {first_width}x{first_height}")
            
            return new_width == first_width and new_height == first_height
            
        except Exception as e:
            print(f"检查图片尺寸时出错: {e}")
            return False
    
    def update_source_list(self) -> None:
        """
        更新源文件列表显示
        """
        self.src_list_widget.clear()
        for src_path in self.v_src:
            # 只显示文件名，不显示完整路径
            filename = os.path.basename(src_path)
            self.src_list_widget.addItem(filename)
    
    def get_current_path(self, path_name: str) -> str:
        """
        获取当前路径
        
        Args:
            path_name (str): 路径名称
            
        Returns:
            str: 当前路径
        """
        if os.path.exists(path_name):
            return path_name
        return os.getcwd()


def main():
    """
    主函数
    """
    app = QApplication(sys.argv)
    window = MainFrame()
    window.show()
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
