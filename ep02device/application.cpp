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
        glfwInit(); // 初始化glfw

        //设置环境，关掉opengl API 并且禁止窗口改变大小
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        //创建窗口
        mWindow = glfwCreateWindow(WIDTH, HEIGHT, "vulkan window", nullptr, nullptr);
        if (!mWindow)
        {
            std::cerr << "Error: failed to create window" << std::endl;
        }
    }

    void Application::initVulkan()
    {
        mInstance = Wrapper::Instance::create(true); //获取Vulkan实例的智能指针
        mDevice = Wrapper::Device::create(mInstance); //创建物理设备
    }

    void Application::mainLoop()
    {
        //只有在接收到关闭窗口的消息时才退出循环
        while (!glfwWindowShouldClose(mWindow))
        {
            glfwPollEvents(); //分发事件
        }
    }

    void Application::cleanUp()
    {
        //在析构时需要将智能指针置空
        mDevice.reset();
        mInstance.reset();
        glfwDestroyWindow(mWindow); //销毁窗口
        glfwTerminate(); ///终止glfw
    }
}
