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
#include "vulkanWrapper/descriptorSetLayout.h"
#include "vulkanWrapper/descriptorPool.h"
#include "vulkanWrapper/descriptorSet.h"
#include "vulkanWrapper/description.h"
#include "uniformManager.h"
#include "vulkanWrapper/image.h"
#include "vulkanWrapper/sampler.h"
#include "texture/texture.h"

#include "robot.h"

namespace FF
{
    // Chapter16-02: 机器人动画系统

    class Application : public std::enable_shared_from_this<Application>
    {
    public:
        using Ptr = std::shared_ptr<Application>;
        static Ptr create() { return std::make_shared<Application>(); }

        Application() = default;

        ~Application() = default;

        void run();

        void onMouseMove(double xpos, double ypos);
        void onKeyDown(int key, int action);

    private:
        void initWindow();

        void initVulkan();

        void mainLoop();

        void render();

        void cleanUp();

    private:
        void createPipeline();
        void createRenderPass();
        void createCommandBuffers();
        void createSyncObjects();

        //重建交换链:  当窗口大小发生变化的时候，交换链也要发生变化
        void recreateSwapChain();

        void cleanupSwapChain();

        // 处理输入
        void processInput();

        // 初始化纹理和模型
        void initTextures();
        void initRobot();

        // 绘制对象
        void drawObjects();

        // 更新动画
        void updateAnimation();

    private:
        unsigned int mWidth{800};
        unsigned int mHeight{600};

    private:
        int mCurrentFrame{0};
        Wrapper::Window::Ptr mWindow{nullptr};
        Wrapper::Instance::Ptr mInstance{nullptr};
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

        UniformManager::Ptr mUniformManager{nullptr};

        // 机器人系统
        Robot::Ptr mRobot{nullptr};
        VPMatrices mVPMatrices;

        // 鼠标控制相关
        double mLastMouseX{0.0};
        double mLastMouseY{0.0};
        bool mFirstMouse{true};
        float mYaw{-90.0f};
        float mPitch{0.0f};

        // 相机位置
        glm::vec3 mCameraPos{0.0f, 5.0f, 20.0f};
        glm::vec3 mCameraFront{0.0f, 0.0f, -1.0f};
        glm::vec3 mCameraUp{0.0f, 1.0f, 0.0f};

        // 动画相关
        float mAnimationTime{0.0f};
        bool mAnimationEnabled{true};
    };
}
