#include <vulkan/vulkan.h>
#include "MatrixState3D.h"
#include "MatrixState2D.h"
#include "MyVulkanManager.h"
#include "FileUtil.h"
#include "TextureManager.h"
#include "HelpFunction.h"
#include <thread>
#include <iostream>
#include <assert.h>
#include <chrono>
#include "ThreadTask.h"
#include "LoadUtil.h"
#include "CameraUtil.h"
#include "FPSUtil.h"
#include <time.h>
#include "Square3D.h"
#include "My3DLayer.h"
#include "ZFTManager.h"
#include "DiCiBox.h"
#include "MyDraw.h"
#include "PathData.h"
#include <GLFW/glfw3.h>
#define USED_GPU_INDEX 0

//===================================Static member initialization============================================

GLFWwindow* MyVulkanManager::window = nullptr;
bool MyVulkanManager::loopDrawFlag = true;
std::vector<const char*> MyVulkanManager::instanceExtensionNames;
VkInstance MyVulkanManager::instance;
uint32_t MyVulkanManager::gpuCount;
std::vector<VkPhysicalDevice> MyVulkanManager::gpus;
uint32_t MyVulkanManager::queueFamilyCount;
std::vector<VkQueueFamilyProperties> MyVulkanManager::queueFamilyprops;
uint32_t MyVulkanManager::queueGraphicsFamilyIndex;
VkQueue MyVulkanManager::queueGraphics;
uint32_t MyVulkanManager::queuePresentFamilyIndex;
std::vector<const char*> MyVulkanManager::deviceExtensionNames;
VkDevice MyVulkanManager::device;
VkCommandPool MyVulkanManager::cmdPool;
VkCommandBuffer MyVulkanManager::cmdBuffer;
VkCommandBufferBeginInfo MyVulkanManager::cmd_buf_info;
VkCommandBuffer MyVulkanManager::cmd_bufs[1];
VkSubmitInfo MyVulkanManager::submit_info[1];
uint32_t MyVulkanManager::screenWidth;
uint32_t MyVulkanManager::screenHeight;
VkSurfaceKHR MyVulkanManager::surface;
std::vector<VkFormat> MyVulkanManager::formats;
VkSurfaceCapabilitiesKHR MyVulkanManager::surfCapabilities;
uint32_t MyVulkanManager::presentModeCount;
std::vector<VkPresentModeKHR> MyVulkanManager::presentModes;
VkExtent2D MyVulkanManager::swapchainExtent;
VkSwapchainKHR MyVulkanManager::swapChain;
uint32_t MyVulkanManager::swapchainImageCount;
std::vector<VkImage> MyVulkanManager::swapchainImages;
std::vector<VkImageView> MyVulkanManager::swapchainImageViews;
VkFormat MyVulkanManager::depthFormat;
VkFormatProperties MyVulkanManager::depthFormatProps;
VkImage MyVulkanManager::depthImage;
VkPhysicalDeviceMemoryProperties MyVulkanManager::memoryroperties;
VkDeviceMemory MyVulkanManager::memDepth;
VkImageView MyVulkanManager::depthImageView;
VkSemaphore MyVulkanManager::imageAcquiredSemaphore;
uint32_t MyVulkanManager::currentBuffer;
VkRenderPass MyVulkanManager::renderPass;
VkClearValue MyVulkanManager::clear_values[2];
VkRenderPassBeginInfo MyVulkanManager::rp_begin;
VkFence MyVulkanManager::taskFinishFence;
VkPresentInfoKHR MyVulkanManager::present;

VkFramebuffer* MyVulkanManager::framebuffers;

ShaderQueueSuit_CommonTexLight* MyVulkanManager::sqsCTL;
ShaderQueueSuit_Dashboard2D* MyVulkanManager::sqsD2D;

Dashboard2DObject* MyVulkanManager::d2dA;

