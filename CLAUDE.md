# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 项目概述

这是一个综合性的Vulkan学习项目，包含两本Vulkan书籍的示例代码：
- 《Vulkan开发实战详解》(吴亚峰) - Chapter_* 系列
- 现代Vulkan教程 - ep* 系列 (使用现代C++框架)

## 构建系统

**特别注意：**所有子项目共用依赖和头文件，所以需要运行特定的子项目时确保该子项目已经在根目录的CMakeList文件中添加或者启用

### 主要构建命令
```bash
# 创建构建目录
mkdir build
cd build

# 配置项目（自动查找Vulkan SDK）
cmake ..

# 或手动指定Vulkan SDK路径
cmake .. -DAUTO_LOCATE_VULKAN=OFF -DVULKAN_SDK=E:/IDE/VulkanSDK -DVULKAN_VERSION=1.3.290.0

# 构建特定项目（在根目录CMakeLists.txt中启用相应章节）
# 取消注释相应的 add_subdirectory() 行
```

### 构建配置
- **C++标准**: C++20
- **Vulkan SDK**: 1.3.290.0
- **主要依赖**: GLFW 3.4, GLM, glslang
- **平台支持**: Windows, macOS (通过MoltenVK)

### 依赖管理
项目使用Git子模块管理第三方库：
```bash
# 初始化子模块
git submodule update --init --recursive
```

## 项目架构

### 传统架构 (Chapter_* 系列)
- **设计模式**: 单体应用模式
- **资源管理**: 手动管理Vulkan资源
- **文件组织**: src/ 目录集中所有源码
- **适用场景**: 学习完整的Vulkan渲染流程

### 现代架构 (Chapter13-04-refactored)
- **设计模式**: 模块化设计，RAII + 智能指针
- **资源管理**: 自动资源管理，异常安全
- **文件组织**: vulkanWrapper/ 目录模块化封装
- **适用场景**: 现代C++开发实践

### 现代C++教程 (ep* 系列)
这是一个循序渐进的现代Vulkan教程系列，推荐按顺序学习：
- **ep01instance**: Vulkan实例创建
- **ep02device**: 物理和逻辑设备
- **ep03surface**: 窗口表面
- **ep04swapChain**: 交换链
- **ep05pipeline**: 渲染管线
- **ep06renderPass**: 渲染通道
- **ep07command**: 命令缓冲
- **ep08vertex**: 顶点缓冲
- **ep09uniform**: Uniform缓冲
- **ep10imageAndSampler**: 图像和采样器
- **ep11depth**: 深度缓冲
- **ep12antiAlias**: 抗锯齿
- **ep13modelDrawing**: 模型绘制

## 开发工作流

### 新增章节项目
1. 在对应系列目录下创建新文件夹
2. 添加src/目录和源文件
3. 创建CMakeLists.txt文件，参考现有模板
4. 在根目录CMakeLists.txt中添加`add_subdirectory()`
5. 更新README.md文档

### 编译着色器
项目支持GLSL到SPIR-V的编译：
```bash
# 使用glslangValidator
glslangValidator shader.vert -V -o shader.vert.spv
glslangValidator shader.frag -V -o shader.frag.spv

# 或使用项目批处理脚本
cd shaders
compile.bat
```

### 现代C++教程学习路径
建议按以下顺序学习ep*系列：
1. **基础阶段**: ep01-ep03 - 环境搭建和设备初始化
2. **渲染基础**: ep04-ep07 - 交换链、管线、命令
3. **渲染进阶**: ep08-ep09 - 顶点和Uniform数据
4. **渲染完善**: ep10-ep13 - 纹理、深度、抗锯齿、模型

### macOS开发注意事项
- 必须启用`VK_KHR_portability_enumeration`扩展
- 添加`VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR`标志
- 设备创建时需要`VK_KHR_portability_subset`扩展
- Apple GPU评分逻辑已适配

## 常见问题解决

### 编译错误
- **链接顺序问题**: MinGW需要调整链接顺序，textureLib依赖vulkanLib
- **找不到Vulkan SDK**: 检查环境变量或使用手动路径配置
- **CMake版本过低**: 需要3.4以上版本

### 运行时错误
- **0xC0000374错误**: 多个章节存在程序退出时的堆栈溢出问题
- **黑屏问题**: Chapter16-02存在渲染问题
- **水面效果问题**: Chapter17-07水面效果未正常显示

## GPU配置

支持通过config.json文件配置GPU选择：
```json
{
  "GpuIndex": 0
}
```

在代码中使用nlohmann/json库读取配置。

## 现代C++最佳实践

在重构版本和ep*系列中使用的现代C++特性：
- **智能指针**: `std::shared_ptr`, `std::unique_ptr`
- **RAII**: 自动资源管理
- **异常处理**: 替代错误码
- **工厂模式**: 统一对象创建接口
- **类型安全**: 强类型封装

## 性能优化建议

1. **资源复用**: 避免频繁创建销毁Vulkan对象
2. **命令缓冲优化**: 批量录制命令
3. **内存管理**: 使用内存池减少分配开销
4. **同步优化**: 最小化CPU-GPU同步点
