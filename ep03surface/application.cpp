#include "application.h"

namespace FF
{
    void Application::run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanUp();
    }

    void Application::initWindow()
    {
        mWindow = Wrapper::Window::create(WIDTH, HEIGHT); //从自定义窗口类中创建窗口
    }

    void Application::initVulkan()
    {
        mInstance = Wrapper::Instance::create(true); //获取Vulkan实例的智能指针
        mSurface = Wrapper::WindowSurface::create(mInstance, mWindow); //创建窗口表面

        mDevice = Wrapper::Device::create(mInstance, mSurface); //创建物理设备
    }

    void Application::mainLoop()
    {
        //只有在接收到关闭窗口的消息时才退出循环
        while (!mWindow->shouldClose())
        {
            mWindow->pollEvents();
        }
    }

    void Application::cleanUp()
    {
        //在析构时需要将智能指针置空
        mDevice.reset();
        mSurface.reset();
        mInstance.reset();
        mWindow.reset();
    }
}
