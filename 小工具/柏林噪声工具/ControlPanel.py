"""
控制面板组件
提供噪声生成的各种控制选项和参数设置
"""

import os
import random
import threading
from PySide6.QtWidgets import (QWidget, QVBoxLayout, QHBoxLayout, QLabel, 
                               QComboBox, QSpinBox, QCheckBox, QPushButton,
                               QGroupBox, QGridLayout)
from PySide6.QtCore import Qt, QThread, Signal
from PIL import Image
from Constant import *
from Noise1DUtil import cal_sybp as cal_1d_sybp
from Noise2DUtil import cal_sybp as cal_2d_sybp
from Noise3DUtil import cal_sybp, cal_sybp_bpfl, GLOBAL_SIZE
from ExportUtil3DTexture import export, export_bpfl


class NoiseGenerationThread(QThread):
    """
    噪声生成线程类
    在后台线程中生成噪声，避免界面卡顿
    """
    
    # 定义信号
    generation_completed = Signal()
    error_occurred = Signal(str)
    
    def __init__(self, noise_type, control_panel):
        """
        初始化生成线程
        
        Args:
            noise_type: 噪声类型 (0=1D, 1=2D, 2=3D)
            control_panel: 控制面板引用
        """
        super().__init__()
        self.noise_type = noise_type
        self.control_panel = control_panel
        
    def run(self):
        """
        线程运行函数
        """
        try:
            if self.noise_type == 0:
                self.generate_1d_noise()
            elif self.noise_type == 1:
                self.generate_2d_noise()
            elif self.noise_type == 2:
                self.generate_3d_noise()
                
            self.generation_completed.emit()
            
        except Exception as e:
            self.error_occurred.emit(str(e))
    
    def generate_1d_noise(self):
        """
        生成1D噪声
        """
        global X_CURR
        X_CURR = int(100000 * random.random())
        
        # 切换到1D显示标签页
        self.control_panel.father.display_panel.tab_widget.setCurrentIndex(0)
        
        # 生成噪声数据
        noise_data = cal_1d_sybp()
        self.control_panel.father.display_panel.one_d_display.refresh(noise_data)
    
    def generate_2d_noise(self):
        """
        生成2D噪声
        """
        global X_CURR, Y_CURR
        X_CURR = int(100000 * random.random())
        Y_CURR = int(100000 * random.random())
        
        # 切换到2D显示标签页
        self.control_panel.father.display_panel.tab_widget.setCurrentIndex(1)
        
        # 生成噪声数据
        noise_images = cal_2d_sybp()
        self.control_panel.father.display_panel.two_d_display.refresh(noise_images)
        
        # 保存图像到文件
        self.save_2d_images(noise_images)
    
    def generate_3d_noise(self):
        """
        生成3D噪声
        """
        global X_CURR, Y_CURR, Z_CURR
        X_CURR = int(100000 * random.random())
        Y_CURR = int(100000 * random.random())
        Z_CURR = int(100000 * random.random())
        
        # 切换到3D显示标签页
        self.control_panel.father.display_panel.tab_widget.setCurrentIndex(2)
        
        # 生成噪声数据
        if self.control_panel.rgba_checkbox.isChecked():
            # RGBA各一个倍频模式
            noise_data = cal_sybp_bpfl()
            self.control_panel.father.display_panel.three_d_display.refresh(D3Each)
            export_bpfl(noise_data)
        else:
            # 普通模式
            noise_data = cal_sybp()
            self.control_panel.father.display_panel.three_d_display.refresh(D3Each)
            export(noise_data)
    
    def save_2d_images(self, images):
        """
        保存2D噪声图像到文件
        
        Args:
            images: 图像列表
        """
        # 确保pic目录存在
        os.makedirs("pic", exist_ok=True)
        
        # 找到可用的文件名
        count = 0
        while True:
            filename = f"pic/noise{count}.png"
            if not os.path.exists(filename):
                break
            count += 1
        
        # 保存最后一个图像（合成结果）
        if images and len(images) > PLS:
            images[PLS].save(filename)


