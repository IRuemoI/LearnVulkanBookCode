"""
3D棋盘纹理生成器主界面
使用PySide6构建的图形用户界面
"""

import os
import sys
from PySide6.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout, 
                               QHBoxLayout, QLabel, QLineEdit, QSpinBox, 
                               QPushButton, QFileDialog, QMessageBox, QColorDialog)
from PySide6.QtCore import Qt
from PySide6.QtGui import QColor, QPalette
from WhiteAndRed import WhiteAndRed


class MainFrame(QMainWindow):
    """3D棋盘纹理生成器主窗口类"""
    
    def __init__(self):
        """
        初始化主窗口
        """
        super().__init__()
        self.setWindowTitle("生成3D棋盘纹理")
        self.setFixedSize(460, 120)
        
        # 初始化界面组件
        self._init_ui()
        self._setup_connections()
        
        # 显示窗口
        self.show()
    
    def _init_ui(self) -> None:
        """
        初始化用户界面组件
        """
        # 创建中央widget
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        
        # 创建主布局
        main_layout = QVBoxLayout(central_widget)
        main_layout.setSpacing(10)
        main_layout.setContentsMargins(10, 10, 10, 10)
        
        # 第一行：文件名、分段数、颜色选择
        first_row = QHBoxLayout()
        
        # 纹理文件名
        file_label = QLabel("纹理文件名:")
        file_label.setFixedWidth(80)
        self.file_name_edit = QLineEdit()
        self.file_name_edit.setFixedWidth(100)
        first_row.addWidget(file_label)
        first_row.addWidget(self.file_name_edit)
        
        # 分段数
        count_label = QLabel("分段数:")
        count_label.setFixedWidth(50)
        self.count_spinbox = QSpinBox()
        self.count_spinbox.setRange(2, 32)
        self.count_spinbox.setValue(4)
        self.count_spinbox.setFixedWidth(50)
        first_row.addWidget(count_label)
        first_row.addWidget(self.count_spinbox)
        
        # 棋盘颜色1
        self.color_a_label = QLabel("棋盘颜色1")
        self.color_a_label.setAlignment(Qt.AlignCenter)
        self.color_a_label.setFixedSize(73, 20)
        self.color_a_label.setStyleSheet("background-color: red; color: white; border: 1px solid black;")
        first_row.addWidget(self.color_a_label)
        
        # 棋盘颜色2
        self.color_b_label = QLabel("棋盘颜色2")
        self.color_b_label.setAlignment(Qt.AlignCenter)
        self.color_b_label.setFixedSize(73, 20)
        self.color_b_label.setStyleSheet("background-color: white; color: black; border: 1px solid black;")
        first_row.addWidget(self.color_b_label)
        
        main_layout.addLayout(first_row)
        
        # 第二行：纹理目录选择
        second_row = QHBoxLayout()
        
        dir_label = QLabel("纹理目录:")
        dir_label.setFixedWidth(60)
        self.dir_edit = QLineEdit()
        self.dir_edit.setFixedWidth(300)
        self.dir_button = QPushButton("选择")
        self.dir_button.setFixedWidth(60)
        second_row.addWidget(dir_label)
        second_row.addWidget(self.dir_edit)
        second_row.addWidget(self.dir_button)
        
        main_layout.addLayout(second_row)
        
        # 第三行：生成按钮
        self.generate_button = QPushButton("生成3D棋盘纹理")
        self.generate_button.setFixedSize(140, 20)
        main_layout.addWidget(self.generate_button)
        
        # 添加弹性空间
        main_layout.addStretch()
    
    def _setup_connections(self) -> None:
        """
        设置信号和槽的连接
        """
        # 目录选择按钮
        self.dir_button.clicked.connect(self._select_directory)
        
        # 颜色选择标签
        self.color_a_label.mousePressEvent = self._select_color_a
        self.color_b_label.mousePressEvent = self._select_color_b
        
        # 生成按钮
        self.generate_button.clicked.connect(self._generate_texture)
    
    def _select_directory(self) -> None:
        """
        选择输出目录
        """
        # 设置默认目录
        default_dir = os.path.join(os.getcwd(), "TexData3D")
        if not os.path.exists(default_dir):
            default_dir = os.getcwd()
        
        # 打开目录选择对话框
        directory = QFileDialog.getExistingDirectory(
            self, 
            "选择纹理目录", 
            default_dir,
            QFileDialog.ShowDirsOnly
        )
        
        if directory:
            self.dir_edit.setText(directory)
    
    def _select_color_a(self, event) -> None:
        """
        选择棋盘颜色1
        
        Args:
            event: 鼠标点击事件
        """
        current_color = self._get_label_color(self.color_a_label)
        color = QColorDialog.getColor(current_color, self, "请选择颜色1")
        
        if color.isValid():
            self._set_label_color(self.color_a_label, color)
    
    def _select_color_b(self, event) -> None:
        """
        选择棋盘颜色2
        
        Args:
            event: 鼠标点击事件
        """
        current_color = self._get_label_color(self.color_b_label)
        color = QColorDialog.getColor(current_color, self, "请选择颜色2")
        
        if color.isValid():
            self._set_label_color(self.color_b_label, color)
    
    def _get_label_color(self, label: QLabel) -> QColor:
        """
        从标签的样式表中获取颜色
        
        Args:
            label: 要获取颜色的标签
            
        Returns:
            QColor: 标签的背景颜色
        """
        style = label.styleSheet()
        # 从样式表中提取背景颜色
        if "background-color:" in style:
            color_str = style.split("background-color:")[1].split(";")[0].strip()
            return QColor(color_str)
        return QColor(255, 255, 255)  # 默认白色
    
    def _set_label_color(self, label: QLabel, color: QColor) -> None:
        """
        设置标签的颜色
        
        Args:
            label: 要设置颜色的标签
            color: 要设置的颜色
        """
        # 计算对比色作为文字颜色
        text_color = "white" if (color.red() + color.green() + color.blue()) < 384 else "black"
        
        # 设置样式
        label.setStyleSheet(f"background-color: {color.name()}; color: {text_color}; border: 1px solid black;")
    
    def _generate_texture(self) -> None:
        """
        生成3D棋盘纹理
        """
        # 禁用生成按钮
        self.generate_button.setEnabled(False)
        
        try:
            # 验证文件名
            file_name = self.file_name_edit.text().strip()
            if not file_name:
                QMessageBox.warning(self, "提示", "必须给出文件名！")
                self.generate_button.setEnabled(True)
                return
            
            # 验证目录
            output_dir = self.dir_edit.text().strip()
            if not output_dir or not os.path.exists(output_dir):
                QMessageBox.warning(self, "提示", "目标路径不存在！")
                self.generate_button.setEnabled(True)
                return
            
            # 获取参数
            span = self.count_spinbox.value()
            color_a = self._color_to_int(self._get_label_color(self.color_a_label))
            color_b = self._color_to_int(self._get_label_color(self.color_b_label))
            
            # 构建输出路径
            output_path = os.path.join(output_dir, file_name)
            
            # 生成3D纹理
            success = WhiteAndRed.gen_3d_texture(span, color_a, color_b, output_path)
            
            if success:
                QMessageBox.information(self, "提示", "生成成功！")
            else:
                QMessageBox.critical(self, "提示", "生成失败！")
                
        except Exception as e:
            QMessageBox.critical(self, "错误", f"生成过程中发生错误: {str(e)}")
        finally:
            # 重新启用生成按钮
            self.generate_button.setEnabled(True)
    
    def _color_to_int(self, color: QColor) -> int:
        """
        将QColor转换为ARGB整数格式
        
        Args:
            color: 要转换的颜色
            
        Returns:
            int: ARGB格式的整数
        """
        # 打印颜色值用于调试
        print(f"颜色值: R={color.red()}, G={color.green()}, B={color.blue()}")
        
        # 转换为ARGB格式 (Alpha=255, Red, Green, Blue)
        return (255 << 24) | (color.red() << 16) | (color.green() << 8) | color.blue()


def main():
    """
    主函数，程序入口点
    """
    app = QApplication(sys.argv)
    window = MainFrame()
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
