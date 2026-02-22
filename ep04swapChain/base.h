//常用的头文件，以免重复包含
#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <memory>//智能指针
#include <cstring>
#include <optional>
#include <set>

#include <glm/glm.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>


//设置需要启用的校验层名称列表(最好使用宏定义)
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation" //官方校验层。会对函数参数，以及资源是否正确使用以及释放等做验证，并提示错误。
};
