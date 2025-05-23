#include "VulkanRenderer.h"
#include "MeshData.h"
#include "VulkanApplication.h"
#include "Wrappers.h"

VulkanRenderer::VulkanRenderer(VulkanApplication *app, VulkanDevice *deviceObject) {
    // 注意使用0进行初始化
    memset(&Depth, 0, sizeof(Depth));
    memset(&connection, 0, sizeof(HINSTANCE));// hInstance - Windows Instance

    application = app;
    deviceObj = deviceObject;

    assert(application != nullptr);
    assert(deviceObj != nullptr);

    swapChainObj = new VulkanSwapChain(this);
    VulkanDrawable *drawableObj = new VulkanDrawable(this);
    drawableList.push_back(drawableObj);
}

VulkanRenderer::~VulkanRenderer() {
    delete swapChainObj;
    swapChainObj = nullptr;
}

void VulkanRenderer::initialize() {
    // 创建一个500x500的空白窗口
    createPresentationWindow(500, 500);

    // 初始化交换链
    swapChainObj->initializeSwapChain();

    // 我们需要用到指令缓存，因此创建指令缓冲池
    createCommandPool();

    // 创建交换链的颜色图和深度图
    buildSwapChainAndDepthImage();

    // 构建顶点缓冲
    createVertexBuffer();
}

bool VulkanRenderer::render() {
    MSG msg;// 消息
    PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
    if (msg.message == WM_QUIT) {
        return false;// 如果收到了退出消息，退出渲染循环
    }
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    RedrawWindow(window, nullptr, nullptr, RDW_INTERNALPAINT);
    return true;
}

#ifdef _WIN32

// MS-Windows的窗口回调函数
LRESULT CALLBACK VulkanRenderer::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    VulkanApplication *appObj = VulkanApplication::GetInstance();
    switch (uMsg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        default:
            break;
    }
    return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

void VulkanRenderer::createPresentationWindow(const int &windowWidth, const int &windowHeight) {
#ifdef _WIN32
    width = windowWidth;
    height = windowHeight;
    assert(width > 0 || height > 0);

    WNDCLASSEX winInfo;

    sprintf(name, "Vertex buffer creation");
    memset(&winInfo, 0, sizeof(WNDCLASSEX));

    // 初始化窗口类结构体
    winInfo.cbSize = sizeof(WNDCLASSEX);
    winInfo.style = CS_HREDRAW | CS_VREDRAW;
    winInfo.lpfnWndProc = WndProc;
    winInfo.cbClsExtra = 0;
    winInfo.cbWndExtra = 0;
    winInfo.hInstance = connection;// hInstance
    winInfo.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    winInfo.hCursor = LoadCursor(nullptr, IDC_ARROW);
    winInfo.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    winInfo.lpszMenuName = nullptr;
    winInfo.lpszClassName = name;
    winInfo.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);

    // 注册窗口类
    if (!RegisterClassEx(&winInfo)) {
        // It didn't work, so try to give a useful error:
        printf("试图启动应用程序时出现意外错误!\n");
        fflush(stdout);
        exit(1);
    }

    // Create window with the registered class:
    RECT wr = {0, 0, width, height};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
    window = CreateWindowEx(0,
                            name,// class name
                            name,// app name
                            WS_OVERLAPPEDWINDOW |// window style
                                    WS_VISIBLE |
                                    WS_SYSMENU,
                            100, 100,// x/y coords
                            wr.right - wr.left,// width
                            wr.bottom - wr.top,// height
                            nullptr,// handle to parent
                            nullptr,// handle to menu
                            connection,// hInstance
                            nullptr);// no extra parameters

    if (!window) {
        // It didn't work, so try to give a useful error:
        printf("无法创建要在其中绘图的窗口!\n");
        fflush(stdout);
        exit(1);
    }

    SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR) &application);
#else
    const xcb_setup_t *setup;
    xcb_screen_iterator_t iter;
    int scr;

    connection = xcb_connect(nullptr, &scr);
    if (connection == nullptr) {
        std::cout << "Cannot find a compatible Vulkan ICD.\n";
        exit(-1);
    }

    setup = xcb_get_setup(connection);
    iter = xcb_setup_roots_iterator(setup);
    while (scr-- > 0)
        xcb_screen_next(&iter);

    screen = iter.data;
#endif// _WIN32
}

