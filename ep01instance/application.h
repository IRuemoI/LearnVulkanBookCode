#pragma once

#include "base.h"
#include "vulkanWrapper/instance.h"

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
        GLFWwindow* mWindow{NULL}; //设置默认值
        Wrapper::Instance::Ptr mInstance{nullptr}; //智能指针必须设置默认值
    };
}
