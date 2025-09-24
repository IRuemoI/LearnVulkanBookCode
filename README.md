## 项目说明

通过学习本项目中包含的两本Vulkan相关的参考书——《Vulkan学习指南(王锐 译)》《Vulkan开发实战详解(吴亚峰)》及其示例代码，小伙伴们可以以相对平缓的学习曲线完成Vulkan入门到进阶阶段的学习。

**发行版中的epub文件仅供学习用途！**

## 本项目的内容包含

1. 《Vulkan学习指南(王锐 译)》(英文原书:《Learning Vulkan(Parminder Singh)》)的随书代码
2. 《Vulkan开发实战详解(吴亚峰)》的随书代码

## 开发环境以及项目依赖

* 独立显卡(可选项，支持的层和扩展更多)
* VS/CLion/VSCode
* VulkanSDK:1.3.290.0(安装时勾选glm)
* glfw:3.4
* glm:复制自vulkanSDK中的include文件夹
* glslang:在Github[仓库](https://github.com/KhronosGroup/glslang)选择与vulkanSDK版本匹配的1.3.290.0版本的分支并克隆到本项目的依赖文件夹内(在CMakeLists.txt中已经配置完毕)

![环境配置](./Assets/环境配置.png)

## 注意事项

* 《Vulkan学习指南(王锐译)》部分
    * 代码位置：本项目中所有以`Basic`为前缀的文件夹内
* 《Vulkan开发实战详解》部分
    * 本仓库中包含的内容**仅包含**[原书附件示例代码](https://www.epubit.com/bookDetails?id=UB6c7827767c54a)中十九章之前的PC实例代码，如果了解安卓开发并想学习vulkan的小伙伴们建议同时参照原书的安卓版代码以及本仓库中的代码，避免Vulkan API过时。
    * 代码位置：本项目中所有以`Chapter`为前缀的文件夹内
    * 使用新开发环境、替换过时API、修复BUG并小幅重构后的可运行的代码。使用CMake构建。
    * 文件夹内的所有项目共用文件夹根目录的依赖
    * 仓库内上传了前18章的模型和纹理文件，但需要注意的是一个模型或纹理文件可能会被多个项目使用。第19章并没有整理，模型、纹理等资源文件在第19章的子目录中。
    * 本项目中的文件使用UTF8编码
    * 由于GLFW学习理解门槛相对较低，项目中使用GLFW代替了WIN32API实现窗口的创建以及外设输入处理

由于所有项目使用的物理设备均为物理设备列表中的第一个设备，需要使用设备的高级特性请调整选择的物理设备索引；当然这里也给出了使用Json配置GPU选择的一种方式：在仓库的根目录创建`config.json`文件

~~~json
{
  "GpuIndex": 0
}
~~~

然后在代码中使用：

~~~ c++
//头文件导入
#include <fstream>
#include <nlohmann/json.hpp>

//读取并输出json文件中键对应的值
std::ifstream f("../config.json");
json data = json::parse(f);
std::cout << data["GpuIndex"] << std::endl;
f.close();
~~~

## 代办列表：

* **重要(影响程序效果)**
    * 后续修复16.02黑屏的问题
    * 后续修复17.07没有水面效果的问题
    * 找到为什么使用RenderDoc对进阶学习中的项目进行抓帧启动时会报错的原因(在Clion中可以运行，在文件管理器中双击打开报错 可能是缺库？)

* **次要(不影响效果)**
    * 后续修复12.14关闭程序会报错`0xC0000374`的问题
    * 后续修复12.15关闭程序会报错`0xC0000374`的问题
    * 后续修复13.07关闭程序会报错`0xC0000374`的问题
    * 后续修复14.01关闭程序会报错`0xC0000374`的问题
    * 后续修复14.02关闭程序会报错`0xC0000374`的问题
    * 后续修复14.07关闭程序会报错`0xC0000374`的问题
    * 后续修复15.03关闭程序会报错`0xC0000374`的问题
    * 后续修复15.05关闭程序会报错`0xC0000374`的问题
    * 后续修复15.08关闭程序会报错`0xC0000374`的问题
    * 后续修复16.01关闭程序会报错`0xC0000374`的问题
    * 后续修复16.03关闭程序会报错`0xC0000374`的问题
    * 后续修复16.04关闭程序会报错`0xC0000374`的问题
    * 后续修复16.05关闭程序会报错`0xC0000374`的问题
    * 后续修复17.01关闭程序会报错`0xC0000374`的问题
    * 后续修复18.01关闭程序会报错`0xC0000374`的问题
    * 所有项目在改变窗口大小后都不能正确绘制(重建交换链)

* **后续计划**
  * 在完成18章的示例项目后给出一个集成[ImGui](https://github.com/ocornut/imgui),[Json](https://github.com/nlohmann/json)的三色三角形示例项目
  * 整理第19章内容

## Q&A

### Q:项目依赖如何配置？

本项目使用VulkanSDK版本为：1.3.290. 并且以下第三方库作为依赖项，可以通过git submodule进行管理：

- **GLFW**: 窗口和输入管理库
  - 版本分支: 3.4
  - 仓库地址: https://github.com/glfw/glfw
  - 项目路径: dependencies/glfw

- **GLI**: OpenGL图像库，用于纹理加载
  - 版本分支: main
  - 仓库地址: https://github.com/g-truc/gli
  - 项目路径: dependencies/gli

- **GLM**: OpenGL数学库，用于图形计算
  - 版本分支: main
  - 仓库地址: https://github.com/g-truc/glm
  - 项目路径: dependencies/glm

- **GLSLang**: GLSL着色器编译器
  - 版本分支: vulkan-sdk-1.3.290
  - 仓库地址: https://github.com/KhronosGroup/glslang
  - 项目路径: dependencies/glslang


### Q:如何在MacOS(Apple Silicon)运行此项目?

**MoltenVK安装与版本要求**

   - macOS上Vulkan通过MoltenVK实现，需确保安装版本≥1.2.3以支持必要的扩展。
   - 推荐使用Homebrew安装：`brew install molten-vk`

**端口ability扩展配置**
   - **实例创建阶段**：需添加`VK_KHR_portability_enumeration`扩展和`VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR`标志。
   - **设备创建阶段**：需添加`VK_KHR_portability_subset`扩展。
   - 确保`VK_KHR_get_physical_device_properties2`扩展在实例级别启用（非设备级别）。

**设备选择与配置**
   - 对于Apple GPU（M1/M2/M3/M4系列），需放宽设备类型要求（不严格要求独立显卡）。
   - 注意Apple GPU可能不支持某些特性（如几何着色器），需在设备评分和选择时考虑这些限制。

**扩展依赖关系**
   - 区分实例级扩展和设备级扩展，避免将实例级扩展错误添加到设备扩展列表。
   - `VK_KHR_portability_subset`依赖于`VK_KHR_get_physical_device_properties2`，需确保两者都正确配置。

**验证层错误处理**
   - 常见错误如`VK_KHR_portability_subset must be enabled`或依赖扩展缺失，需检查扩展配置。
   - 使用标准宏定义（如`VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME`）而非硬编码字符串，避免拼写错误。

**文件路径与资源加载**
   - macOS文件系统区分大小写，确保纹理、着色器等资源文件路径正确。
   - 检查STB_IMAGE库配置，确保正确读取图像数据。
**代码示例**

实例部分(instance.cpp)

~~~ cpp
Instance::Instance(bool enableValidationLayer) {
    mEnableValidationLayer = enableValidationLayer;

    if (mEnableValidationLayer && !checkValidationLayerSupport()) {
        throw std::runtime_error("Error: validation layer is not supported");
    }

    printAvailableExtensions();

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "vulkanLession";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "NO ENGINE";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instCreateInfo = {};
    instCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instCreateInfo.pApplicationInfo = &appInfo;

    //扩展相关
    auto extensions = getRequiredExtensions();
    instCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instCreateInfo.ppEnabledExtensionNames = extensions.data();
    #if defined(__APPLE__)
    instCreateInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    #endif
    //layer相关
    if (mEnableValidationLayer) {
        instCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        instCreateInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        instCreateInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&instCreateInfo, nullptr, &mInstance) != VK_SUCCESS) {
        throw std::runtime_error("Error:failed to create instance");
    }

    setupDebugger();
}

std::vector<const char *> Instance::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;

    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    #if defined(__APPLE__)
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    #endif
    return extensions;
}
~~~

设备部分(device.cpp)

~~~ cpp
int Device::rateDevice(VkPhysicalDevice device) {
    int score = 0;

    // 设备名称 类型 支持vulkan的版本
    VkPhysicalDeviceProperties deviceProp;
    vkGetPhysicalDeviceProperties(device, &deviceProp);

    // 纹理压缩 浮点数运算特性 多视口渲染
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    // 打印设备信息进行调试
    //printf("设备名称: %s\n", deviceProp.deviceName);
    //printf("设备类型: %d\n", deviceProp.deviceType);
    //printf("支持几何着色器: %d\n", deviceFeatures.geometryShader);
    //printf("支持各向异性采样: %d\n", deviceFeatures.samplerAnisotropy);

    // 不同平台的设备评分逻辑
    #if defined(__APPLE__)
    // macOS平台：M系列芯片只有集成显卡，否则会找不到物理设备 
    // 检查是否为Apple GPU (通过设备名称识别)
    if (std::string(deviceProp.deviceName).find("Apple") != std::string::npos)
    {
        score += 1000;
    }
    // 确保Apple GPU至少有基础分数
    score = std::max(score, 100);
    #elif defined(_WIN32)
    // Windows平台：仅为独立显卡提供高分
    if (deviceProp.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }
    #endif

    score += deviceProp.limits.maxImageDimension2D;

    return score;
}

bool Device::isDeviceSuitable(VkPhysicalDevice device) {
    // 设备名称 类型 支持vulkan的版本
    VkPhysicalDeviceProperties deviceProp;
    vkGetPhysicalDeviceProperties(device, &deviceProp);

    // 纹理压缩 浮点数运算特性 多视口渲染
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    // 不同平台的设备适配逻辑
    #if defined(_WIN32)
    // Windows平台：保持原有要求
    return deviceProp.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
        deviceFeatures.geometryShader &&
        deviceFeatures.samplerAnisotropy;
		#elif defined(__APPLE__)
        // macOS平台：放宽设备特性支持要求
        return true;
		#else
		//比如要求物理设备必须是独立显卡且要支持几何着色器
		return deviceProp.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
			deviceFeatures.geometryShader;
		#endif
}
void Device::createLogicalDevice() {
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    std::set<uint32_t> queueFamilies = {mGraphicQueueFamily.value(), mPresentQueueFamily.value()};

    float queuePriority = 1.0;

    for (uint32_t queueFamily: queueFamilies) {
        // 填写创建信息
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }

    // 填写逻辑设备创建信息
    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    std::vector<const char *> extensions = deviceRequiredExtensions;
    #if defined(__APPLE__)
    // 添加VK_KHR_portability_subset扩展
    extensions.push_back("VK_KHR_portability_subset");
    #endif

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = extensions.data();

    // layer层
    if (mInstance->getEnableValidationLayer()) {
        deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        deviceCreateInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mDevice) != VK_SUCCESS) {
        throw std::runtime_error("Error:failed to create logical device");
    }

    vkGetDeviceQueue(mDevice, mGraphicQueueFamily.value(), 0, &mGraphicQueue);
    vkGetDeviceQueue(mDevice, mPresentQueueFamily.value(), 0, &mPresentQueue);
}
~~~

