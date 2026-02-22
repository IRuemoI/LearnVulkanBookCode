"""
高度域灰度图转换为法向量纹理图工具
使用PySide6构建图形界面，实现高度图到法向量图的转换
"""

import os
import sys
from typing import Optional, Tuple
from PIL import Image
import numpy as np

from PySide6.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout, 
                               QHBoxLayout, QLabel, QFileDialog, QMessageBox, 
                               QSplitter, QScrollArea, QPushButton)
from PySide6.QtCore import Qt
from PySide6.QtGui import QPixmap, QImage

from vector_util import get_cross_product, vector_normal


class NormalMapUtil(QMainWindow):
    """法向量纹理图转换工具主窗口"""
    
    def __init__(self):
        super().__init__()
        self.source_image: Optional[Image.Image] = None
        self.source_pixmap: Optional[QPixmap] = None
        self.target_pixmap: Optional[QPixmap] = None
        self.file_name: str = ""
        
        self.init_ui()
        self.load_source_image()
    
    def init_ui(self) -> None:
        """
        初始化用户界面
        创建主窗口布局和控件
        """
        self.setWindowTitle("高度域灰度图转换成法向量纹理图工具")
        self.setGeometry(100, 100, 1000, 600)
        
        # 创建中央部件
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        
        # 创建主布局
        main_layout = QVBoxLayout(central_widget)
        
        # 创建按钮布局
        button_layout = QHBoxLayout()
        self.load_button = QPushButton("选择源图片")
        self.load_button.clicked.connect(self.load_source_image)
        self.process_button = QPushButton("生成法向量图")
        self.process_button.clicked.connect(self.process_image)
        self.save_button = QPushButton("保存结果")
        self.save_button.clicked.connect(self.save_result)
        
        button_layout.addWidget(self.load_button)
        button_layout.addWidget(self.process_button)
        button_layout.addWidget(self.save_button)
        button_layout.addStretch()
        
        # 创建分割器
        self.splitter = QSplitter(Qt.Horizontal)
        
        # 左侧：源图片显示区域
        self.source_scroll = QScrollArea()
        self.source_label = QLabel()
        self.source_label.setAlignment(Qt.AlignCenter)
        self.source_label.setMinimumSize(400, 300)
        self.source_scroll.setWidget(self.source_label)
        self.source_scroll.setWidgetResizable(True)
        
        # 右侧：结果图片显示区域
        self.target_scroll = QScrollArea()
        self.target_label = QLabel()
        self.target_label.setAlignment(Qt.AlignCenter)
        self.target_label.setMinimumSize(400, 300)
        self.target_scroll.setWidget(self.target_label)
        self.target_scroll.setWidgetResizable(True)
        
        # 添加标签
        source_title = QLabel("源图片（高度图）")
        source_title.setAlignment(Qt.AlignCenter)
        target_title = QLabel("法向量图")
        target_title.setAlignment(Qt.AlignCenter)
        
        # 左侧布局
        left_layout = QVBoxLayout()
        left_layout.addWidget(source_title)
        left_layout.addWidget(self.source_scroll)
        left_widget = QWidget()
        left_widget.setLayout(left_layout)
        
        # 右侧布局
        right_layout = QVBoxLayout()
        right_layout.addWidget(target_title)
        right_layout.addWidget(self.target_scroll)
        right_widget = QWidget()
        right_widget.setLayout(right_layout)
        
        # 添加到分割器
        self.splitter.addWidget(left_widget)
        self.splitter.addWidget(right_widget)
        self.splitter.setSizes([500, 500])
        
        # 添加到主布局
        main_layout.addLayout(button_layout)
        main_layout.addWidget(self.splitter)
    
    def load_source_image(self) -> None:
        """
        加载源图片文件
        打开文件对话框让用户选择高度图
        """
        # 设置默认目录
        default_dir = os.path.join(os.getcwd(), "srcPic")
        if not os.path.exists(default_dir):
            default_dir = os.getcwd()
        
        file_path, _ = QFileDialog.getOpenFileName(
            self, 
            "选择高度图", 
            default_dir,
            "图片文件 (*.png *.jpg *.jpeg *.bmp *.tiff)"
        )
        
        if file_path:
            try:
                # 加载图片
                self.source_image = Image.open(file_path)
                self.file_name = os.path.basename(file_path)
                
                # 转换为QPixmap并显示
                self.source_pixmap = self.pil_to_qpixmap(self.source_image)
                self.source_label.setPixmap(self.source_pixmap)
                
                # 清空之前的结果
                self.target_label.clear()
                self.target_pixmap = None
                
                QMessageBox.information(self, "成功", f"已加载图片: {self.file_name}")
                
            except Exception as e:
                QMessageBox.critical(self, "错误", f"加载图片失败: {str(e)}")
    
    def process_image(self) -> None:
        """
        处理图片，将高度图转换为法向量图
        使用Sobel算子计算梯度，然后生成法向量
        """
        if self.source_image is None:
            QMessageBox.warning(self, "警告", "请先选择源图片")
            return
        
        try:
            # 转换为灰度图
            if self.source_image.mode != 'L':
                gray_image = self.source_image.convert('L')
            else:
                gray_image = self.source_image
            
            # 转换为numpy数组
            height_array = np.array(gray_image, dtype=np.float32) / 255.0
            
            # 获取图片尺寸
            height, width = height_array.shape
            
            # 创建结果数组
            normal_array = np.zeros((height, width, 3), dtype=np.uint8)
            
            # 处理每个像素
            for i in range(height):
                for j in range(width):
                    if i == 0 or j == width - 1:
                        # 边界像素设为灰色
                        normal_array[i, j] = [128, 128, 255]
                    else:
                        # 获取当前像素和相邻像素的高度值
                        current = height_array[i, j]
                        up = height_array[i - 1, j]
                        right = height_array[i, j + 1]
                        
                        # 计算梯度向量
                        vec1 = [1.0, 0.0, up - current]  # 上方向
                        vec2 = [0.0, 1.0, right - current]  # 右方向
                        
                        # 计算叉积
                        cross_product = get_cross_product(
                            vec1[0], vec1[1], vec1[2] * 4.0,
                            vec2[0], vec2[1], vec2[2] * 4.0
                        )
                        
                        # 归一化
                        normal = vector_normal(cross_product)
                        
                        # 转换为RGB值 (法向量图格式: R=X, G=Y, B=Z)
                        r = int(normal[0] * 128 + 128)
                        g = int(normal[1] * 128 + 128)
                        b = int(normal[2] * 128 + 128)
                        
                        # 限制范围
                        r = min(255, max(0, r))
                        g = min(255, max(0, g))
                        b = min(255, max(0, b))
                        
                        normal_array[i, j] = [r, g, b]
            
            # 转换为PIL图像
            result_image = Image.fromarray(normal_array, 'RGB')
            
            # 转换为QPixmap并显示
            self.target_pixmap = self.pil_to_qpixmap(result_image)
            self.target_label.setPixmap(self.target_pixmap)
            
            # 保存结果图像
            self.result_image = result_image
            
            QMessageBox.information(self, "成功", "法向量图生成完成")
            
        except Exception as e:
            QMessageBox.critical(self, "错误", f"处理图片失败: {str(e)}")
    
    def save_result(self) -> None:
        """
        保存生成的法向量图
        将结果保存到nmPic目录
        """
        if not hasattr(self, 'result_image') or self.result_image is None:
            QMessageBox.warning(self, "警告", "请先生成法向量图")
            return
        
        try:
            # 创建输出目录
            output_dir = "nmPic"
            if not os.path.exists(output_dir):
                os.makedirs(output_dir)
            
            # 生成输出文件名
            base_name = os.path.splitext(self.file_name)[0]
            output_path = os.path.join(output_dir, f"{base_name}_Nm.jpg")
            
            # 保存图片
            self.result_image.save(output_path, "JPEG")
            
            QMessageBox.information(self, "成功", f"法向量图已保存到: {output_path}")
            
        except Exception as e:
            QMessageBox.critical(self, "错误", f"保存图片失败: {str(e)}")
    
    def pil_to_qpixmap(self, pil_image: Image.Image) -> QPixmap:
        """
        将PIL图像转换为QPixmap
        
        Args:
            pil_image: PIL图像对象
        
        Returns:
            QPixmap: Qt图像对象
        """
        # 转换为RGB模式
        if pil_image.mode != 'RGB':
            pil_image = pil_image.convert('RGB')
        
        # 获取图像数据
        width, height = pil_image.size
        data = pil_image.tobytes('raw', 'RGB')
        
        # 创建QImage
        qimage = QImage(data, width, height, QImage.Format_RGB888)
        
        # 转换为QPixmap
        return QPixmap.fromImage(qimage)


def main():
    """主函数"""
    app = QApplication(sys.argv)
    
    # 设置应用程序信息
    app.setApplicationName("法向量纹理图转换工具")
    app.setApplicationVersion("1.0")
    
    # 创建主窗口
    window = NormalMapUtil()
    window.show()
    
    # 运行应用程序
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
