#pragma once

#include "base.h"
#include "vulkanWrapper/instance.h"
#include "vulkanWrapper/device.h"
#include "vulkanWrapper/window.h"
#include "vulkanWrapper/windowSurface.h"
#include "vulkanWrapper/swapChain.h"
#include "vulkanWrapper/shader.h"
#include "vulkanWrapper/pipeline.h"
#include "vulkanWrapper/renderPass.h"

namespace FF
{
    const int WIDTH = 800;
    const int HEIGHT = 600;

    class Application
    {
    public:
        //设置默认构造函数
        Application() = default;

        //设置默认析构函数
        ~Application() = default;

        //运行程序
        void run();

    private:
        //初始化窗体
        void initWindow();

        //初始化Vulkan
        void initVulkan();

        //程序主循环
        void mainLoop();

        //清理资源
        void cleanUp();

    private:
        //创建图形流水线
        void createPipeline();
        //创建渲染过程
        void createRenderPass();

    private:
        Wrapper::Window::Ptr mWindow{nullptr}; //将GLFWwindow类型封装成自定义类型
        Wrapper::Instance::Ptr mInstance{nullptr}; //智能指针必须设置默认值
        Wrapper::Device::Ptr mDevice{nullptr};
        Wrapper::WindowSurface::Ptr mSurface{nullptr};
        Wrapper::SwapChain::Ptr mSwapChain{nullptr};
        Wrapper::Pipeline::Ptr mPipeline{nullptr};
        Wrapper::RenderPass::Ptr mRenderPass{nullptr};
    };
}