ObjObject* My3DLayer::planeForDraw;
ObjObject* My3DLayer::sp3Tree[TREE_NUMBER];
ObjObject* My3DLayer::sp3Dici[DICI_NUMBER];
ObjObject* My3DLayer::sp3JiaziLeft[JIAZI_NUMBER];
ObjObject* My3DLayer::sp3JiaziRight[JIAZI_NUMBER];
ObjObject* My3DLayer::sp3JiaziLeftClose[JIAZI_NUMBER];
ObjObject* My3DLayer::sp3JiaziRightClose[JIAZI_NUMBER];
ObjObject* My3DLayer::sp3DiaoLuo[DIAOLUO_NUMBER];
ObjObject* My3DLayer::sp3DiaoLuoshadow[DIAOLUO_NUMBER];
ObjObject* My3DLayer::sp3YiDong[YIDONG_NUMBER];
ObjObject* My3DLayer::mainBox;
ObjObject* My3DLayer::attachBox[ATTACH_BOX_NUMBER];
ObjObject* My3DLayer::attachBox2[ATTACH_BOX_NUMBER];
ObjObject* My3DLayer::attachBox3[ATTACH_BOX_NUMBER];
ObjObject* My3DLayer::attachBox4[ATTACH_BOX_NUMBER];
ObjObject* My3DLayer::attachBox5[ATTACH_BOX_NUMBER];
ObjObject* My3DLayer::attachBox6[ATTACH_BOX_NUMBER];
ObjObject* My3DLayer::attachBox7[ATTACH_BOX_NUMBER];
ObjObject* My3DLayer::attachBox8[ATTACH_BOX_NUMBER];
ObjObject* My3DLayer::qizi;
ObjObject* My3DLayer::qizigun[QIZIGUN_NUMBER];
ObjObject* My3DLayer::tanshe[TANSHE_NUMBER];

void MyVulkanManager::init_vulkan_instance()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    for (uint32_t i = 0; i < glfwExtensionCount; i++)
    {
        instanceExtensionNames.push_back(glfwExtensions[i]);
    }

    VkApplicationInfo app_info = {};

    instanceExtensionNames.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    VkInstanceCreateInfo inst_info = {};
    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_info.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    inst_info.pNext = NULL;
    inst_info.pApplicationInfo = &app_info;
    inst_info.enabledExtensionCount = static_cast<uint32_t>(instanceExtensionNames.size());
    inst_info.ppEnabledExtensionNames = instanceExtensionNames.data();
    inst_info.enabledLayerCount = 0;
    inst_info.ppEnabledLayerNames = NULL;

    VkResult result;
    result = vkCreateInstance(&inst_info, NULL, &instance);
    if (result == VK_SUCCESS)
    {
        printf("Vulkan instance created successfully!\n");
    }
    else
    {
        printf("Vulkan instance creation failed!\n");
    }
}

//Method to destroy Vulkan instance
void MyVulkanManager::destroy_vulkan_instance()
{
    printf("Vulkan instance destroyed successfully!\n");
}

void MyVulkanManager::enumerate_vulkan_phy_devices()
{
    gpuCount = 0;
    VkResult result = vkEnumeratePhysicalDevices(instance, &gpuCount, NULL);
    //Method to destroy Vulkan instance
    printf("[Vulkan physical device count: %d]\n", gpuCount);
    gpus.resize(gpuCount);
    result = vkEnumeratePhysicalDevices(instance, &gpuCount, gpus.data());
    assert(result == VK_SUCCESS);
    printf("[Successfully retrieved %d physical devices]\n", gpuCount);
    vkGetPhysicalDeviceMemoryProperties(gpus[USED_GPU_INDEX], &memoryroperties);

    // Get queue family properties for the selected GPU
    vkGetPhysicalDeviceQueueFamilyProperties(gpus[USED_GPU_INDEX], &queueFamilyCount, NULL);
    queueFamilyprops.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(gpus[USED_GPU_INDEX], &queueFamilyCount, queueFamilyprops.data());
    printf("[Vulkan physical device %d queue family count: %d]\n", USED_GPU_INDEX, queueFamilyCount);
}

