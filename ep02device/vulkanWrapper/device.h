#pragma once

#include "../base.h"
#include "instance.h"

namespace FF::Wrapper
{
    class Device
    {
    public:
        using Ptr = std::shared_ptr<Device>;
        static Ptr create(Instance::Ptr instance) { return std::make_shared<Device>(instance); }

        Device(Instance::Ptr instance);

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

    private:
        //物理设备
        VkPhysicalDevice mPhysicalDevice{VK_NULL_HANDLE};
        Instance::Ptr mInstance{nullptr};

        std::optional<uint32_t> mGraphicQueueFamily; //存储当前渲染任务队列族的id;optional可以理解为CheckBox
        VkQueue mGraphicQueue{VK_NULL_HANDLE}; //图形队列句柄

        //逻辑设备
        VkDevice mDevice{VK_NULL_HANDLE};
    };
}