void VulkanRenderer::destroyPresentationWindow() {
    DestroyWindow(window);
}
#else
void VulkanRenderer::createPresentationWindow() {
    assert(width > 0);
    assert(height > 0);

    uint32_t value_mask, value_list[32];

    window = xcb_generate_id(connection);

    value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    value_list[0] = screen->black_pixel;
    value_list[1] = XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_EXPOSURE;

    xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, screen->root, 0, 0, width, height, 0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, value_mask, value_list);

    /* Magic code that will send notification when window is destroyed */
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(connection, 1, 12, "WM_PROTOCOLS");
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(connection, cookie, 0);

    xcb_intern_atom_cookie_t cookie2 = xcb_intern_atom(connection, 0, 16, "WM_DELETE_WINDOW");
    reply = xcb_intern_atom_reply(connection, cookie2, 0);

    xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, (*reply).atom, 4, 32, 1, &(*reply).atom);
    free(reply);

    xcb_map_window(connection, window);

    // Force the x/y coordinates to 100,100 results are identical in consecutive runs
    const uint32_t coords[] = {100, 100};
    xcb_configure_window(connection, window, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, coords);
    xcb_flush(connection);

    xcb_generic_event_t *e;
    while ((e = xcb_wait_for_event(connection))) {
        if ((e->response_type & ~0x80) == XCB_EXPOSE)
            break;
    }
}

void VulkanRenderer::destroyWindow() {
    xcb_destroy_window(connection, window);
    xcb_disconnect(connection);
}

#endif// _WIN32

void VulkanRenderer::createCommandPool() {
    VulkanDevice *deviceObj = application->deviceObj;
    /* 依赖于initializeSwapChainExtension() */
    VkResult res;

    VkCommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.pNext = nullptr;
    cmdPoolInfo.queueFamilyIndex = deviceObj->graphicsQueueWithPresentIndex;
    cmdPoolInfo.flags = 0;

    res = vkCreateCommandPool(deviceObj->device, &cmdPoolInfo, nullptr, &cmdPool);
    assert(res == VK_SUCCESS);
}

void VulkanRenderer::createDepthImage() {
    VkResult result;
    bool pass;

    VkImageCreateInfo imageInfo = {};

    // 如果深度格式未定义，那么使用16位深度值
    if (Depth.format == VK_FORMAT_UNDEFINED) {
        Depth.format = VK_FORMAT_D16_UNORM;
    }

    const VkFormat depthFormat = Depth.format;

    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(*deviceObj->gpu, depthFormat, &props);
    if (props.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
        imageInfo.tiling = VK_IMAGE_TILING_LINEAR;
    } else if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    } else {
        std::cout << "深度格式不支持, 请尝试其他格式.\n";
        exit(-1);
    }

    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.pNext = nullptr;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = depthFormat;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.samples = NUM_SAMPLES;
    imageInfo.queueFamilyIndexCount = 0;
    imageInfo.pQueueFamilyIndices = nullptr;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageInfo.flags = 0;

    // 用户创建图像信息和图像对象
    result = vkCreateImage(deviceObj->device, &imageInfo, nullptr, &Depth.image);
    assert(result == VK_SUCCESS);

    // 获取图像内存需求
    VkMemoryRequirements memRqrmnt;
    vkGetImageMemoryRequirements(deviceObj->device, Depth.image, &memRqrmnt);

    VkMemoryAllocateInfo memAlloc = {};
    memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAlloc.pNext = nullptr;
    memAlloc.allocationSize = 0;
    memAlloc.memoryTypeIndex = 0;
    memAlloc.allocationSize = memRqrmnt.size;
    // 借助内存属性确定所需的内存类型
    pass = deviceObj->memoryTypeFromProperties(memRqrmnt.memoryTypeBits, 0, /* No requirements */ &memAlloc.memoryTypeIndex);
    assert(pass);

    // 为深度图像对象分配内存
    result = vkAllocateMemory(deviceObj->device, &memAlloc, nullptr, &Depth.mem);
    assert(result == VK_SUCCESS);

    // 绑定刚刚分配的内存
    result = vkBindImageMemory(deviceObj->device, Depth.image, Depth.mem, 0);
    assert(result == VK_SUCCESS);


    VkImageViewCreateInfo imgViewInfo = {};
    imgViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imgViewInfo.pNext = nullptr;
    imgViewInfo.image = VK_NULL_HANDLE;
    imgViewInfo.format = depthFormat;
    imgViewInfo.components = {VK_COMPONENT_SWIZZLE_IDENTITY};
    imgViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    imgViewInfo.subresourceRange.baseMipLevel = 0;
    imgViewInfo.subresourceRange.levelCount = 1;
    imgViewInfo.subresourceRange.baseArrayLayer = 0;
    imgViewInfo.subresourceRange.layerCount = 1;
    imgViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imgViewInfo.flags = 0;

    if (depthFormat == VK_FORMAT_D16_UNORM_S8_UINT ||
        depthFormat == VK_FORMAT_D24_UNORM_S8_UINT ||
        depthFormat == VK_FORMAT_D32_SFLOAT_S8_UINT) {
        imgViewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }

    // 使用指令缓存来创建深度图，包含指令缓存的分配、记录指令的起始和结束范围、指令提交行为这些步骤
    CommandBufferMgr::allocCommandBuffer(&deviceObj->device, cmdPool, &cmdDepthImage);
    CommandBufferMgr::beginCommandBuffer(cmdDepthImage);
    {
        // 设置图像布局为：深度蒙版优化
        convertImageLayout(Depth.image,
                       imgViewInfo.subresourceRange.aspectMask,
                       VK_IMAGE_LAYOUT_UNDEFINED,
                       VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, (VkAccessFlagBits) 0, cmdDepthImage);
    }
    CommandBufferMgr::endCommandBuffer(cmdDepthImage);
    CommandBufferMgr::submitCommandBuffer(deviceObj->queue, &cmdDepthImage);

    // 创建图像视图并允许应用程序使用图像
    imgViewInfo.image = Depth.image;
    result = vkCreateImageView(deviceObj->device, &imgViewInfo, nullptr, &Depth.view);
    assert(result == VK_SUCCESS);
}