### Q:为什么本项目需要使用VS打开？，以及为什么使用CLion默认设置有可能会构建失败

因为MSVC编译器对于依赖库的链接顺序相对宽松，而CLion默认设置(使用MingW编译器)则相对严格，需要调整CMakeLists.txt文件中的链接顺序，否则会出现链接错误。

~~~ cmake
# 比如
target_link_libraries(${CHAPTER_NAME}
        vulkanLib
        textureLib # 因为textureLib依赖vulkanLib
        Vulkan::Vulkan
        glslang
        glslang::SPIRV
        glslang::glslang-default-resource-limits
        glfw
        glm
)
#需要调整为
target_link_libraries(${CHAPTER_NAME}
        textureLib
        vulkanLib
        Vulkan::Vulkan
        glslang
        glslang::SPIRV
        glslang::glslang-default-resource-limits
        glfw
        glm
)
~~~

## 新架构 vs 旧架构的详细优点

本项目包含了原始架构（Chapter13-04）和重构后架构（Chapter13-04-refactored）的对比。重构后的架构相比原始架构具有显著的优势，以下是详细分析：

### 1. 架构设计层面的改进

#### 1.1 模块化设计 vs 单体式设计

**旧架构（Chapter13-04）：**
```cpp
class VulkanDemoApp {
    // 所有功能都集中在一个类中
    VkInstance instance;
    VkDevice device;
    VkSwapchainKHR swapChain;
    VkRenderPass renderPass;
    VkPipeline pipeline;
    // ... 100多个成员变量
    // 所有方法都在一个类中
    void initVulkanInstance();
    void createVulkanDevices();
    void createVulkanSwapChain();
    // ... 50多个方法
};
```

