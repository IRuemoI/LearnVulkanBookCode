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
#include "vulkanWrapper/commandPool.h"
#include "vulkanWrapper/commandBuffer.h"
#include "vulkanWrapper/semaphore.h"
#include "vulkanWrapper/fence.h"
#include "vulkanWrapper/buffer.h"

#include "model.h"

namespace FF
{
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
        //渲染过程(绘制指令录制过程)
        void render();

        //清理资源
        void cleanUp();

    private:
        //创建图形流水线
        void createPipeline();
        //创建渲染过程
        void createRenderPass();
        //创建命令缓冲
        void createCommandBuffers();
        //创建同步对象
        void createSyncObjects();

        //重建交换链:  当窗口大小发生变化的时候，交换链也要发生变化，Frame View Pipeline RenderPass Sync
        void recreateSwapChain();
        //清理交换链
        void cleanupSwapChain();

    private:
        unsigned int mWidth{800};
        unsigned int mHeight{600};

    private:
        int mCurrentFrame{0};
        Wrapper::Window::Ptr mWindow{nullptr}; //将GLFWwindow类型封装成自定义类型
        Wrapper::Instance::Ptr mInstance{nullptr}; //智能指针必须设置默认值
        Wrapper::Device::Ptr mDevice{nullptr};
        Wrapper::WindowSurface::Ptr mSurface{nullptr};
        Wrapper::SwapChain::Ptr mSwapChain{nullptr};
        Wrapper::Pipeline::Ptr mPipeline{nullptr};
        Wrapper::RenderPass::Ptr mRenderPass{nullptr};
        Wrapper::CommandPool::Ptr mCommandPool{nullptr};

        std::vector<Wrapper::CommandBuffer::Ptr> mCommandBuffers{};

        std::vector<Wrapper::Semaphore::Ptr> mImageAvailableSemaphores{};
        std::vector<Wrapper::Semaphore::Ptr> mRenderFinishedSemaphores{};
        std::vector<Wrapper::Fence::Ptr> mFences{};

        Model::Ptr mModel{nullptr};
    };
}
