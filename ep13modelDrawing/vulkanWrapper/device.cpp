#include "device.h"

namespace FF::Wrapper
{
    Device::Device(Instance::Ptr instance, WindowSurface::Ptr surface)
    {
        mInstance = instance;
        mSurface = surface;
        pickPhysicalDevice();
        initQueueFamilies(mPhysicalDevice);
        createLogicalDevice();
    }

    Device::~Device()
    {
        vkDestroyDevice(mDevice, nullptr);
        mSurface.reset();
        mInstance.reset();
    }

    void Device::pickPhysicalDevice()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(mInstance->getInstance(), &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            throw std::runtime_error("Error:failed to enumeratePhysicalDevice");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(mInstance->getInstance(), &deviceCount, devices.data());

        std::multimap<int, VkPhysicalDevice> candidates;
        for (const auto& device : devices)
        {
            int score = rateDevice(device);
            candidates.insert(std::make_pair(score, device));
        }

        if (candidates.rbegin()->first > 0 && isDeviceSuitable(candidates.rbegin()->second))
        {
            mPhysicalDevice = candidates.rbegin()->second;
        }

        if (mPhysicalDevice == VK_NULL_HANDLE)
        {
            throw std::runtime_error("Error:failed to get physical device");
        }
    }

    int Device::rateDevice(VkPhysicalDevice device)
    {
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
        // macOS平台：M系列芯片只有独显，所以为独立显卡和Apple GPU提供高分，否则会找不到物理设备
        if (deviceProp.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ||
            // 检查是否为Apple GPU (通过设备名称识别)
            std::string(deviceProp.deviceName).find("Apple") != std::string::npos)
        {
            score += 1000;
        }
        // 确保Apple GPU至少有基础分数
        score = std::max(score, 100);
#elif defined(_WIN32)
        // Windows平台：仅为独立显卡提供高分
        if (deviceProp.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            score += 1000;
        }
#endif

        score += deviceProp.limits.maxImageDimension2D;

        // 在macOS上放宽几何着色器要求
#if defined(__APPLE__)
        if (!deviceFeatures.geometryShader)
        {
            return 0;
        }
#endif
        return score;
    }

    bool Device::isDeviceSuitable(VkPhysicalDevice device)
    {
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
        return deviceFeatures.samplerAnisotropy;
#else
        // 默认返回false,其他平台不支持
        return false;
#endif
    }

    void Device::initQueueFamilies(VkPhysicalDevice device)
    {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT))
            {
                mGraphicQueueFamily = i;
            }

            // 寻找支持显示的队列族
            VkBool32 presentSupport = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, mSurface->getSurface(), &presentSupport);

            if (presentSupport)
            {
                mPresentQueueFamily = i;
            }

            if (isQueueFamilyComplete())
            {
                break;
            }

            ++i;
        }
    }

    void Device::createLogicalDevice()
    {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

        std::set<uint32_t> queueFamilies = {mGraphicQueueFamily.value(), mPresentQueueFamily.value()};

        float queuePriority = 1.0;

        for (uint32_t queueFamily : queueFamilies)
        {
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

        std::vector<const char*> extensions = deviceRequiredExtensions;
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
        if (mInstance->getEnableValidationLayer())
        {
            deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
        {
            deviceCreateInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mDevice) != VK_SUCCESS)
        {
            throw std::runtime_error("Error:failed to create logical device");
        }

        vkGetDeviceQueue(mDevice, mGraphicQueueFamily.value(), 0, &mGraphicQueue);
        vkGetDeviceQueue(mDevice, mPresentQueueFamily.value(), 0, &mPresentQueue);
    }

    bool Device::isQueueFamilyComplete()
    {
        return mGraphicQueueFamily.has_value() && mPresentQueueFamily.has_value();
    }

    VkSampleCountFlagBits Device::getMaxUsableSampleCount()
    {
        VkPhysicalDeviceProperties props{};
        vkGetPhysicalDeviceProperties(mPhysicalDevice, &props);

        VkSampleCountFlags counts = std::min(
            props.limits.framebufferColorSampleCounts,
            props.limits.framebufferDepthSampleCounts);

        if (counts & VK_SAMPLE_COUNT_64_BIT)
        {
            return VK_SAMPLE_COUNT_64_BIT;
        }
        if (counts & VK_SAMPLE_COUNT_32_BIT)
        {
            return VK_SAMPLE_COUNT_32_BIT;
        }
        if (counts & VK_SAMPLE_COUNT_16_BIT)
        {
            return VK_SAMPLE_COUNT_16_BIT;
        }
        if (counts & VK_SAMPLE_COUNT_8_BIT)
        {
            return VK_SAMPLE_COUNT_8_BIT;
        }
        if (counts & VK_SAMPLE_COUNT_4_BIT)
        {
            return VK_SAMPLE_COUNT_4_BIT;
        }
        if (counts & VK_SAMPLE_COUNT_2_BIT)
        {
            return VK_SAMPLE_COUNT_2_BIT;
        }

        return VK_SAMPLE_COUNT_1_BIT;
    }
}
