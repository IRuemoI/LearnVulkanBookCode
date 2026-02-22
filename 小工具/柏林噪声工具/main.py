"""
柏林噪声工具主程序入口
使用PySide6构建的图形界面应用程序
"""

import sys
import os
from PySide6.QtWidgets import QApplication
from PySide6.QtCore import Qt
from MainFrame import MainFrame


def main():
    """
    主函数
    初始化应用程序并显示主窗口
    """
    # 创建应用程序实例
    app = QApplication(sys.argv)
    
    # 设置应用程序属性
    app.setApplicationName("Perlin噪声生成工具")
    app.setApplicationVersion("1.0.0")
    app.setOrganizationName("柏林噪声工具")
    
    # 设置高DPI支持
    app.setAttribute(Qt.ApplicationAttribute.AA_EnableHighDpiScaling, True)
    app.setAttribute(Qt.ApplicationAttribute.AA_UseHighDpiPixmaps, True)
    
    # 创建主窗口
    main_window = MainFrame()
    main_window.show()
    
    # 运行应用程序
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
