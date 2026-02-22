#pragma once

#include "../base.h"

namespace FF::Wrapper
{
    class Instance
    {
    public:
        //将类型std::shared_ptr<Instance>简写为Ptr,shared_ptr的功能是引用计数，当引用计数为0时，自动释放内存
        using Ptr = std::shared_ptr<Instance>;

        //创建并返回一个Instance实例的智能指针
        static Ptr create(bool enableValidationLayer) { return std::make_shared<Instance>(enableValidationLayer); }

        Instance(bool enableValidationLayer);

        ~Instance();

        //输出当前Vulkan示例可用的所有扩展
        void printAvailableExtensions();

        //获取需要开启的实例级扩展
        std::vector<const char*> getRequiredExtensions();

        //检查所有需要启用验证层都被支持
        bool checkValidationLayerSupport();
        //设置调试器
        void setupDebugger();

        //获取实例
        [[nodiscard]] VkInstance getInstance() const { return mInstance; } //[[nodiscard]]表示不应该忽略函数的返回值

        [[nodiscard]] bool getEnableValidationLayer() const { return mEnableValidationLayer; }

    private:
        VkInstance mInstance{VK_NULL_HANDLE};
        bool mEnableValidationLayer{false}; //是否启用校验层
        VkDebugUtilsMessengerEXT mDebugger{VK_NULL_HANDLE}; //调试器对象指针
    };
}