**新架构（Chapter13-04-refactored）：**
```cpp
// 职责分离，每个类负责特定功能
class Application {
    Wrapper::Instance::Ptr mInstance;
    Wrapper::Device::Ptr mDevice;
    Wrapper::SwapChain::Ptr mSwapChain;
    // 清晰的依赖关系
};

class Wrapper::Device {
    // 专门负责设备管理
};

class Wrapper::SwapChain {
    // 专门负责交换链管理
};
```

**优点：**
- **单一职责原则**：每个类只负责一个特定功能
- **降低耦合度**：模块间依赖关系清晰
- **提高可测试性**：可以独立测试每个模块
- **便于团队协作**：不同开发者可以并行开发不同模块

#### 1.2 依赖注入 vs 硬编码依赖

**旧架构：**
```cpp
void VulkanDemoApp::initVulkan() {
    // 直接创建和管理所有对象
    instance = createInstance();
    device = createDevice();
    swapChain = createSwapChain();
}
```

**新架构：**
```cpp
void Application::initVulkan() {
    // 通过工厂方法创建，依赖注入
    mInstance = Wrapper::Instance::create(true);
    mDevice = Wrapper::Device::create(mInstance, mSurface);
    mSwapChain = Wrapper::SwapChain::create(mDevice, mWindow, mSurface, mCommandPool);
}
```