void VulkanRenderer::destroyDrawableVertexBuffer() {
    for (VulkanDrawable *drawableObj: drawableList) {
        drawableObj->destroyVertexBuffer();
    }
}
void VulkanRenderer::destroyDepthBuffer() {
    vkDestroyImageView(deviceObj->device, Depth.view, nullptr);
    vkDestroyImage(deviceObj->device, Depth.image, nullptr);
    vkFreeMemory(deviceObj->device, Depth.mem, nullptr);
}

void VulkanRenderer::destroyCommandBuffer() {
    VkCommandBuffer cmdBufs[] = {cmdDepthImage, cmdVertexBuffer};
    vkFreeCommandBuffers(deviceObj->device, cmdPool, sizeof(cmdBufs) / sizeof(VkCommandBuffer), cmdBufs);
}

void VulkanRenderer::destroyCommandPool() {
    VulkanDevice *deviceObj = application->deviceObj;

    vkDestroyCommandPool(deviceObj->device, cmdPool, nullptr);
}

void VulkanRenderer::buildSwapChainAndDepthImage() {
    // 获取用于提交指令的队列
    deviceObj->getDeviceQueue();

    // 创建交换链，获取颜色图对象
    swapChainObj->createSwapChain(cmdDepthImage);

    // 创建深度图
    createDepthImage();
}

void VulkanRenderer::createVertexBuffer() {
    CommandBufferMgr::allocCommandBuffer(&deviceObj->device, cmdPool, &cmdVertexBuffer);
    CommandBufferMgr::beginCommandBuffer(cmdVertexBuffer);

    for (VulkanDrawable *drawableObj: drawableList) {
        drawableObj->createVertexBuffer(triangleData, sizeof(triangleData), sizeof(triangleData[0]), false);
    }
    CommandBufferMgr::endCommandBuffer(cmdVertexBuffer);
    CommandBufferMgr::submitCommandBuffer(deviceObj->queue, &cmdVertexBuffer);
}

void VulkanRenderer::convertImageLayout(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkAccessFlagBits srcAccessMask, const VkCommandBuffer &cmd) {
    assert(cmd != VK_NULL_HANDLE);
    assert(deviceObj->queue != VK_NULL_HANDLE);

    VkImageMemoryBarrier imageMemoryBarrier = {};
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.pNext = nullptr;
    imageMemoryBarrier.srcAccessMask = srcAccessMask;
    imageMemoryBarrier.dstAccessMask = 0;
    imageMemoryBarrier.oldLayout = oldImageLayout;
    imageMemoryBarrier.newLayout = newImageLayout;
    imageMemoryBarrier.image = image;
    imageMemoryBarrier.subresourceRange.aspectMask = aspectMask;
    imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
    imageMemoryBarrier.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
    imageMemoryBarrier.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;

    // 设置源和目标访问掩码以及管线阶段
    VkPipelineStageFlags srcStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags destStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    switch (oldImageLayout) {
        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            srcStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            break;
            // ... 其他布局的访问标志和阶段 ...
    }

    switch (newImageLayout) {
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            destStageFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
            break;
        case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            destStageFlags = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
            break;
        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            srcStageFlags = VK_PIPELINE_STAGE_HOST_BIT | VK_PIPELINE_STAGE_TRANSFER_BIT;
            destStageFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            break;
        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            destStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            break;
        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            destStageFlags = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            break;
            // ... 其他布局的访问标志和阶段 ...
    }

    vkCmdPipelineBarrier(
            cmd,
            srcStageFlags, destStageFlags,
            0,
            0, nullptr,
            0, nullptr,
            1, &imageMemoryBarrier);
}