void MyVulkanManager::create_vulkan_devices()
{
    printf("[Vulkan physical device %d queue family count: %d]\n", USED_GPU_INDEX, queueFamilyCount);
    printf("[Vulkan physical device %d queue family count: %d]\n", USED_GPU_INDEX, queueFamilyCount);

    queueFamilyprops.resize(queueFamilyCount);
    printf("[Successfully retrieved queue family properties for Vulkan physical device %d]\n", USED_GPU_INDEX);
    printf("[Successfully retrieved queue family properties for Vulkan physical device %d]\n", USED_GPU_INDEX);

    VkDeviceQueueCreateInfo queueInfo = {};

    bool found = false;
    for (unsigned int i = 0; i < queueFamilyCount; i++)
    {
        if (queueFamilyprops[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            queueGraphicsFamilyIndex = i;
            queueInfo.queueFamilyIndex = i;
            printf("[Queue family index %d supports GRAPHICS operations]\n", i);
            printf("[This queue family supports %d queues]\n", queueFamilyprops[i].queueCount);
            printf("[This queue family supports %d queues]\n", queueFamilyprops[i].queueCount);
            found = true;
            break;
        }
    }

    float queue_priorities[1] = {0.0};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.pNext = NULL;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = queue_priorities;
    queueInfo.queueFamilyIndex = queueGraphicsFamilyIndex;

    deviceExtensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

#ifdef __APPLE__
    deviceExtensionNames.push_back("VK_KHR_portability_subset");
#endif

    VkPhysicalDeviceFeatures pdf;
    vkGetPhysicalDeviceFeatures(gpus[0], &pdf);
    if (pdf.textureCompressionBC == VK_TRUE)
    {
        printf("Supports BC compressed texture format\n");
    }
    else
    {
        printf("Does not support BC compressed texture format\n");
    }
    assert(pdf.textureCompressionBC == VK_TRUE);

    VkDeviceCreateInfo deviceInfo = {};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext = NULL;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    deviceInfo.enabledExtensionCount = deviceExtensionNames.size();
    deviceInfo.ppEnabledExtensionNames = deviceExtensionNames.data();
    deviceInfo.enabledLayerCount = 0;
    deviceInfo.ppEnabledLayerNames = NULL;
    deviceInfo.pEnabledFeatures = NULL;

    VkResult result = vkCreateDevice(gpus[USED_GPU_INDEX], &deviceInfo, NULL, &device);
    assert(result == VK_SUCCESS);
}

void MyVulkanManager::destroy_vulkan_devices()
{
    vkDestroyDevice(device, NULL);
    printf("Logical device destroyed successfully!\n");
}

void MyVulkanManager::create_vulkan_CommandBuffer()
{
    VkCommandPoolCreateInfo cmd_pool_info = {};
    cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmd_pool_info.pNext = NULL;
    cmd_pool_info.queueFamilyIndex = queueGraphicsFamilyIndex;
    cmd_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    VkResult result = vkCreateCommandPool(device, &cmd_pool_info, NULL, &cmdPool);
    assert(result == VK_SUCCESS);

    VkCommandBufferAllocateInfo cmdBAI = {};
    cmdBAI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBAI.pNext = NULL;
    cmdBAI.commandPool = cmdPool;
    cmdBAI.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdBAI.commandBufferCount = 1;
    result = vkAllocateCommandBuffers(device, &cmdBAI, &cmdBuffer);
    assert(result == VK_SUCCESS);
    cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_info.pNext = NULL;
    cmd_buf_info.flags = 0;
    cmd_buf_info.pInheritanceInfo = NULL;

    cmd_bufs[0] = cmdBuffer;
    VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    //ï¿½á½»ï¿½ï¿½Ï¢
    submit_info[0].pNext = NULL;
    submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info[0].pWaitDstStageMask = &pipe_stage_flags;
    submit_info[0].commandBufferCount = 1;
    submit_info[0].pCommandBuffers = cmd_bufs;
    submit_info[0].signalSemaphoreCount = 0;
    submit_info[0].pSignalSemaphores = NULL;
}

void MyVulkanManager::destroy_vulkan_CommandBuffer()
{
    VkCommandBuffer cmdBufferArray[1] = {cmdBuffer};

    vkFreeCommandBuffers
    (
        device,
        cmdPool,
        1,
        cmdBufferArray
    );
    vkDestroyCommandPool(device, cmdPool, NULL);
}

void MyVulkanManager::create_vulkan_swapChain()
{
    VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
    assert(result == VK_SUCCESS);

    VkBool32* pSupportsPresent = (VkBool32*)malloc(queueFamilyCount * sizeof(VkBool32));
    for (uint32_t i = 0; i < queueFamilyCount; i++)
    {
        vkGetPhysicalDeviceSurfaceSupportKHR(gpus[USED_GPU_INDEX], i, surface, &pSupportsPresent[i]);
        printf("Queue family index=%d %s present\n", i, (pSupportsPresent[i] == 1 ? "supports" : "doesn't support"));
    }

    queueGraphicsFamilyIndex = UINT32_MAX;
    queuePresentFamilyIndex = UINT32_MAX;
    for (uint32_t i = 0; i < queueFamilyCount; ++i)
    {
        //If current queue family supports Graphics (graphics) operations
        if ((queueFamilyprops[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
        {
            //Record the index of queue family supporting Graphics (graphics) operations
            if (queueGraphicsFamilyIndex == UINT32_MAX) queueGraphicsFamilyIndex = i;
            //If current queue family supports Present (display) operations
            if (pSupportsPresent[i] == VK_TRUE)
            {
                queueGraphicsFamilyIndex = i;
                queuePresentFamilyIndex = i;
                printf("Queue family index=%d supports both Graphics (graphics) and Present (display) operations\n", i);
                break;
            }
        }
    }

    queueGraphicsFamilyIndex = UINT32_MAX;
    queuePresentFamilyIndex = UINT32_MAX;
    for (uint32_t i = 0; i < queueFamilyCount; ++i)
    {
        if ((queueFamilyprops[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
        {
            if (queueGraphicsFamilyIndex == UINT32_MAX) queueGraphicsFamilyIndex = i;
            if (pSupportsPresent[i] == VK_TRUE)
            {
                queueGraphicsFamilyIndex = i;
                queuePresentFamilyIndex = i;
                printf("Queue family index=%d supports both GRAPHICS and PRESENT operations\n", i);
                break;
            }
        }
    }

    if (queuePresentFamilyIndex == UINT32_MAX)
    {
        for (size_t i = 0; i < queueFamilyCount; ++i)
        {
            if (pSupportsPresent[i] == VK_TRUE)
            {
                queuePresentFamilyIndex = i;
                break;
            }
        }
    }
    free(pSupportsPresent);

    if (queueGraphicsFamilyIndex == UINT32_MAX || queuePresentFamilyIndex == UINT32_MAX)
    {
        printf("No queue family supports both GRAPHICS and PRESENT operations\n");
        assert(false);
    }

    uint32_t formatCount;

    result = vkGetPhysicalDeviceSurfaceFormatsKHR(gpus[USED_GPU_INDEX], surface, &formatCount, NULL);
    printf("Supported format count: %d\n", formatCount);

    VkSurfaceFormatKHR* surfFormats = (VkSurfaceFormatKHR*)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
    formats.resize(formatCount);

    result = vkGetPhysicalDeviceSurfaceFormatsKHR(gpus[USED_GPU_INDEX], surface, &formatCount, surfFormats);

    for (int i = 0; i < formatCount; i++)
    {
        formats[i] = surfFormats[i].format;
        printf("[%d] Supported format: %d\n", i, formats[i]);
    }

    if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED)
    {
        formats[0] = VK_FORMAT_B8G8R8A8_UNORM;
    }

    free(surfFormats);

    result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpus[USED_GPU_INDEX], surface, &surfCapabilities);
    assert(result == VK_SUCCESS);

    result = vkGetPhysicalDeviceSurfacePresentModesKHR(gpus[USED_GPU_INDEX], surface, &presentModeCount, NULL);
    assert(result == VK_SUCCESS);
    printf("Present mode count: %d\n", presentModeCount);

    presentModes.resize(presentModeCount);

    result = vkGetPhysicalDeviceSurfacePresentModesKHR(gpus[USED_GPU_INDEX], surface, &presentModeCount,
                                                       presentModes.data());
    for (int i = 0; i < presentModeCount; i++)
    {
        printf("Present mode[%d]: %d\n", i, presentModes[i]);
    }

    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (size_t i = 0; i < presentModeCount; i++)
    {
        if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        }
        if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) && (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR))
        {
            swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        }
    }

    if (surfCapabilities.currentExtent.width == 0xFFFFFFFF)
    {
        swapchainExtent.width = screenWidth;
        swapchainExtent.height = screenHeight;
        if (swapchainExtent.width < surfCapabilities.minImageExtent.width)
        {
            swapchainExtent.width = surfCapabilities.minImageExtent.width;
        }
        else if (swapchainExtent.width > surfCapabilities.maxImageExtent.width)
        {
            swapchainExtent.width = surfCapabilities.maxImageExtent.width;
        }
        if (swapchainExtent.height < surfCapabilities.minImageExtent.height)
        {
            swapchainExtent.height = surfCapabilities.minImageExtent.height;
        }
        else if (swapchainExtent.height > surfCapabilities.maxImageExtent.height)
        {
            swapchainExtent.height = surfCapabilities.maxImageExtent.height;
        }
        printf("Using recommended resolution: width %d height %d\n", swapchainExtent.width, swapchainExtent.height);
    }
    else
    {
        swapchainExtent = surfCapabilities.currentExtent;
        printf("Using surface resolution: width %d height %d\n", swapchainExtent.width, swapchainExtent.height);
    }

    screenWidth = swapchainExtent.width;
    screenHeight = swapchainExtent.height;

    uint32_t desiredMinNumberOfSwapChainImages = surfCapabilities.minImageCount + 1;
    if ((surfCapabilities.maxImageCount > 0) && (desiredMinNumberOfSwapChainImages > surfCapabilities.maxImageCount))
    {
        desiredMinNumberOfSwapChainImages = surfCapabilities.maxImageCount;
    }

    VkSurfaceTransformFlagBitsKHR preTransform;
    if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
    {
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else
    {
        preTransform = surfCapabilities.currentTransform;
    }

    VkSwapchainCreateInfoKHR swapchain_ci = {};
    swapchain_ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_ci.pNext = NULL;
    swapchain_ci.surface = surface;
    swapchain_ci.minImageCount = desiredMinNumberOfSwapChainImages;
    swapchain_ci.imageFormat = formats[0];
    swapchain_ci.imageExtent.width = swapchainExtent.width;
    swapchain_ci.imageExtent.height = swapchainExtent.height;
    swapchain_ci.preTransform = preTransform;
    swapchain_ci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain_ci.imageArrayLayers = 1;
    swapchain_ci.presentMode = swapchainPresentMode;
    swapchain_ci.oldSwapchain = VK_NULL_HANDLE;
    swapchain_ci.clipped = true;
    swapchain_ci.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    swapchain_ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain_ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_ci.queueFamilyIndexCount = 0;
    swapchain_ci.pQueueFamilyIndices = NULL;

    if (queueGraphicsFamilyIndex != queuePresentFamilyIndex)
    {
        swapchain_ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_ci.queueFamilyIndexCount = 2;
        uint32_t queueFamilyIndices[2] = {queueGraphicsFamilyIndex, queuePresentFamilyIndex};
        swapchain_ci.pQueueFamilyIndices = queueFamilyIndices;
    }
    result = vkCreateSwapchainKHR(device, &swapchain_ci, NULL, &swapChain);
    assert(result == VK_SUCCESS);

    result = vkGetSwapchainImagesKHR(device, swapChain, &swapchainImageCount, NULL);
    assert(result == VK_SUCCESS);
    printf("[SwapChain image count: %d]\n", swapchainImageCount);

    swapchainImages.resize(swapchainImageCount);

    result = vkGetSwapchainImagesKHR(device, swapChain, &swapchainImageCount, swapchainImages.data());
    assert(result == VK_SUCCESS);
    swapchainImageViews.resize(swapchainImageCount);
    for (uint32_t i = 0; i < swapchainImageCount; i++)
    {
        VkImageViewCreateInfo color_image_view = {};
        color_image_view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        color_image_view.pNext = NULL;
        color_image_view.flags = 0;
        color_image_view.image = swapchainImages[i];
        color_image_view.viewType = VK_IMAGE_VIEW_TYPE_2D;
        color_image_view.format = formats[0];
        color_image_view.components.r = VK_COMPONENT_SWIZZLE_R;
        color_image_view.components.g = VK_COMPONENT_SWIZZLE_G;
        color_image_view.components.b = VK_COMPONENT_SWIZZLE_B;
        color_image_view.components.a = VK_COMPONENT_SWIZZLE_A;
        color_image_view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        color_image_view.subresourceRange.baseMipLevel = 0;
        color_image_view.subresourceRange.levelCount = 1;
        color_image_view.subresourceRange.baseArrayLayer = 0;
        color_image_view.subresourceRange.layerCount = 1;
        result = vkCreateImageView(device, &color_image_view, NULL, &swapchainImageViews[i]);
        assert(result == VK_SUCCESS);
    }
}

void MyVulkanManager::destroy_vulkan_swapChain()
{
    for (uint32_t i = 0; i < swapchainImageCount; i++)
    {
        vkDestroyImageView(device, swapchainImageViews[i], NULL);
        printf("[Destroyed SwapChain ImageView %d successfully]\n", i);
    }
    vkDestroySwapchainKHR(device, swapChain, NULL);
    printf("SwapChain destroyed successfully!\n");
}

void MyVulkanManager::create_vulkan_DepthBuffer()
{
    depthFormat = VK_FORMAT_D16_UNORM;
    VkImageCreateInfo image_info = {};
    vkGetPhysicalDeviceFormatProperties(gpus[USED_GPU_INDEX], depthFormat, &depthFormatProps);
    if (depthFormatProps.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
    {
        image_info.tiling = VK_IMAGE_TILING_LINEAR;
        printf("tiling is VK_IMAGE_TILING_LINEAR\n");
    }
    else if (depthFormatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
    {
        image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
        printf("tiling is VK_IMAGE_TILING_OPTIMAL\n");
    }
    else
    {
        printf("VK_FORMAT_D16_UNORM not supported\n");
    }

    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.pNext = NULL;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.format = depthFormat;
    image_info.extent.width = screenWidth;
    image_info.extent.height = screenHeight;
    image_info.extent.depth = 1;
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    image_info.queueFamilyIndexCount = 0;
    image_info.pQueueFamilyIndices = NULL;
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    image_info.flags = 0;

    VkMemoryAllocateInfo mem_alloc = {};
    mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    mem_alloc.pNext = NULL;
    mem_alloc.allocationSize = 0;
    mem_alloc.memoryTypeIndex = 0;

    VkImageViewCreateInfo view_info = {};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.pNext = NULL;
    view_info.image = VK_NULL_HANDLE;
    view_info.format = depthFormat;
    view_info.components.r = VK_COMPONENT_SWIZZLE_R;
    view_info.components.g = VK_COMPONENT_SWIZZLE_G;
    view_info.components.b = VK_COMPONENT_SWIZZLE_B;
    view_info.components.a = VK_COMPONENT_SWIZZLE_A;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.flags = 0;

    VkResult result = vkCreateImage(device, &image_info, NULL, &depthImage);
    assert(result == VK_SUCCESS);

    VkMemoryRequirements mem_reqs;
    vkGetImageMemoryRequirements(device, depthImage, &mem_reqs);

    mem_alloc.allocationSize = mem_reqs.size;

    VkFlags requirements_mask = 0;

    bool flag = memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits, requirements_mask,
                                         &mem_alloc.memoryTypeIndex);
    assert(flag);
    printf("Memory type determined successfully, type index: %d\n", mem_alloc.memoryTypeIndex);

    result = vkAllocateMemory(device, &mem_alloc, NULL, &memDepth);
    assert(result == VK_SUCCESS);

    result = vkBindImageMemory(device, depthImage, memDepth, 0);
    assert(result == VK_SUCCESS);

    view_info.image = depthImage;
    result = vkCreateImageView(device, &view_info, NULL, &depthImageView);
    assert(result == VK_SUCCESS);
}

void MyVulkanManager::destroy_vulkan_DepthBuffer()
{
    vkDestroyImageView(device, depthImageView, NULL);
    vkDestroyImage(device, depthImage, NULL);
    vkFreeMemory(device, memDepth, NULL);
    printf("Depth buffer destroyed successfully!\n");
}

void MyVulkanManager::create_render_pass()
{
    VkSemaphoreCreateInfo imageAcquiredSemaphoreCreateInfo;
    imageAcquiredSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    imageAcquiredSemaphoreCreateInfo.pNext = NULL;
    imageAcquiredSemaphoreCreateInfo.flags = 0;

    VkResult result = vkCreateSemaphore(device, &imageAcquiredSemaphoreCreateInfo, NULL, &imageAcquiredSemaphore);
    assert(result == VK_SUCCESS);

    VkAttachmentDescription attachments[2];
    attachments[0].format = formats[0];
    attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    attachments[0].flags = 0;

    attachments[1].format = depthFormat;
    attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    attachments[1].flags = 0;

    VkAttachmentReference color_reference = {};
    color_reference.attachment = 0;
    color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depth_reference = {};
    depth_reference.attachment = 1;
    depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.flags = 0;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = NULL;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_reference;
    subpass.pResolveAttachments = NULL;
    subpass.pDepthStencilAttachment = &depth_reference;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = NULL;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo rp_info = {};
    rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rp_info.pNext = NULL;
    rp_info.attachmentCount = 2;
    rp_info.pAttachments = attachments;
    rp_info.subpassCount = 1;
    rp_info.pSubpasses = &subpass;
    rp_info.dependencyCount = 1;
    rp_info.pDependencies = &dependency;

    result = vkCreateRenderPass(device, &rp_info, NULL, &renderPass);
    assert(result == VK_SUCCESS);

    clear_values[0].color.float32[0] = 0.9f;
    clear_values[0].color.float32[1] = 0.9f;
    clear_values[0].color.float32[2] = 0.9f;
    clear_values[0].color.float32[3] = 0.9f;

    clear_values[1].depthStencil.depth = 1.0f;
    clear_values[1].depthStencil.stencil = 0;

    rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_begin.pNext = NULL;
    rp_begin.renderPass = renderPass;
    rp_begin.renderArea.offset.x = 0;
    rp_begin.renderArea.offset.y = 0;
    rp_begin.renderArea.extent.width = screenWidth;
    rp_begin.renderArea.extent.height = screenHeight;
    rp_begin.clearValueCount = 2;
    rp_begin.pClearValues = clear_values;
}

void MyVulkanManager::destroy_render_pass()
{
    vkDestroyRenderPass(device, renderPass, NULL);
    vkDestroySemaphore(device, imageAcquiredSemaphore, NULL);
}

void MyVulkanManager::init_queue()
{
    vkGetDeviceQueue(device, queueGraphicsFamilyIndex, 0, &queueGraphics);
}


void MyVulkanManager::create_frame_buffer()
{
    VkImageView attachments[2];
    attachments[1] = depthImageView;

    VkFramebufferCreateInfo fb_info = {};
    fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fb_info.pNext = NULL;
    fb_info.renderPass = renderPass;
    fb_info.attachmentCount = 2;
    fb_info.pAttachments = attachments;
    fb_info.width = screenWidth;
    fb_info.height = screenHeight;
    fb_info.layers = 1;

    uint32_t i;
    framebuffers = (VkFramebuffer*)malloc(swapchainImageCount * sizeof(VkFramebuffer));
    assert(framebuffers);

    for (i = 0; i < swapchainImageCount; i++)
    {
        attachments[0] = swapchainImageViews[i];
        VkResult result = vkCreateFramebuffer(device, &fb_info, NULL, &framebuffers[i]);
        assert(result == VK_SUCCESS);
        printf("[Destroyed framebuffer %d successfully]\n", i);
    }
}

void MyVulkanManager::destroy_frame_buffer()
{
    for (int i = 0; i < swapchainImageCount; i++)
    {
        vkDestroyFramebuffer(device, framebuffers[i], NULL);
    }
    free(framebuffers);
    printf("Framebuffer destroyed successfully!\n");
}

void MyVulkanManager::createDrawableObject()
{
    My3DLayer::initLevel();
    MyDraw::CreateDrawobject();
    MyDraw::InitDrawobject();
    float* vdataIn = new float[30]
    {
        -1, 1, 0, 0, 0, 1, -1, 0, 1, 1, 1, 1, 0, 1, 0,
        -1, 1, 0, 0, 0, -1, -1, 0, 0, 1, 1, -1, 0, 1, 1
    };
    d2dA = new Dashboard2DObject(vdataIn, 30 * 4, 6, device, memoryroperties);
}

void MyVulkanManager::destroyDrawableObject()
{
    delete d2dA;
    delete My3DLayer::planeForDraw;
    delete My3DLayer::mainBox;
    delete My3DLayer::sp3Dici[TREE_NUMBER];
    delete My3DLayer::sp3Tree[DICI_NUMBER];
    delete My3DLayer::sp3JiaziLeft[JIAZI_NUMBER];
    delete My3DLayer::sp3JiaziRight[JIAZI_NUMBER];
    delete My3DLayer::sp3JiaziLeftClose[JIAZI_NUMBER];
    delete My3DLayer::sp3JiaziRightClose[JIAZI_NUMBER];
    delete My3DLayer::sp3DiaoLuo[DIAOLUO_NUMBER];
    delete My3DLayer::sp3DiaoLuoshadow[DIAOLUO_NUMBER];
    delete My3DLayer::sp3YiDong[YIDONG_NUMBER];
    delete My3DLayer::attachBox[ATTACH_BOX_NUMBER];
    delete My3DLayer::attachBox2[ATTACH_BOX_NUMBER];
    delete My3DLayer::attachBox3[ATTACH_BOX_NUMBER];
    delete My3DLayer::attachBox4[ATTACH_BOX_NUMBER];
    delete My3DLayer::attachBox5[ATTACH_BOX_NUMBER];
    delete My3DLayer::attachBox6[ATTACH_BOX_NUMBER];
    delete My3DLayer::attachBox7[ATTACH_BOX_NUMBER];
    delete My3DLayer::attachBox8[ATTACH_BOX_NUMBER];
    delete My3DLayer::qizi;
    delete My3DLayer::qizigun[QIZIGUN_NUMBER];
    delete My3DLayer::tanshe[TANSHE_NUMBER];
}

void MyVulkanManager::createFence()
{
    VkFenceCreateInfo fenceInfo;
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = NULL;
    fenceInfo.flags = 0;
    vkCreateFence(device, &fenceInfo, NULL, &taskFinishFence);
}

void MyVulkanManager::destroyFence()
{
    vkDestroyFence(device, taskFinishFence, NULL);
}

void MyVulkanManager::initPresentInfo()
{
    present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present.pNext = NULL;
    present.swapchainCount = 1;
    present.pSwapchains = &swapChain;
    present.pWaitSemaphores = NULL;
    present.waitSemaphoreCount = 0;
    present.pResults = NULL;
}

void MyVulkanManager::initMatrixAndLight()
{
    MatrixState3D::setInitStack();

    float ratio = (float)screenWidth / (float)screenHeight;
    MatrixState3D::setProjectFrustum(-ratio, ratio, -1, 1, 1.5f, 10000);

    MatrixState2D::setInitStack();
    MatrixState2D::setProjectOrtho(-ratio, ratio, -1, 1, 1.5f, 1000);
    MatrixState2D::setCamera(0, 0, 5, 0, 0, 0, 0, 1, 0);
    CameraUtil::camera9Para[0] = -200;
    CameraUtil::camera9Para[1] = 400;
    CameraUtil::camera9Para[2] = -100;
    CameraUtil::camera9Para[3] = My3DLayer::mainBox->x;
    CameraUtil::camera9Para[4] = My3DLayer::mainBox->y;
    CameraUtil::camera9Para[5] = My3DLayer::mainBox->z;
    CameraUtil::camera9Para[6] = 0;
    CameraUtil::camera9Para[7] = 1;
    CameraUtil::camera9Para[8] = 0;
}

void MyVulkanManager::flushUniformBuffer()
{
    uint8_t* pData;
    float fragmentUniformData[16] =
    {
        0.9f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f
    };
    VkResult result = vkMapMemory(device, sqsD2D->memUniformBuf, 0, sqsD2D->bufferByteCount, 0, (void**)&pData);
    assert(result == VK_SUCCESS);
    memcpy(pData, fragmentUniformData, sqsD2D->bufferByteCount);
    vkUnmapMemory(device, sqsD2D->memUniformBuf);
}

void MyVulkanManager::flushTexToDesSet()
{
    for (int i = 0; i < TextureManager::texNamesForCommonTexLight.size(); i++)
    {
        sqsCTL->writes[0].dstSet = sqsCTL->descSet[i];
        sqsCTL->writes[1].dstSet = sqsCTL->descSet[i];
        sqsCTL->writes[1].pImageInfo = &(TextureManager::texImageInfoList[TextureManager::texNamesForCommonTexLight[
            i]]);
        vkUpdateDescriptorSets(device, 2, sqsCTL->writes, 0, NULL);
    }
    for (int i = 0; i < TextureManager::texNamesForDashboard2D.size(); i++)
    {
        sqsD2D->writes[0].dstSet = sqsD2D->descSet[i];
        sqsD2D->writes[1].dstSet = sqsD2D->descSet[i];
        sqsD2D->writes[1].pImageInfo = &(TextureManager::texImageInfoList[TextureManager::texNamesForDashboard2D[i]]);
        vkUpdateDescriptorSets(device, 2, sqsD2D->writes, 0, NULL);
    }
}

void MyVulkanManager::drawObject()
{
    FPSUtil::init();
    while (MyVulkanManager::loopDrawFlag)
    {
        FPSUtil::calFPS();
        FPSUtil::before();

        CameraUtil::flushCameraToMatrix();

        VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE,
                                                &currentBuffer);

        rp_begin.framebuffer = framebuffers[currentBuffer];


        vkResetCommandBuffer(cmdBuffer, 0);
        result = vkBeginCommandBuffer(cmdBuffer, &cmd_buf_info);
        MyVulkanManager::flushUniformBuffer();
        MyVulkanManager::flushTexToDesSet();
        vkCmdBeginRenderPass(cmdBuffer, &rp_begin, VK_SUBPASS_CONTENTS_INLINE);

        MyDraw::Draw3Dobject();
        MyDraw::Draw2Dobject();

        CameraUtil::calCamera();

        vkCmdEndRenderPass(cmdBuffer);
        result = vkEndCommandBuffer(cmdBuffer);
        submit_info[0].waitSemaphoreCount = 1;
        submit_info[0].pWaitSemaphores = &imageAcquiredSemaphore;
        result = vkQueueSubmit(queueGraphics, 1, submit_info, taskFinishFence);
        do
        {
            result = vkWaitForFences(device, 1, &taskFinishFence, VK_TRUE, FENCE_TIMEOUT);
        }
        while (result == VK_TIMEOUT);
        vkResetFences(device, 1, &taskFinishFence);
        present.pImageIndices = &currentBuffer;
        result = vkQueuePresentKHR(queueGraphics, &present);
        FPSUtil::after(60);
    }
}


void MyVulkanManager::doVulkan()
{
    ThreadTask* tt = new ThreadTask();
    thread t1(&ThreadTask::doTask, tt);
    t1.detach();
    // tt->doTask();
}

void MyVulkanManager::init_texture()
{
    TextureManager::initTextures(device, gpus[USED_GPU_INDEX], memoryroperties, cmdBuffer, queueGraphics);
}

void MyVulkanManager::destroy_textures()
{
    TextureManager::deatroyTextures(device);
}

void MyVulkanManager::initPipeline()
{
    sqsCTL = new ShaderQueueSuit_CommonTexLight(&device, renderPass, memoryroperties);
    sqsD2D = new ShaderQueueSuit_Dashboard2D(&device, renderPass, memoryroperties);
}

void MyVulkanManager::destroyPipeline()
{
    delete sqsCTL;
    delete sqsD2D;
}
