#pragma once

#include "Headers.h"
class VulkanInstance;
class VulkanDevice;
class VulkanRenderer;
class VulkanApplication;
/*
* Keep each of our swap chain buffers' image, command buffer and view in one spot
*/
struct SwapChainBuffer
{
    VkImage image;
    VkImageView view;
};

struct SwapChainPrivateVariables
{
    // Store the image surface capabilities
    VkSurfaceCapabilitiesKHR surfCapabilities;

    // Stores the number of present mode support by the implementation
    uint32_t presentModeCount;

    // Arrays for retrived present modes
    std::vector<VkPresentModeKHR> presentModes;

    // Size of the swap chain color images
    VkExtent2D swapChainExtent;

    // Number of color images supported by the implementation
    uint32_t desiredNumberOfSwapChainImages;

    VkSurfaceTransformFlagBitsKHR preTransform;

    // Stores present mode bitwise flag for the creation of swap chain
    VkPresentModeKHR swapchainPresentMode;

    // The retrived drawing color swap chain images
    std::vector<VkImage> swapchainImages;

    std::vector<VkSurfaceFormatKHR> surfFormats;
};

struct SwapChainPublicVariables
{
    // The logical platform dependent surface object
    VkSurfaceKHR surface;

    // Number of buffer image used for swap chain
    uint32_t swapchainImageCount;

    // Swap chain object
    VkSwapchainKHR swapChain;

    // List of color swap chain images
    std::vector<SwapChainBuffer> colorBuffer;

    // Semaphore for sync purpose
    VkSemaphore presentCompleteSemaphore;

    // Current drawing surface index in use
    uint32_t currentColorBuffer;

    // Format of the image
    VkFormat format;
};

class VulkanSwapChain {

    // Public member function
public:
    VulkanSwapChain(VulkanRenderer *renderer);
    ~VulkanSwapChain();
    void initializeSwapChain();
    void createSwapChain(const VkCommandBuffer &cmd);
    void destroySwapChain();


    // Private member variables
private:
    VkResult getSwapChainExtensions();
    void getSupportedFormats();
    VkResult createSurface();
    uint32_t getGraphicsQueueWithPresentationSupport();
    void getSurfaceCapabilitiesAndPresentMode();
    void managePresentMode();
    void getSwapChainColorImages();
    void createColorImageView(const VkCommandBuffer &cmd);

    // Public member variables
public:
    // 用户定义的结构体，其中包含了交换链的私有和共有函数用到各个变量
    SwapChainPublicVariables scPublicVars;
    PFN_vkQueuePresentKHR fpQueuePresentKHR;
    PFN_vkAcquireNextImageKHR fpAcquireNextImageKHR;

    // Private member variables
private:
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR fpGetPhysicalDeviceSurfaceSupportKHR;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR fpGetPhysicalDeviceSurfaceFormatsKHR;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR fpGetPhysicalDeviceSurfacePresentModesKHR;
    PFN_vkDestroySurfaceKHR fpDestroySurfaceKHR;

    // Layer Extensions Debugging
    PFN_vkCreateSwapchainKHR fpCreateSwapchainKHR;
    PFN_vkDestroySwapchainKHR fpDestroySwapchainKHR;
    PFN_vkGetSwapchainImagesKHR fpGetSwapchainImagesKHR;

    // User define structure containing private variables used
    // by the swap chain private and public functions.
    SwapChainPrivateVariables scPrivateVars;
    VulkanRenderer *rendererObj;// parent
    VulkanApplication *appObj;
};
