# Chapter13-04 重构项目

## 项目概述

本项目是将原始的Chapter13-04项目重构为采用范例项目（ep12antiAlias和ep13modelDrawing）架构模式的新版本。重构后的项目采用了更清晰的模块化设计，使用了vulkanWrapper封装层来简化Vulkan API的使用。

## 架构特点

### 1. 模块化设计
- **vulkanWrapper封装层**: 提供了对Vulkan API的高级封装，包括Instance、Device、SwapChain、Pipeline等核心组件
- **Application类**: 采用现代C++设计模式，使用智能指针管理资源
- **UniformManager**: 统一管理着色器uniform变量和描述符集
- **Model类**: 封装3D模型数据和渲染逻辑

### 2. 资源管理
- 使用RAII机制和智能指针自动管理Vulkan资源
- 采用工厂模式创建各种Vulkan对象
- 支持多帧渲染和同步机制

### 3. 渲染管线
- 支持多重采样抗锯齿（MSAA）
- 完整的深度测试和模板测试
- 动态视口和裁剪区域

## 项目结构

```
Chapter13-04-refactored/
├── application.h/cpp          # 主应用程序类
├── model.h                    # 3D模型类
├── uniformManager.h/cpp       # Uniform变量管理器
├── base.h                     # 基础头文件和数据结构
├── main.cpp                   # 程序入口
├── vulkanWrapper/             # Vulkan API封装层
│   ├── instance.h/cpp
│   ├── device.h/cpp
│   ├── window.h/cpp
│   ├── swapChain.h/cpp
│   ├── renderPass.h/cpp
│   ├── pipeline.h/cpp
│   ├── commandBuffer.h/cpp
│   └── ... (其他Vulkan组件)
├── texture/                   # 纹理管理
├── shaders/                   # 着色器文件
│   ├── shader.vert
│   ├── shader.frag
│   ├── shader.vert.spv
│   └── shader.frag.spv
└── CMakeLists.txt            # 构建配置
```

## 主要改进

### 1. 代码组织
- 将原始的VulkanDemoApp重构为Application类
- 采用命名空间FF来组织代码
- 使用现代C++特性（智能指针、RAII等）

### 2. 错误处理
- 集成了Vulkan验证层
- 更好的异常处理机制
- 详细的错误信息输出

### 3. 可扩展性
- 模块化设计便于添加新功能
- 清晰的接口定义
- 支持多种渲染效果

## 编译和运行

### 前置要求
- CMake 3.4+
- C++20编译器
- Vulkan SDK
- GLFW
- GLM
- glslang

### 编译步骤
```bash
# 在项目根目录创建构建目录
mkdir build-refactored
cd build-refactored

# 配置CMake
cmake ..

# 编译
make Chapter13-04-refactored
```

### 运行
```bash
cd Chapter13-04-refactored
./Chapter13-04-refactored
```

## 功能特性

### 1. 渲染功能
- 3D场景渲染
- 多重采样抗锯齿
- 深度测试
- 纹理映射（可扩展）

### 2. 交互功能
- 鼠标控制相机视角
- 键盘控制相机移动
- 窗口大小调整支持

### 3. 性能优化
- 多帧渲染支持
- 高效的资源管理
- GPU同步机制

## 技术细节

### 1. 着色器
- 顶点着色器：处理顶点变换和纹理坐标
- 片段着色器：处理纹理采样和颜色输出

### 2. 渲染管线
- 支持MSAA的多重采样渲染
- 完整的深度缓冲
- 动态视口设置

### 3. 内存管理
- 使用Vulkan内存分配器
- 智能指针自动资源释放
- 避免内存泄漏

## 与原项目的对比

| 特性 | 原项目 | 重构后项目 |
|------|--------|------------|
| 架构 | 单体式 | 模块化 |
| 资源管理 | 手动管理 | RAII + 智能指针 |
| 错误处理 | 基础 | 完善的验证层 |
| 可维护性 | 中等 | 高 |
| 可扩展性 | 有限 | 良好 |
| 代码复用 | 低 | 高 |

## 未来扩展

1. **纹理系统**: 完善纹理加载和管理
2. **模型加载**: 支持OBJ、glTF等格式
3. **光照系统**: 添加多种光照模型
4. **后处理**: 实现各种后处理效果
5. **多线程**: 支持多线程渲染

## 总结

重构后的项目成功地将原始的Chapter13-04项目转换为采用现代C++和模块化设计的架构。通过使用vulkanWrapper封装层，大大简化了Vulkan API的使用，提高了代码的可读性和可维护性。项目现在具有更好的错误处理、资源管理和扩展性，为后续的功能开发奠定了良好的基础。