**优点：**
- **灵活性**：可以轻松替换实现
- **可配置性**：参数化创建过程
- **可扩展性**：易于添加新的创建策略

### 2. 资源管理层面的改进

#### 2.1 RAII + 智能指针 vs 手动管理

**旧架构：**
```cpp
class VulkanDemoApp {
    VkDevice device;
    VkSwapchainKHR swapChain;
    VkRenderPass renderPass;
    
    ~VulkanDemoApp() {
        // 手动清理，容易遗漏
        vkDestroyRenderPass(device, renderPass, nullptr);
        vkDestroySwapchainKHR(device, swapChain, nullptr);
        vkDestroyDevice(device, nullptr);
    }
};
```

**新架构：**
```cpp
class Application {
    Wrapper::Device::Ptr mDevice{ nullptr };
    Wrapper::SwapChain::Ptr mSwapChain{ nullptr };
    Wrapper::RenderPass::Ptr mRenderPass{ nullptr };
    
    // 析构函数自动调用智能指针的析构
    // 无需手动管理资源
};

class Wrapper::Device {
    VkDevice mDevice{ VK_NULL_HANDLE };
    ~Device() {
        if (mDevice != VK_NULL_HANDLE) {
            vkDestroyDevice(mDevice, nullptr);
        }
    }
};
```

**优点：**
- **自动资源管理**：智能指针自动处理资源释放
- **异常安全**：即使发生异常也能正确清理资源
- **减少内存泄漏**：RAII机制确保资源不泄漏
- **简化代码**：无需手动编写清理代码

### 3. 错误处理层面的改进

#### 3.1 异常处理 vs 错误码

**旧架构：**
```cpp
VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
if (result != VK_SUCCESS) {
    // 简单的错误处理
    std::cerr << "Failed to create instance!" << std::endl;
    return;
}
```

**新架构：**
```cpp
class Wrapper::Instance {
    Instance() {
        if (vkCreateInstance(&createInfo, nullptr, &mInstance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance!");
        }
    }
};

// 使用时的异常处理
try {
    mInstance = Wrapper::Instance::create(true);
} catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
```

**优点：**
- **统一的错误处理**：使用C++异常机制
- **更好的错误信息**：详细的错误描述
- **异常安全**：保证程序状态一致性
- **减少错误处理代码**：集中处理异常

### 4. 代码质量层面的改进

#### 4.1 类型安全

**旧架构：**
```cpp
// 使用原始指针，类型不安全
VkDevice device;
VkSwapchainKHR swapChain;
// 容易混淆不同类型的句柄
```

**新架构：**
```cpp
// 强类型封装
class Wrapper::Device {
    VkDevice mDevice{ VK_NULL_HANDLE };
public:
    [[nodiscard]] auto getDevice() const { return mDevice; }
};

class Wrapper::SwapChain {
    VkSwapchainKHR mSwapChain{ VK_NULL_HANDLE };
public:
    [[nodiscard]] auto getSwapChain() const { return mSwapChain; }
};
```

**优点：**
- **编译时类型检查**：避免类型错误
- **更好的IDE支持**：自动补全和错误提示
- **防止误用**：封装后的接口更安全

#### 4.2 现代C++特性

**旧架构：**
```cpp
// 使用C风格代码
VkDevice device = VK_NULL_HANDLE;
VkSwapchainKHR swapChain = VK_NULL_HANDLE;
```

**新架构：**
```cpp
// 使用现代C++特性
using Ptr = std::shared_ptr<Device>;
static Ptr create(const Instance::Ptr& instance, const WindowSurface::Ptr& surface) { 
    return std::make_shared<Device>(instance, surface); 
}

// 使用智能指针和RAII
Wrapper::Device::Ptr mDevice{ nullptr };
```

