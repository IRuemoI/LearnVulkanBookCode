#pragma once

#include "../base.h"
#include "instance.h"
#include "windowSurface.h"

namespace FF::Wrapper
{
    //需要启用的设备级别的扩展列表
    const std::vector<const char*> deviceRequiredExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    class Device
    {
    public:
        using Ptr = std::shared_ptr<Device>;

        static Ptr create(Instance::Ptr instance, WindowSurface::Ptr surface)
        {
            return std::make_shared<Device>(instance, surface);
        }

        Device(Instance::Ptr instance, WindowSurface::Ptr surface);

        ~Device();

        //挑选物理设备
        void pickPhysicalDevice();

        //评价物理设备(对比各个硬件的参数以及特性支持)
        int rateDevice(VkPhysicalDevice device);

        //当前设备是否合适
        bool isDeviceSuitable(VkPhysicalDevice device);

        //初始化队列族
        void initQueueFamilies(VkPhysicalDevice device);

        //创建逻辑设备
        void createLogicalDevice();

        //检查是否所有需要的队列族都已就绪
        bool isQueueFamilyComplete();

        [[nodiscard]] auto getDevice() const { return mDevice; }
        [[nodiscard]] auto getPhysicalDevice() const { return mPhysicalDevice; }

        [[nodiscard]] auto getGraphicQueueFamily() const { return mGraphicQueueFamily; }
        [[nodiscard]] auto getPresentQueueFamily() const { return mPresentQueueFamily; }

    private:
        //物理设备
        VkPhysicalDevice mPhysicalDevice{VK_NULL_HANDLE};
        Instance::Ptr mInstance{nullptr};
        WindowSurface::Ptr mSurface{nullptr};

        std::optional<uint32_t> mGraphicQueueFamily; //存储当前渲染任务队列族的ID;optional可以理解为CheckBox
        VkQueue mGraphicQueue{VK_NULL_HANDLE}; //图形队列句柄

        std::optional<uint32_t> mPresentQueueFamily; //存储当前渲染显示队列族的ID;
        VkQueue mPresentQueue{VK_NULL_HANDLE}; //呈现队列句柄

        //逻辑设备
        VkDevice mDevice{VK_NULL_HANDLE};
    };
}
