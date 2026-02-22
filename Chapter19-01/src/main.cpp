#include <GLFW/glfw3.h>
#include "AudioManager.h"
#include "MyVulkanManager.h"
#include "Touch2D.h"
#include <stdio.h>

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    MyVulkanManager::window = glfwCreateWindow(1440, 810, "Vulkan 19.1", nullptr, nullptr);
    if (!MyVulkanManager::window)
    {
        printf("Failed to create GLFW window!\n");
        glfwTerminate();
        return -1;
    }

    glfwSetMouseButtonCallback(MyVulkanManager::window, Touch2D::mouse_button_callback);
    glfwSetCursorPosCallback(MyVulkanManager::window, Touch2D::cursor_position_callback);

    MyVulkanManager::loopDrawFlag = true;

    AudioManager::init();

    printf("Starting Vulkan application...\n");
    MyVulkanManager::doVulkan();

    while (!glfwWindowShouldClose(MyVulkanManager::window))
    {
        glfwPollEvents();
    }

    printf("Application shutting down...\n");
    AudioManager::cleanup();

    glfwDestroyWindow(MyVulkanManager::window);
    glfwTerminate();

    return 0;
}