**优点：**
- **内存安全**：智能指针自动管理内存
- **异常安全**：RAII保证资源正确释放
- **代码简洁**：减少样板代码
- **性能优化**：移动语义和完美转发

### 5. 可维护性层面的改进

#### 5.1 代码组织

**旧架构：**
```
Chapter13-04/src/
├── VulkanDemoApp.h (132行，包含所有声明)
├── VulkanDemoApp.cpp (916行，包含所有实现)
├── 其他工具类文件...
```

**新架构：**
```
Chapter13-04-refactored/
├── application.h (94行，清晰的接口)
├── application.cpp (497行，专注应用逻辑)
├── vulkanWrapper/ (38个文件，模块化封装)
│   ├── device.h/cpp
│   ├── swapChain.h/cpp
│   └── ...
```

**优点：**
- **文件大小合理**：每个文件职责单一
- **易于导航**：清晰的目录结构
- **便于维护**：修改影响范围小

#### 5.2 接口设计

**旧架构：**
```cpp
// 暴露内部实现细节
class VulkanDemoApp {
public:
    VkDevice device;  // 直接暴露Vulkan句柄
    VkSwapchainKHR swapChain;
    // 大量公共成员变量
};
```

**新架构：**
```cpp
// 封装实现细节
class Application {
private:
    Wrapper::Device::Ptr mDevice{ nullptr };
    Wrapper::SwapChain::Ptr mSwapChain{ nullptr };
    
public:
    void run();  // 只暴露必要的接口
    void onMouseMove(double xpos, double ypos);
    void onKeyDown(int key, int action);
};
```

### 6. 性能层面的改进

#### 6.1 对象创建优化

**旧架构：**
```cpp
// 每次重建时重新创建所有对象
void recreateSwapChain() {
    vkDestroyRenderPass(device, renderPass, nullptr);
    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    // 重新创建所有对象
}
```

**新架构：**
```cpp
// 智能指针自动管理，只重建必要的对象
void recreateSwapChain() {
    cleanupSwapChain();  // 自动清理
    mSwapChain = Wrapper::SwapChain::create(mDevice, mWindow, mSurface, mCommandPool);
    // 其他对象可以复用
}
```

### 7. 可扩展性层面的改进

#### 7.1 插件化架构

**旧架构：**
```cpp
// 硬编码的功能
class VulkanDemoApp {
    void drawObject();  // 固定的绘制逻辑
};
```

**新架构：**
```cpp
// 可扩展的架构
class Application {
    Model::Ptr mModel{ nullptr };
    void drawObjects();  // 可以绘制多个对象
    
    // 可以轻松添加新的渲染功能
    void addModel(const Model::Ptr& model);
    void setRenderMode(RenderMode mode);
};
```

## 架构对比总结

| 特性 | 旧架构 | 新架构 |
|------|--------|--------|
| **架构设计** | 单体式设计 | 模块化设计 |
| **资源管理** | 手动管理 | RAII + 智能指针 |
| **错误处理** | 基础错误码 | 异常安全 + 验证层 |
| **代码组织** | 大文件集中 | 模块化分离 |
| **类型安全** | 原始指针 | 强类型封装 |
| **可维护性** | 中等 | 高 |
| **可扩展性** | 有限 | 良好 |
| **代码复用** | 低 | 高 |
| **团队协作** | 困难 | 容易 |
| **测试性** | 困难 | 容易 |

## 本次重构带来的核心价值

1. **开发效率提升**：模块化设计使得开发更加高效
2. **维护成本降低**：清晰的架构减少了维护难度
3. **错误率减少**：类型安全和自动资源管理减少了常见错误
4. **团队协作改善**：清晰的模块边界便于多人协作
5. **功能扩展容易**：插件化架构支持快速添加新功能
6. **代码质量提升**：现代C++特性提高了代码质量

这些改进使得重构后的项目更加健壮、可维护、可扩展，为后续的功能开发奠定了坚实的基础。

## 后记

1. 为了让书中使用glslang将glsl编译成spir-v的代码可用。使用SDK中的头和库文件编译后一直报错找不到符号，折腾了三四天翻了几十篇博客才搞定依赖并解决glsl编译成spirV代码
2. 报错`0xC0000374`:堆栈溢出（Stack Overflow）,可能是ObjObject::~ObjObject()中的`delete vdata;`而不是`delete[] vdata;`造成的
3. 《实战详解》里这么多有价值的代码不能运行我真是醉了 (눈‸눈)

![德莉莎](./Assets/德莉莎.jpg)