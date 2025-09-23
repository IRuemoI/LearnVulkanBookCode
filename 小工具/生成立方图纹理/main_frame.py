"""
立方图纹理生成工具主界面
使用PySide6创建图形用户界面
"""

import os
import sys
from typing import List, Optional
from PySide6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, 
    QLabel, QLineEdit, QPushButton, QListWidget, QListWidgetItem,
    QFileDialog, QMessageBox, QGroupBox, QGridLayout, QProgressBar,
    QTextEdit, QSplitter
)
from PySide6.QtCore import Qt, QThread, Signal, QTimer
from PySide6.QtGui import QFont, QPixmap, QIcon
import sy_data_util


class ImageProcessingThread(QThread):
    """图像处理线程类"""
    
    # 定义信号
    progress_updated = Signal(int)  # 进度更新信号
    status_updated = Signal(str)    # 状态更新信号
    finished_signal = Signal(bool)  # 完成信号
    
    def __init__(self, image_paths: List[str], output_path: str):
        """
        初始化处理线程
        
        Args:
            image_paths (List[str]): 图像文件路径列表
            output_path (str): 输出文件路径
        """
        super().__init__()
        self.image_paths = image_paths
        self.output_path = output_path
    
    def run(self):
        """
        执行图像处理任务
        """
        try:
            self.status_updated.emit("开始处理图像...")
            self.progress_updated.emit(10)
            
            # 验证图像文件
            self.status_updated.emit("验证图像文件...")
            for i, path in enumerate(self.image_paths):
                if not os.path.exists(path):
                    self.status_updated.emit(f"错误: 文件不存在 - {path}")
                    self.finished_signal.emit(False)
                    return
                self.progress_updated.emit(20 + i * 10)
            
            # 生成立方图纹理
            self.status_updated.emit("生成立方图纹理...")
            success = sy_data_util.change(self.image_paths, self.output_path)
            
            if success:
                self.status_updated.emit("转换成功！")
                self.progress_updated.emit(100)
                self.finished_signal.emit(True)
            else:
                self.status_updated.emit("转换失败！")
                self.finished_signal.emit(False)
                
        except Exception as e:
            self.status_updated.emit(f"处理过程中发生错误: {str(e)}")
            self.finished_signal.emit(False)


