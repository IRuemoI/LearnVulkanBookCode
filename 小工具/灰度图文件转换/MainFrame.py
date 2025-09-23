"""
灰度图转换工具主界面
使用PySide6构建的图形用户界面，用于将灰度图转换为bnhdt格式文件
"""

import sys
import os
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
        self.init_ui()
        self.setup_connections()
        
    def init_ui(self) -> None:
        """
        初始化用户界面
        """
        # 设置窗口属性
        self.setWindowTitle("灰度图转为bnhdt文件")
        self.setFixedSize(460, 120)
        
        # 创建中央部件
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        
        # 创建主布局
        main_layout = QVBoxLayout(central_widget)
        main_layout.setSpacing(10)
        main_layout.setContentsMargins(10, 10, 10, 10)
        
        # 创建源文件选择区域
        src_layout = self.create_file_selection_layout("原图文件", "选择")
        main_layout.addLayout(src_layout)
        
        # 创建目标路径选择区域
        dst_layout = self.create_file_selection_layout("目标路径", "选择")
        main_layout.addLayout(dst_layout)
        
        # 创建转换按钮
        self.convert_button = QPushButton("转换")
        self.convert_button.setFixedSize(60, 30)
        self.convert_button.setFont(QFont("Arial", 10))
        main_layout.addWidget(self.convert_button)
        
        # 设置样式
        self.setStyleSheet("""
            QMainWindow {
                background-color: #f0f0f0;
            }
            QLabel {
                font-size: 12px;
                color: #333;
            }
            QLineEdit {
                padding: 5px;
                border: 1px solid #ccc;
                border-radius: 3px;
                font-size: 12px;
            }
            QPushButton {
                padding: 5px 10px;
                border: 1px solid #007acc;
                border-radius: 3px;
                background-color: #007acc;
                color: white;
                font-size: 12px;
            }
            QPushButton:hover {
                background-color: #005a9e;
            }
            QPushButton:disabled {
                background-color: #ccc;
                border-color: #ccc;
            }
        """)
    
    def create_file_selection_layout(self, label_text: str, button_text: str) -> QHBoxLayout:
        """
        创建文件选择布局
        
        Args:
            label_text (str): 标签文本
            button_text (str): 按钮文本
            
        Returns:
            QHBoxLayout: 水平布局对象
        """
        layout = QHBoxLayout()
        layout.setSpacing(10)
        
        # 创建标签
        label = QLabel(label_text)
        label.setFixedWidth(60)
        label.setAlignment(Qt.AlignLeft | Qt.AlignVCenter)
        layout.addWidget(label)
        
        # 创建文本输入框
        line_edit = QLineEdit()
        line_edit.setPlaceholderText(f"请选择{label_text}")
        layout.addWidget(line_edit)
        
        # 创建选择按钮
        button = QPushButton(button_text)
        button.setFixedSize(60, 30)
        layout.addWidget(button)
        
        # 根据标签文本设置对应的属性
        if label_text == "原图文件":
            self.src_line_edit = line_edit
            self.src_button = button
        else:
            self.dst_line_edit = line_edit
            self.dst_button = button
        
        return layout
    
    def setup_connections(self) -> None:
        """
        设置信号连接
        """
        self.src_button.clicked.connect(self.select_source_file)
        self.dst_button.clicked.connect(self.select_destination_path)
        self.convert_button.clicked.connect(self.convert_file)
    
    def select_source_file(self) -> None:
        """
        选择源图像文件
        """
        # 设置文件对话框
        file_dialog = QFileDialog(self)
        file_dialog.setWindowTitle("选择源图像文件")
        file_dialog.setNameFilter("图像文件 (*.png *.jpg *.jpeg *.bmp *.gif *.tiff)")
        file_dialog.setFileMode(QFileDialog.ExistingFile)
        
        # 设置初始目录
        current_dir = self.get_current_path("picSrc")
        if os.path.exists(current_dir):
            file_dialog.setDirectory(current_dir)
        
        # 显示对话框并获取结果
        if file_dialog.exec():
            selected_files = file_dialog.selectedFiles()
            if selected_files:
                self.src_line_edit.setText(selected_files[0])
    
    def select_destination_path(self) -> None:
        """
        选择目标路径
        """
        # 设置文件夹对话框
        file_dialog = QFileDialog(self)
        file_dialog.setWindowTitle("选择目标路径")
        file_dialog.setFileMode(QFileDialog.Directory)
        
        # 设置初始目录
        current_dir = self.get_current_path("hdtDst")
        if os.path.exists(current_dir):
            file_dialog.setDirectory(current_dir)
        
        # 显示对话框并获取结果
        if file_dialog.exec():
            selected_dirs = file_dialog.selectedFiles()
            if selected_dirs:
                self.dst_line_edit.setText(selected_dirs[0])
    
    def convert_file(self) -> None:
        """
        执行文件转换
        """
        # 禁用转换按钮
        self.convert_button.setEnabled(False)
        
        try:
            # 获取源文件和目标路径
            src_path = self.src_line_edit.text().strip()
            dst_path = self.dst_line_edit.text().strip()
            
            # 验证源文件
            if not src_path:
                self.show_message("请选择源图像文件！", "提示", QMessageBox.Warning)
                return
            
            if not os.path.exists(src_path):
                self.show_message("原图文件不存在！", "提示", QMessageBox.Warning)
                return
            
            # 验证目标路径
            if not dst_path:
                self.show_message("请选择目标路径！", "提示", QMessageBox.Warning)
                return
            
            if not os.path.exists(dst_path):
                self.show_message("目标路径不存在！", "提示", QMessageBox.Warning)
                return
            
            # 生成目标文件路径
            src_filename = os.path.splitext(os.path.basename(src_path))[0]
            dst_file_path = os.path.join(dst_path, f"{src_filename}.bnhdt")
            
            print(f"目标文件路径: {dst_file_path}")
            
            # 执行转换
            success = SYDataUtil.change(src_path, dst_file_path)
            
            if success:
                self.show_message("转换成功！", "提示", QMessageBox.Information)
            else:
                self.show_message("转换失败！", "提示", QMessageBox.Critical)
                
        except Exception as e:
            self.show_message(f"转换过程中发生错误: {str(e)}", "错误", QMessageBox.Critical)
        finally:
            # 重新启用转换按钮
            self.convert_button.setEnabled(True)
    
    def get_current_path(self, folder_name: str) -> str:
        """
        获取当前工作目录下的指定文件夹路径
        
        Args:
            folder_name (str): 文件夹名称
            
        Returns:
            str: 文件夹的完整路径
        """
        current_dir = os.getcwd()
        return os.path.join(current_dir, folder_name)
    
    def show_message(self, message: str, title: str, icon: QMessageBox.Icon) -> None:
        """
        显示消息对话框
        
        Args:
            message (str): 消息内容
            title (str): 对话框标题
            icon (QMessageBox.Icon): 图标类型
        """
        msg_box = QMessageBox(self)
        msg_box.setWindowTitle(title)
        msg_box.setText(message)
        msg_box.setIcon(icon)
        msg_box.exec()


def main():
    """
    主函数 - 程序入口点
    """
    app = QApplication(sys.argv)
    
    # 设置应用程序属性
    app.setApplicationName("灰度图转换工具")
    app.setApplicationVersion("1.0")
    
    # 创建并显示主窗口
    window = MainFrame()
    window.show()
    
    # 运行应用程序
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
