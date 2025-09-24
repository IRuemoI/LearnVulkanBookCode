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
        uint32_t deviceCount = 0; //定义设备数量变量
        vkEnumeratePhysicalDevices(mInstance->getInstance(), &deviceCount, nullptr); //获取设备数量

        if (deviceCount == 0)
        {
            //没有可用的设备
            throw std::runtime_error("Error:failed to enumeratePhysicalDevice");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount); //创建可用物理设备数量大小的列表
        vkEnumeratePhysicalDevices(mInstance->getInstance(), &deviceCount, devices.data()); //将物理设备信息设置到devices列表中

        //定义一个支持多个个相同Key的字典,会默认根据key升序排序
        std::multimap<int, VkPhysicalDevice> candidates;
        for (const auto& device : devices)
        {
            int score = rateDevice(device); //给这个设备进行打分
            candidates.insert(std::make_pair(score, device)); //将分数和设备加入字典中
        }

        //candidates.rbegin()->first对应key
        //candidates.rbegin()->second对应value 
        //如果当前评分最高的设备符合条件，则返回该设备
        if (candidates.rbegin()->first > 0 && isDeviceSuitable(candidates.rbegin()->second))
        {
            mPhysicalDevice = candidates.rbegin()->second;
        }

        //如果没有满足条件的设备，则抛出异常
        if (mPhysicalDevice == VK_NULL_HANDLE)
        {
            throw std::runtime_error("Error:failed to get physical device");
        }
    }

    int Device::rateDevice(VkPhysicalDevice device)
    {
        int score = 0;

        VkPhysicalDeviceProperties deviceProp; //定义设备属性变量(如设备名称,类型,支持vulkan的版本)
        vkGetPhysicalDeviceProperties(device, &deviceProp); //获取该设备的属性信息

        VkPhysicalDeviceFeatures deviceFeatures; //定义设备特性变量(纹理压缩,浮点数运算,多视口渲染等特性的支持)
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures); //获取该设备的特性信息

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
        if (deviceProp.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            score += 1000;
        }
#endif

        //根据最大图像尺寸的支持
        score += deviceProp.limits.maxImageDimension2D;

        // 在macOS上放宽几何着色器要求
#if defined(__APPLE__)
        // 不强制要求几何着色器，因为Apple GPU可能不支持
#else
        //如果不支持几何着色器，该设备设置为0分
        if (!deviceFeatures.geometryShader)
        {
            return 0;
        }
#endif

        return score;
    }

    bool Device::isDeviceSuitable(VkPhysicalDevice device)
    {
        VkPhysicalDeviceProperties deviceProp; //定义设备属性变量(如设备名称,类型,支持vulkan的版本)
        vkGetPhysicalDeviceProperties(device, &deviceProp); //获取该设备的属性信息

        VkPhysicalDeviceFeatures deviceFeatures; //定义设备特性变量(纹理压缩,浮点数运算,多视口渲染等特性的支持)
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures); //获取该设备的特性信息

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

    void Device::initQueueFamilies(VkPhysicalDevice device)
    {
        uint32_t queueFamilyCount = 0; //定义队列族数量变量
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr); //获取设备队列族数量

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount); //创建可用队列族数量大小的列表
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data()); //将队列族信息设置到队列族列表中

        int i = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            //对于所有的队列族
            if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT))
            {
                //如果当前队列族包含的队列数大于0且当前队列族支持图形渲染
                mGraphicQueueFamily = i; //将当前队列族索引保存下来
            }

            //寻找支持显示的队列族
            VkBool32 presentSupport = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, mSurface->getSurface(), &presentSupport); //获取当前队列族是否支持显示

            if (presentSupport)
            {
                //如果当前队列族支持显示
                mPresentQueueFamily = i; //将当前队列族索引保存下来
            }

            //如果找到了两个队列族，则退出循环
            if (isQueueFamilyComplete())
            {
                break;
            }

            ++i; //索引加1
        }
    }

    void Device::createLogicalDevice()
    {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos; //创建一个队列创建信息列表

        std::set<uint32_t> queueFamilies = {mGraphicQueueFamily.value(), mPresentQueueFamily.value()};
        //将这个两个队列族的索引放在集合容器中去重

        float queuePriority = 1.0; //定义队列的优先级

        for (uint32_t queueFamily : queueFamilies)
        {
            //对于集合中所有的队列族索引
            //填写创建信息
            VkDeviceQueueCreateInfo queueCreateInfo = {}; //创建队列创建信息
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; //设置结构类型
            queueCreateInfo.queueFamilyIndex = queueFamily; //设置包含图形队列的队列族索引
            queueCreateInfo.queueCount = 1; //设置队列数量
            queueCreateInfo.pQueuePriorities = &queuePriority; //设置队列的优先级

            queueCreateInfos.push_back(queueCreateInfo); //添加队列创建信息到队列创建信息列表中
        }


        //填写逻辑设备创建信息
        VkPhysicalDeviceFeatures deviceFeatures = {}; //当前不启用任何特性
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        // 添加macOS平台必需的扩展
        std::vector<const char*> extensions = deviceRequiredExtensions;
#if defined(__APPLE__)
        extensions.push_back("VK_KHR_portability_subset");
#endif

        VkDeviceCreateInfo deviceCreateInfo = {}; //创建逻辑设备创建信息
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO; //设置结构类型
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data(); //设置多个队列创建信息
        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()); //设置多个队列创建数量
        deviceCreateInfo.pEnabledFeatures = &deviceFeatures; //设置逻辑设备特性
        deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size()); //设置逻辑设备扩展数量
        deviceCreateInfo.ppEnabledExtensionNames = extensions.data(); //设置逻辑设备扩展名称列表

        //如果启用了验证层
        if (mInstance->getEnableValidationLayer())
        {
            deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size()); //设置逻辑设备验证层数量
            deviceCreateInfo.ppEnabledLayerNames = validationLayers.data(); //设置逻辑设备验证层
        }
        else
        {
            deviceCreateInfo.enabledLayerCount = 0; //设置逻辑设备验证层数量为0
        }

        //创建逻辑设备,如果创建不成功则抛出异常
        if (vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mDevice) != VK_SUCCESS)
        {
            throw std::runtime_error("Error:failed to create logical device");
        }

        vkGetDeviceQueue(mDevice, mGraphicQueueFamily.value(), 0, &mGraphicQueue); //创建具有图形能力的队列
        vkGetDeviceQueue(mDevice, mPresentQueueFamily.value(), 0, &mPresentQueue); //创建具有呈现能力的队列
    }

    bool Device::isQueueFamilyComplete()
    {
        //如果支持图形和显示队列的队列族都存在就返回真
        return mGraphicQueueFamily.has_value() && mPresentQueueFamily.has_value();
    }

    VkSampleCountFlagBits Device::getMaxUsableSampleCount()
    {
        VkPhysicalDeviceProperties props{};
        vkGetPhysicalDeviceProperties(mPhysicalDevice, &props);

        VkSampleCountFlags counts = std::min(
            props.limits.framebufferColorSampleCounts,
            props.limits.framebufferDepthSampleCounts
        );

        if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
        if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
        if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
        if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
        if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
        if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

        return VK_SAMPLE_COUNT_1_BIT;
    }
}
