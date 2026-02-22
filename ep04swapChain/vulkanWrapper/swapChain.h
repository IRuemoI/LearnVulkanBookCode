#pragma once

#include "../base.h"
#include "device.h"
#include "window.h"
#include "windowSurface.h"

namespace FF::Wrapper
{
    //交换链支持信息
    struct SwapChainSupportInfo
    {
        VkSurfaceCapabilitiesKHR mCapabilities; //表面能力
        std::vector<VkSurfaceFormatKHR> mFormats; //表面支持的格式列表
        std::vector<VkPresentModeKHR> mPresentModes; //表面支持的显示模式列表
    };

    class SwapChain
    {
    public:
        using Ptr = std::shared_ptr<SwapChain>;

        static Ptr create(const Device::Ptr& device, const Window::Ptr& window, const WindowSurface::Ptr& surface)
        {
            return std::make_shared<SwapChain>(device, window, surface);
        }

        SwapChain(const Device::Ptr& device, const Window::Ptr& window, const WindowSurface::Ptr& surface);

        ~SwapChain();

        //查询交换链支持
        SwapChainSupportInfo querySwapChainSupportInfo();

        //选择交换链格式
        VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        //选择交换链显示模式
        VkPresentModeKHR chooseSurfacePresentMode(const std::vector<VkPresentModeKHR>& availablePresenstModes);

        //选择交换链的显示范围
        VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    private:
        //创建图像视图
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags,
                                    uint32_t mipLevels = 1);

    private:
        VkSwapchainKHR mSwapChain{VK_NULL_HANDLE};

        VkFormat mSwapChainFormat; //交换链的格式
        VkExtent2D mSwapChainExtent; //交换链的显示范围
        uint32_t mImageCount{0}; //交换链中的图像数量

        //vkimage由swapchain创建，销毁也要交给swapchain
        std::vector<VkImage> mSwapChainImages{};

        //对图像的管理器。管理框架
        std::vector<VkImageView> mSwapChainImageViews{};


        Device::Ptr mDevice{nullptr};
        Window::Ptr mWindow{nullptr};
        WindowSurface::Ptr mSurface{nullptr};
    };
}
