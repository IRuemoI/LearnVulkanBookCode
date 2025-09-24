#!/bin/bash

# Vulkan SDK路径
VULKAN_SDK_PATH="/users/yinjianxin/IDE/VulkanSDK/1.3.290.0/macos"

# 确保spirv子目录存在
mkdir -p shaders/

# 编译顶点着色器
$VULKAN_SDK_PATH/bin/glslc -fshader-stage=vertex shaders/base.vert -o shaders/vert.spv

# 编译片段着色器
$VULKAN_SDK_PATH/bin/glslc -fshader-stage=fragment shaders/base.frag -o shaders/frag.spv

echo "着色器编译完成，输出目录：shaders/"