class ControlPanel(QWidget):
    """
    控制面板类
    提供噪声生成的各种控制选项
    """
    
    def __init__(self, parent=None):
        """
        初始化控制面板
        
        Args:
            parent: 父组件
        """
        super().__init__(parent)
        self.father = parent
        self.generation_thread = None
        self.init_ui()
        self.setup_connections()
        
    def init_ui(self):
        """
        初始化用户界面
        """
        layout = QVBoxLayout(self)
        layout.setSpacing(10)
        layout.setContentsMargins(10, 10, 10, 10)
        
        # 噪声维度选择组
        dimension_group = QGroupBox("噪声维度")
        dimension_layout = QGridLayout(dimension_group)
        
        dimension_layout.addWidget(QLabel("噪声维度:"), 0, 0)
        self.dimension_combo = QComboBox()
        self.dimension_combo.addItems(["1D柏林噪声", "2D柏林噪声", "3D柏林噪声"])
        dimension_layout.addWidget(self.dimension_combo, 0, 1)
        
        layout.addWidget(dimension_group)
        
        # 噪声参数组
        params_group = QGroupBox("噪声参数")
        params_layout = QGridLayout(params_group)
        
        params_layout.addWidget(QLabel("噪声频率数:"), 0, 0)
        self.frequency_spin = QSpinBox()
        self.frequency_spin.setRange(1, 6)
        self.frequency_spin.setValue(5)
        params_layout.addWidget(self.frequency_spin, 0, 1)
        
        layout.addWidget(params_group)
        
        # 2D噪声选项组
        self.two_d_group = QGroupBox("2D噪声选项")
        two_d_layout = QVBoxLayout(self.two_d_group)
        
        self.contrast_checkbox = QCheckBox("2D噪声各分频是否增强对比度")
        self.contrast_checkbox.setChecked(True)
        two_d_layout.addWidget(self.contrast_checkbox)
        
        layout.addWidget(self.two_d_group)
        
        # 3D噪声选项组
        self.three_d_group = QGroupBox("3D噪声选项")
        three_d_layout = QGridLayout(self.three_d_group)
        
        self.rgba_checkbox = QCheckBox("3D噪声是否RGBA各一个倍频")
        three_d_layout.addWidget(self.rgba_checkbox, 0, 0, 1, 2)
        
        three_d_layout.addWidget(QLabel("3D噪声尺寸:"), 1, 0)
        self.size_combo = QComboBox()
        self.size_combo.addItems(["64X64X64", "32X32X32"])
        three_d_layout.addWidget(self.size_combo, 1, 1)
        
        layout.addWidget(self.three_d_group)
        
        # 生成按钮
        self.generate_button = QPushButton("生成噪声图像及数据")
        self.generate_button.setMinimumHeight(40)
        layout.addWidget(self.generate_button)
        
        # 添加弹性空间
        layout.addStretch()
        
        # 初始状态设置
        self.update_controls_state()
        
    def setup_connections(self):
        """
        设置信号连接
        """
        self.dimension_combo.currentIndexChanged.connect(self.on_dimension_changed)
        self.frequency_spin.valueChanged.connect(self.on_frequency_changed)
        self.contrast_checkbox.toggled.connect(self.on_contrast_changed)
        self.rgba_checkbox.toggled.connect(self.on_rgba_changed)
        self.size_combo.currentIndexChanged.connect(self.on_size_changed)
        self.generate_button.clicked.connect(self.on_generate_clicked)
        
    def on_dimension_changed(self, index):
        """
        噪声维度改变时的处理函数
        
        Args:
            index: 选中的索引
        """
        self.update_controls_state()
        
    def on_frequency_changed(self, value):
        """
        噪声频率数改变时的处理函数
        
        Args:
            value: 新的频率数值
        """
        global PLS
        PLS = value
        
    def on_contrast_changed(self, checked):
        """
        对比度增强选项改变时的处理函数
        
        Args:
            checked: 是否选中
        """
        global ZQDBD_FLAG
        ZQDBD_FLAG = checked
        
    def on_rgba_changed(self, checked):
        """
        RGBA倍频选项改变时的处理函数
        
        Args:
            checked: 是否选中
        """
        pass  # 在生成时使用
        
    def on_size_changed(self, index):
        """
        3D噪声尺寸改变时的处理函数
        
        Args:
            index: 选中的索引
        """
        global GLOBAL_SIZE
        if index == 0:
            GLOBAL_SIZE = 64
        else:
            GLOBAL_SIZE = 32
            
    def update_controls_state(self):
        """
        更新控制组件的启用状态
        """
        dimension = self.dimension_combo.currentIndex()
        
        # 根据噪声维度启用/禁用相关控件
        if dimension == 0:  # 1D
            self.contrast_checkbox.setEnabled(False)
            self.rgba_checkbox.setEnabled(False)
            self.size_combo.setEnabled(False)
        elif dimension == 1:  # 2D
            self.contrast_checkbox.setEnabled(True)
            self.rgba_checkbox.setEnabled(False)
            self.size_combo.setEnabled(False)
        elif dimension == 2:  # 3D
            self.contrast_checkbox.setEnabled(False)
            self.rgba_checkbox.setEnabled(True)
            self.size_combo.setEnabled(True)
            
    def set_controls_disabled(self):
        """
        禁用所有控制组件
        """
        self.dimension_combo.setEnabled(False)
        self.frequency_spin.setEnabled(False)
        self.contrast_checkbox.setEnabled(False)
        self.rgba_checkbox.setEnabled(False)
        self.size_combo.setEnabled(False)
        self.generate_button.setEnabled(False)
        
    def set_controls_enabled(self):
        """
        启用控制组件
        """
        self.dimension_combo.setEnabled(True)
        self.frequency_spin.setEnabled(True)
        self.generate_button.setEnabled(True)
        self.update_controls_state()
        
    def on_generate_clicked(self):
        """
        生成按钮点击时的处理函数
        """
        self.set_controls_disabled()
        
        # 创建并启动生成线程
        dimension = self.dimension_combo.currentIndex()
        self.generation_thread = NoiseGenerationThread(dimension, self)
        self.generation_thread.generation_completed.connect(self.on_generation_completed)
        self.generation_thread.error_occurred.connect(self.on_generation_error)
        self.generation_thread.start()
        
    def on_generation_completed(self):
        """
        噪声生成完成时的处理函数
        """
        self.set_controls_enabled()
        
    def on_generation_error(self, error_message):
        """
        噪声生成出错时的处理函数
        
        Args:
            error_message: 错误信息
        """
        self.set_controls_enabled()
        print(f"生成噪声时出错: {error_message}")
