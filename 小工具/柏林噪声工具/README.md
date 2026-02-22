# 柏林噪声工具 (Python版本)

这是一个用于生成1D、2D和3D柏林噪声的工具，使用PySide6构建图形界面。

## 功能特性

- 生成1D柏林噪声并显示波形图
- 生成2D柏林噪声并显示灰度图像
- 生成3D柏林噪声并显示切片图像
- 支持多种噪声频率和尺寸设置
- 可导出3D纹理数据文件

## 安装依赖

```bash
pip install -r requirements.txt
```

## 运行程序

### 方法1：直接运行主程序
```bash
python main.py
```

### 方法2：使用启动脚本
```bash
python run.py
```

### 方法3：在Unix/Linux/macOS系统上
```bash
./run.py
```

## 使用说明

1. 选择噪声维度（1D/2D/3D）
2. 设置噪声频率数（1-6）
3. 对于2D噪声，可选择是否增强对比度
4. 对于3D噪声，可选择RGBA各一个倍频模式
5. 点击"生成噪声图像及数据"按钮开始生成
6. 生成的图像会显示在右侧面板中
7. 2D和3D噪声会自动保存到文件

## 技术特性

- **多线程处理**: 使用QThread在后台生成噪声，避免界面卡顿
- **模块化设计**: 每个功能模块独立，便于维护和扩展
- **类型注解**: 完整的Python类型注解，提高代码可读性
- **错误处理**: 完善的异常处理机制
- **跨平台**: 支持Windows、macOS和Linux系统

## 文件结构

- `main.py` - 主程序入口
- `run.py` - 启动脚本
- `MainFrame.py` - 主窗口类
- `ControlPanel.py` - 控制面板
- `DisplayPanel.py` - 显示面板
- `OneDDisplay.py` - 1D噪声显示组件
- `TwoDDisplay.py` - 2D噪声显示组件
- `ThreeDDisplay.py` - 3D噪声显示组件
- `Noise1DUtil.py` - 1D噪声生成工具
- `Noise2DUtil.py` - 2D噪声生成工具
- `Noise3DUtil.py` - 3D噪声生成工具
- `ExportUtil3DTexture.py` - 3D纹理导出工具
- `Constant.py` - 常量定义
- `requirements.txt` - Python依赖包列表
- `README.md` - 项目说明文档

## 输出文件

- `pic/noise*.png` - 2D噪声图像文件
- `TexData3D/3dNoise.bn3dtex` - 3D纹理数据文件