class MainFrame(QMainWindow):
    """主窗口类"""
    
    def __init__(self):
        """
        初始化主窗口
        """
        super().__init__()
        self.image_paths = []  # 存储图像文件路径
        self.current_directory = os.getcwd()  # 当前工作目录
        self.processing_thread = None  # 处理线程
        
        self.init_ui()
        self.setup_connections()
    
    def init_ui(self):
        """
        初始化用户界面
        """
        self.setWindowTitle("生成立方图纹理")
        self.setGeometry(100, 100, 600, 500)
        
        # 创建中央部件
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        
        # 创建主布局
        main_layout = QVBoxLayout(central_widget)
        
        # 创建输出设置组
        output_group = self.create_output_group()
        main_layout.addWidget(output_group)
        
        # 创建图像列表组
        image_group = self.create_image_group()
        main_layout.addWidget(image_group)
        
        # 创建控制按钮组
        control_group = self.create_control_group()
        main_layout.addWidget(control_group)
        
        # 创建进度和状态组
        status_group = self.create_status_group()
        main_layout.addWidget(status_group)
    
    def create_output_group(self) -> QGroupBox:
        """
        创建输出设置组
        
        Returns:
            QGroupBox: 输出设置组
        """
        group = QGroupBox("输出设置")
        layout = QGridLayout(group)
        
        # 输出目录
        layout.addWidget(QLabel("纹理输出目录:"), 0, 0)
        self.dst_path_edit = QLineEdit()
        self.dst_path_edit.setPlaceholderText("选择输出目录...")
        layout.addWidget(self.dst_path_edit, 0, 1)
        
        self.dst_browse_btn = QPushButton("选择")
        self.dst_browse_btn.setMaximumWidth(80)
        layout.addWidget(self.dst_browse_btn, 0, 2)
        
        # 纹理文件名称
        layout.addWidget(QLabel("纹理文件名称:"), 1, 0)
        self.tex_name_edit = QLineEdit()
        self.tex_name_edit.setPlaceholderText("输入纹理文件名称...")
        layout.addWidget(self.tex_name_edit, 1, 1, 1, 2)
        
        return group
    
    def create_image_group(self) -> QGroupBox:
        """
        创建图像列表组
        
        Returns:
            QGroupBox: 图像列表组
        """
        group = QGroupBox("原图文件列表")
        layout = QVBoxLayout(group)
        
        # 说明标签
        info_label = QLabel(
            "请注意：图片一共6幅，顺序为：左、右、上、下、后、前！"
        )
        info_label.setStyleSheet("color: #666; font-style: italic;")
        layout.addWidget(info_label)
        
        # 图像列表
        self.image_list = QListWidget()
        self.image_list.setMaximumHeight(150)
        layout.addWidget(self.image_list)
        
        # 按钮布局
        button_layout = QHBoxLayout()
        
        self.add_btn = QPushButton("添加图像")
        self.add_btn.setMaximumWidth(100)
        button_layout.addWidget(self.add_btn)
        
        self.remove_btn = QPushButton("删除选中")
        self.remove_btn.setMaximumWidth(100)
        button_layout.addWidget(self.remove_btn)
        
        self.clear_btn = QPushButton("清空列表")
        self.clear_btn.setMaximumWidth(100)
        button_layout.addWidget(self.clear_btn)
        
        button_layout.addStretch()
        layout.addLayout(button_layout)
        
        return group
    
    def create_control_group(self) -> QGroupBox:
        """
        创建控制按钮组
        
        Returns:
            QGroupBox: 控制按钮组
        """
        group = QGroupBox("操作控制")
        layout = QHBoxLayout(group)
        
        self.generate_btn = QPushButton("生成立方图纹理")
        self.generate_btn.setMinimumHeight(40)
        self.generate_btn.setStyleSheet("""
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
            QPushButton:disabled {
                background-color: #cccccc;
                color: #666666;
            }
        """)
        layout.addWidget(self.generate_btn)
        
        return group
    
    def create_status_group(self) -> QGroupBox:
        """
        创建状态显示组
        
        Returns:
            QGroupBox: 状态显示组
        """
        group = QGroupBox("处理状态")
        layout = QVBoxLayout(group)
        
        # 进度条
        self.progress_bar = QProgressBar()
        self.progress_bar.setVisible(False)
        layout.addWidget(self.progress_bar)
        
        # 状态文本
        self.status_text = QTextEdit()
        self.status_text.setMaximumHeight(100)
        self.status_text.setReadOnly(True)
        self.status_text.setPlaceholderText("状态信息将在这里显示...")
        layout.addWidget(self.status_text)
        
        return group
    
    def setup_connections(self):
        """
        设置信号连接
        """
        # 按钮连接
        self.dst_browse_btn.clicked.connect(self.browse_output_directory)
        self.add_btn.clicked.connect(self.add_images)
        self.remove_btn.clicked.connect(self.remove_selected_image)
        self.clear_btn.clicked.connect(self.clear_image_list)
        self.generate_btn.clicked.connect(self.generate_cubemap)
    
    def browse_output_directory(self):
        """
        浏览输出目录
        """
        directory = QFileDialog.getExistingDirectory(
            self, "选择输出目录", self.current_directory
        )
        if directory:
            self.dst_path_edit.setText(directory)
            self.current_directory = directory
    
    def add_images(self):
        """
        添加图像文件
        """
        file_paths, _ = QFileDialog.getOpenFileNames(
            self, "选择图像文件", self.current_directory,
            "图像文件 (*.png *.jpg *.jpeg *.bmp *.tiff);;所有文件 (*)"
        )
        
        if not file_paths:
            return
        
        # 检查图像尺寸一致性
        if self.image_paths:
            try:
                first_width, first_height = sy_data_util.get_image_info(self.image_paths[0])
                for path in file_paths:
                    width, height = sy_data_util.get_image_info(path)
                    if width != first_width or height != first_height:
                        QMessageBox.warning(
                            self, "尺寸不一致", 
                            f"图像 {os.path.basename(path)} 的尺寸为 {width}x{height}，"
                            f"与第一张图像的尺寸 {first_width}x{first_height} 不一致！"
                        )
                        return
            except Exception as e:
                QMessageBox.critical(self, "错误", f"检查图像尺寸时发生错误: {str(e)}")
                return
        
        # 添加图像到列表
        for path in file_paths:
            if path not in self.image_paths:
                self.image_paths.append(path)
                item = QListWidgetItem(os.path.basename(path))
                item.setToolTip(path)
                self.image_list.addItem(item)
        
        self.update_image_list_display()
    
    def remove_selected_image(self):
        """
        删除选中的图像
        """
        current_row = self.image_list.currentRow()
        if current_row >= 0:
            self.image_paths.pop(current_row)
            self.image_list.takeItem(current_row)
            self.update_image_list_display()
    
    def clear_image_list(self):
        """
        清空图像列表
        """
        self.image_paths.clear()
        self.image_list.clear()
        self.update_image_list_display()
    
    def update_image_list_display(self):
        """
        更新图像列表显示
        """
        # 更新列表项显示
        for i in range(self.image_list.count()):
            item = self.image_list.item(i)
            if i < len(self.image_paths):
                filename = os.path.basename(self.image_paths[i])
                item.setText(f"{i+1}. {filename}")
                item.setToolTip(self.image_paths[i])
        
        # 更新按钮状态
        self.remove_btn.setEnabled(len(self.image_paths) > 0)
        self.clear_btn.setEnabled(len(self.image_paths) > 0)
        self.generate_btn.setEnabled(len(self.image_paths) == 6)
    
    def generate_cubemap(self):
        """
        生成立方图纹理
        """
        # 验证输入
        if not self.dst_path_edit.text().strip():
            QMessageBox.warning(self, "提示", "请选择输出目录！")
            return
        
        if not self.tex_name_edit.text().strip():
            QMessageBox.warning(self, "提示", "请输入纹理文件名称！")
            return
        
        if len(self.image_paths) != 6:
            QMessageBox.warning(self, "提示", "立方图必须由六幅纹理构成！")
            return
        
        # 检查输出目录是否存在
        if not os.path.exists(self.dst_path_edit.text()):
            QMessageBox.warning(self, "提示", "目标路径不存在！")
            return
        
        # 构建输出路径
        output_path = os.path.join(
            self.dst_path_edit.text().strip(),
            self.tex_name_edit.text().strip()
        )
        
        # 禁用生成按钮
        self.generate_btn.setEnabled(False)
        self.progress_bar.setVisible(True)
        self.progress_bar.setValue(0)
        self.status_text.clear()
        
        # 创建并启动处理线程
        self.processing_thread = ImageProcessingThread(self.image_paths, output_path)
        self.processing_thread.progress_updated.connect(self.progress_bar.setValue)
        self.processing_thread.status_updated.connect(self.update_status)
        self.processing_thread.finished_signal.connect(self.on_processing_finished)
        self.processing_thread.start()
    
    def update_status(self, message: str):
        """
        更新状态信息
        
        Args:
            message (str): 状态消息
        """
        self.status_text.append(f"[{QTimer().currentTime().toString()}] {message}")
        self.status_text.ensureCursorVisible()
    
    def on_processing_finished(self, success: bool):
        """
        处理完成回调
        
        Args:
            success (bool): 是否成功
        """
        self.progress_bar.setVisible(False)
        self.generate_btn.setEnabled(True)
        
        if success:
            QMessageBox.information(self, "成功", "立方图纹理生成成功！")
        else:
            QMessageBox.critical(self, "失败", "立方图纹理生成失败！")


def main():
    """
    主函数
    """
    app = QApplication(sys.argv)
    
    # 设置应用程序信息
    app.setApplicationName("立方图纹理生成工具")
    app.setApplicationVersion("1.0.0")
    app.setOrganizationName("SY Tools")
    
    # 创建主窗口
    window = MainFrame()
    window.show()
    
    # 运行应用程序
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
