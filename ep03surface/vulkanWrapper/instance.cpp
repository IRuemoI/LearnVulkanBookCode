#include "instance.h"

namespace FF::Wrapper
{
    //校验层回调函数
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallBack(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, //消息级别
        VkDebugUtilsMessageTypeFlagsEXT messageType, //消息类型
        const VkDebugUtilsMessengerCallbackDataEXT* pMessageData, //错误信息
        void* pUserData //用户数据
    )
    {
        std::cout << "ValidationLayer: " << pMessageData->pMessage << std::endl;

        return VK_FALSE; //返回false，表示不终止程序
    }

    //辅助函数，获取创建调试器函数的地址
    static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                                 const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                 const VkAllocationCallbacks* pAllocator,
                                                 VkDebugUtilsMessengerEXT* debugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance, "vkCreateDebugUtilsMessengerEXT"); //获取创建调试器的函数地址

        if (func != nullptr)
        {
            //如果获取函数地址成功
            return func(instance, pCreateInfo, pAllocator, debugMessenger); //调用创建调试器的函数
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    //辅助函数，获取销毁调试器函数的地址
    static void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                              VkDebugUtilsMessengerEXT debugMessenger,
                                              const VkAllocationCallbacks* pAllocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance, "vkDestroyDebugUtilsMessengerEXT"); //获取销毁调试器的函数地址

        if (func != nullptr)
        {
            //如果获取函数地址成功
            return func(instance, debugMessenger, pAllocator); //调用销毁调试器的函数
        }
    }


    Instance::Instance(bool enableValidationLayer)
    {
        mEnableValidationLayer = enableValidationLayer; //获取当前是否需要启用校验层

        //如果需要启用校验层但是需要启用验证层中包含有不支持的项，则报错
        if (mEnableValidationLayer && !checkValidationLayerSupport())
        {
            throw std::runtime_error("Error: validation layer is not supported");
        }

        //打印Vulkan实例支持的扩展
        printAvailableExtensions();

        VkApplicationInfo appInfo = {}; //创建应用程序信息结构体
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; //设置本结构体的类型
        appInfo.pApplicationName = "vulkanLession"; //设置应用程序名称
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); //设置应用程序版本
        appInfo.pEngineName = "NO ENGINE"; //设置引擎名称,当前为无
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0); //设置引擎版本
        appInfo.apiVersion = VK_API_VERSION_1_0; //设置应用程序所需的最低VulkanAPI版本

        VkInstanceCreateInfo instCreateInfo = {}; //创建实例创建信息结构体
        instCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO; //设置本结构体的类型
        instCreateInfo.pApplicationInfo = &appInfo; //设置应用程序信息结构体

        //扩展相关
        auto extensions = getRequiredExtensions(); //获取需要启用的扩展名数组
        instCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size()); //设置扩展数量
        instCreateInfo.ppEnabledExtensionNames = extensions.data(); //设置扩展名数组

#if defined(__APPLE__)
        instCreateInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

        //校验层相关
        if (mEnableValidationLayer)
        {
            instCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size()); //在实例创建参数中设置需要启用的校验层数量
            instCreateInfo.ppEnabledLayerNames = validationLayers.data(); //设置需要启用的校验层名称数组
        }
        else
        {
            instCreateInfo.enabledLayerCount = 0; //禁用校验层
        }

        //如果创建实例不成功，则报错
        if (vkCreateInstance(&instCreateInfo, nullptr, &mInstance) != VK_SUCCESS)
        {
            throw std::runtime_error("Error:failed to create instance");
        }

        //安装调试器
        setupDebugger();
    }

    Instance::~Instance()
    {
        if (mEnableValidationLayer)
        {
            //如果启用了校验层
            DestroyDebugUtilsMessengerEXT(mInstance, mDebugger, nullptr); //在销毁实例之前先销毁调试器
        }

        vkDestroyInstance(mInstance, nullptr); //销毁实例
    }

    void Instance::printAvailableExtensions()
    {
        uint32_t extensionCount = 0; //创建一个数量变量
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr); //第一次调用用于获取数量

        std::vector<VkExtensionProperties> extensions(extensionCount); //创建一个大小为实际扩展数量的vector容器
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data()); //第二次调用用于获取扩展信息

        std::cout << "Available extensions:" << std::endl;

        //遍历打印当前Vulkan实例支持的所有扩展名称
        for (const auto& extension : extensions)
        {
            std::cout << extension.extensionName << std::endl;
        }
    }

    std::vector<const char*> Instance::getRequiredExtensions()
    {
        uint32_t glfwExtensionCount = 0; //创建一个glfw扩展数量变量

        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); //获取glfw扩展名称数组

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        //把glfwExtensions数组起始地址到glfwExtensions+glfwExtensionCount地址范围的内容复制到vector容器中

        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); //添加调试扩展

#if defined(__APPLE__)
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
#endif

        return extensions;
    }

    bool Instance::checkValidationLayerSupport()
    {
        uint32_t layerCount = 0; //创建一个数量变量
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr); //第一次调用用于获取数量

        std::vector<VkLayerProperties> availableLayers(layerCount); //创建一个大小为实际支持的验证层数量的vector容器
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()); ////第二次调用用于获取扩展信息

        //双重循环检查在可用验证层列表中，是否存在指定的验证层
        for (const auto& layerName : validationLayers)
        {
            bool layerFound = false;

            for (const auto& layerProp : availableLayers)
            {
                if (std::strcmp(layerName, layerProp.layerName) == 0)
                {
                    //字符串相等返回0
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
            {
                return false;
            }
        }

        return true;
    }

    void Instance::setupDebugger()
    {
        //如果没有启用校验层，则返回
        if (!mEnableValidationLayer) { return; }

        VkDebugUtilsMessengerCreateInfoEXT createInfo = {}; //创建调试信息
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT; //设置本结构体的类型
        createInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | //显示详细消息
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | //显示警告消息
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT; //显示错误消息

        createInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | //显示一般类型消息
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | //显示验证类型消息
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT; //显示性能类型消息

        createInfo.pfnUserCallback = debugCallBack; //设置我们自定义的调用函数
        createInfo.pUserData = nullptr;

        //创建调试器
        if (CreateDebugUtilsMessengerEXT(mInstance, &createInfo, nullptr, &mDebugger) != VK_SUCCESS)
        {
            throw std::runtime_error("Error:failed to create debugger");
        }
    }
}
