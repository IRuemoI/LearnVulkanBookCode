#include "TextureManager.h"
#include "FileUtil.h"
#include "HelpFunction.h"
#include <cassert>
void convertImageLayout(VkCommandBuffer cmd, VkImage image, VkImageAspectFlags aspectMask, VkImageLayout old_image_layout, VkImageLayout new_image_layout) {
    VkImageMemoryBarrier image_memory_barrier = {};//构建图像内存屏障结构体实例
    image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;//结构体类型
    image_memory_barrier.pNext = nullptr;//自定义数据的指针
    image_memory_barrier.srcAccessMask = 0;//源访问掩码
    image_memory_barrier.dstAccessMask = 0;//目标访问掩码
    image_memory_barrier.oldLayout = old_image_layout;//旧布局（屏障前）
    image_memory_barrier.newLayout = new_image_layout;//新布局（屏障后）
    image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;//源队列族索引
    image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;//目标队列族索引
    image_memory_barrier.image = image;//对应的图像
    image_memory_barrier.subresourceRange.aspectMask = aspectMask;//使用方面
    image_memory_barrier.subresourceRange.baseMipLevel = 0;//基础mipmap级别
    image_memory_barrier.subresourceRange.levelCount = 1;//mipmap级别的数量
    image_memory_barrier.subresourceRange.baseArrayLayer = 0;//基础数组层
    image_memory_barrier.subresourceRange.layerCount = 1;//数组层的数量
    if (old_image_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {//判断旧布局
        image_memory_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;//设置源访问掩码
    }
    if (new_image_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {//判断新布局
        image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;//目标访问掩码
    }
    if (new_image_layout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {//判断新布局
        image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;//目标访问掩码
    }
    if (old_image_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {//判断旧布局
        image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;//源访问掩码
    }
    if (old_image_layout == VK_IMAGE_LAYOUT_PREINITIALIZED) {//判断旧布局
        image_memory_barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;//源访问掩码
    }
    if (new_image_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {//判断新布局
        image_memory_barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;//源访问掩码

        image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;//目标访问掩码
    }
    if (new_image_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {//判断新布局
        image_memory_barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;//目标访问掩码
    }
    if (new_image_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        image_memory_barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;//目标访问掩码
    }
    VkPipelineStageFlags src_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;//屏障前阶段
    VkPipelineStageFlags dest_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;//屏障后阶段
    vkCmdPipelineBarrier(cmd, src_stages, dest_stages, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);//放置屏障
}

std::vector<std::string> TextureManager::texNames = {"../textures/0601_wall.bntex"};
std::vector<VkSampler> TextureManager::samplerList;
std::map<std::string, VkImage> TextureManager::textureImageList;
std::map<std::string, VkDeviceMemory> TextureManager::textureMemoryList;
std::map<std::string, VkImageView> TextureManager::viewTextureList;
std::map<std::string, VkDescriptorImageInfo> TextureManager::texImageInfoList;
void TextureManager::initSampler(VkDevice &device, VkPhysicalDevice &gpu) {
    VkSamplerCreateInfo samplerCreateInfo = {};//构建采样器创建信息结构体实例
    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;//结构体的类型
    samplerCreateInfo.magFilter = VK_FILTER_LINEAR;//放大时的纹理采样方式
    samplerCreateInfo.minFilter = VK_FILTER_NEAREST;//缩小时的纹理采样方式
    samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;//mipmap 模式
    samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;//纹理 S 轴的拉伸方式
    samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;//纹理 T 轴的拉伸方式
    samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;//纹理 W 轴的拉伸方式
    samplerCreateInfo.mipLodBias = 0.0;//mipmap 时的 Lod 调整值
    samplerCreateInfo.anisotropyEnable = VK_FALSE;//是否启用各向异性过滤
    samplerCreateInfo.maxAnisotropy = 1;//各向异性最大过滤值
    samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;//纹素数据比较操作
    samplerCreateInfo.minLod = 0.0;//最小 LOD 值
    samplerCreateInfo.maxLod = 0.0;//最大 LOD 值
    samplerCreateInfo.compareEnable = VK_FALSE;//是否开启比较功能
    samplerCreateInfo.borderColor =
            VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;//要使用的预定义边框颜色
    for (int i = 0; i < SAMPLER_COUNT; i++) {//循环创建指定数量的采样器
        VkSampler samplerTexture;//声明采样器对象
        VkResult result = vkCreateSampler(device, &samplerCreateInfo, nullptr, &samplerTexture);//创建采样器
        assert(result == VK_SUCCESS);
        samplerList.push_back(samplerTexture);//将采样器加入列表
    }
}
void TextureManager::init_SPEC_2D_Textures(const std::string &texName, VkDevice &device, VkPhysicalDevice &gpu, VkPhysicalDeviceMemoryProperties &memoryProperties, VkCommandBuffer &cmdBuffer, VkQueue &queueGraphics, VkFormat format, TexDataObject *ctdo) {
    VkFormatProperties formatProps;//指定格式纹理的格式属性
    vkGetPhysicalDeviceFormatProperties(gpu, format, &formatProps);//获取指定格式纹理的格式属性
    bool needStaging = (!(formatProps.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT));//判断是否能使用线性瓦片纹理
    printf("TextureManager %s", (needStaging ? "不能使用线性瓦片纹理" : "能使用线性瓦片纹理"));

    if (needStaging) {
        VkBuffer tempBuf;//中转存储用的缓冲
        VkBufferCreateInfo buf_info = {};//构建缓冲创建信息结构体实例
        buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;//设置结构体类型
        buf_info.pNext = nullptr;//自定义数据的指针
        buf_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;//缓冲的用途为传输源
        buf_info.size = ctdo->dataByteCount;//数据总字节数
        buf_info.queueFamilyIndexCount = 0;//队列族数量
        buf_info.pQueueFamilyIndices = nullptr;//队列族索引列表
        buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;//共享模式
        buf_info.flags = 0;//标志
        VkResult result = vkCreateBuffer(device, &buf_info, nullptr, &tempBuf);//创建缓冲
        assert(result == VK_SUCCESS);//检查缓冲创建是否成功
        VkMemoryRequirements mem_reqs;//缓冲的内存需求
        vkGetBufferMemoryRequirements(device, tempBuf, &mem_reqs);//获取缓冲内存需求
        assert(ctdo->dataByteCount <= mem_reqs.size);//检查内存需求获取是否正确
        VkMemoryAllocateInfo alloc_info = {};//构建内存分配信息结构体实例
        alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;//结构体类型
        alloc_info.pNext = nullptr;//自定义数据的指针
        alloc_info.memoryTypeIndex = 0;//内存类型索引
        alloc_info.allocationSize = mem_reqs.size;//内存总字节数
        VkFlags requirements_mask = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;//需要的内存类型掩码
        bool flag = memoryTypeFromProperties(memoryProperties, mem_reqs.memoryTypeBits,
                                             requirements_mask, &alloc_info.memoryTypeIndex);//获取所需内存类型索引
        if (flag) {
            printf("确定内存类型成功 类型索引为%d", alloc_info.memoryTypeIndex);
        } else {
            printf("确定内存类型失败!");
        }
        VkDeviceMemory memTemp;//设备内存
        result = vkAllocateMemory(device, &alloc_info, nullptr, &memTemp);//分配设备内存
        assert(result == VK_SUCCESS);//检查内存分配是否成功
        uint8_t *pData;//CPU 访问时的辅助指针
        result = vkMapMemory(device, memTemp,
                             0, mem_reqs.size, 0, (void **) &pData);//将设备内存映射为 CPU 可访问
        assert(result == VK_SUCCESS);//检查映射是否成功
        memcpy(pData, ctdo->data, ctdo->dataByteCount);//将纹理数据拷贝进设备内存
        vkUnmapMemory(device, memTemp);//解除内存映射
        result = vkBindBufferMemory(device, tempBuf, memTemp, 0);//绑定内存与缓冲
        assert(result == VK_SUCCESS);//检查绑定是否成功

        VkImageCreateInfo image_create_info = {};//构建图像创建信息结构体实例
        image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;//结构体的类型
        image_create_info.pNext = nullptr;//自定义数据的指针
        image_create_info.imageType = VK_IMAGE_TYPE_2D;//图像类型
        image_create_info.format = format;//图像像素格式
        image_create_info.extent.width = ctdo->width;//图像宽度
        image_create_info.extent.height = ctdo->height;//图像高度
        image_create_info.extent.depth = 1;//图像深度
        image_create_info.mipLevels = 1;//图像 mipmap 级数
        image_create_info.arrayLayers = 1;//图像数组层数量
        image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;//采样模式
        image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;//采用最优瓦片组织方式
        image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;//初始布局
        image_create_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT |//图像用途
                                  VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        image_create_info.queueFamilyIndexCount = 0;//队列族数量
        image_create_info.pQueueFamilyIndices = nullptr;//队列族索引列表
        image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;//共享模式
        image_create_info.flags = 0;//标志
        VkImage textureImage;//纹理对应的图像
        result = vkCreateImage(device, &image_create_info, nullptr, &textureImage);//创建图像
        assert(result == VK_SUCCESS);//检查图像创建是否成功
        textureImageList[texName] = textureImage;//添加到纹理图像列表
        VkMemoryAllocateInfo mem_alloc = {};//构建内存分配信息结构体实例
        mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;//结构体的类型
        mem_alloc.pNext = nullptr;//自定义数据的指针
        mem_alloc.allocationSize = 0;//内存总字节数
        mem_alloc.memoryTypeIndex = 0;//内存类型索引
        vkGetImageMemoryRequirements(device, textureImage, &mem_reqs);//获取纹理图像内存需求
        mem_alloc.allocationSize = mem_reqs.size;//实际分配的内存字节数
        flag = memoryTypeFromProperties(memoryProperties, mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &mem_alloc.memoryTypeIndex);//获取内存类型索引
        VkDeviceMemory textureMemory;//纹理图像对应设备内存
        result = vkAllocateMemory(device, &mem_alloc, nullptr, &(textureMemory));//分配设备内存
        assert(result == VK_SUCCESS);

        textureMemoryList[texName] = textureMemory;//添加到纹理内存列表
        result = vkBindImageMemory(device, textureImage, textureMemory, 0);//将图像和设备内存绑定
        assert(result == VK_SUCCESS);

        VkBufferImageCopy bufferCopyRegion = {};//构建缓冲图像拷贝结构体实例
        bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;//使用方面
        bufferCopyRegion.imageSubresource.mipLevel = 0;// mipmap 级别
        bufferCopyRegion.imageSubresource.baseArrayLayer = 0;//基础数组层
        bufferCopyRegion.imageSubresource.layerCount = 1;//数组层的数量
        bufferCopyRegion.imageExtent.width = ctdo->width;//图像宽度
        bufferCopyRegion.imageExtent.height = ctdo->height;//图像高度
        bufferCopyRegion.imageExtent.depth = 1;//图像深度
        bufferCopyRegion.bufferOffset = 0;//偏移量
        VkCommandBufferBeginInfo cmd_buf_info = {};//构建命令缓冲启动信息结构体实例
        cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;//结构体类型
        cmd_buf_info.pNext = nullptr;//自定义数据的指针
        cmd_buf_info.flags = 0;//标志
        cmd_buf_info.pInheritanceInfo = nullptr;//继承信息
        const VkCommandBuffer cmd_buffers[] = {cmdBuffer};//命令缓冲数组
        VkSubmitInfo submit_info[1] = {};//提交信息数组
        submit_info[0].pNext = nullptr;//自定义数据的指针
        submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;//结构体类型
        submit_info[0].waitSemaphoreCount = 0;//等待的信号量数量
        submit_info[0].pWaitSemaphores = VK_NULL_HANDLE;//等待的信号量列表
        submit_info[0].pWaitDstStageMask = VK_NULL_HANDLE;//给定目标管线阶段
        submit_info[0].commandBufferCount = 1;//命令缓冲的数量
        submit_info[0].pCommandBuffers = cmd_buffers;//命令缓冲列表
        submit_info[0].signalSemaphoreCount = 0;//任务完毕后设置的信号量数量
        submit_info[0].pSignalSemaphores = nullptr;//任务完毕后设置的信号量数组
        VkFenceCreateInfo fenceInfo;//栅栏创建信息结构体实例
        VkFence copyFence;//拷贝任务用栅栏
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;//结构体类型
        fenceInfo.pNext = nullptr;//自定义数据的指针
        fenceInfo.flags = 0;//供将来使用的标志位
        vkCreateFence(device, &fenceInfo, nullptr, &copyFence);//创建栅栏
        vkResetCommandBuffer(cmdBuffer, 0);//清除命令缓冲
        result = vkBeginCommandBuffer(cmdBuffer, &cmd_buf_info);//启动命令缓冲（开始记录命令）
        convertImageLayout(cmdBuffer, textureImage, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);//修改图像布局（为拷贝做准备）
        vkCmdCopyBufferToImage(cmdBuffer, tempBuf, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopyRegion);//将缓冲中的数据拷贝到纹理图像中
        convertImageLayout(cmdBuffer, textureImage, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);//修改图像布局（为纹理采样准备）
        result = vkEndCommandBuffer(cmdBuffer);//结束命令缓冲（停止记录命令）
        assert(result == VK_SUCCESS);
        result = vkQueueSubmit(queueGraphics, 1, submit_info, copyFence);//提交给队列执行
        assert(result == VK_SUCCESS);
        do {//循环等待执行完毕
            result = vkWaitForFences(device, 1, &copyFence, VK_TRUE, 100000000);
        } while (result == VK_TIMEOUT);
        vkDestroyBuffer(device, tempBuf, nullptr);//销毁中转缓冲
        vkFreeMemory(device, memTemp, nullptr);//释放中转缓冲的设备内存
        vkDestroyFence(device, copyFence, nullptr);//销毁拷贝任务用栅栏
    } else {
        VkImageCreateInfo image_create_info = {};//构建图像创建信息结构体实例
        image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;//结构体的类型
        image_create_info.pNext = nullptr;//指向自定义数据的指针
        image_create_info.imageType = VK_IMAGE_TYPE_2D;//图像类型
        image_create_info.format = format;//图像像素格式
        image_create_info.extent.width = ctdo->width;//图像宽度
        image_create_info.extent.height = ctdo->height;//图像高度
        image_create_info.extent.depth = 1;//图像深度
        image_create_info.mipLevels = 1;//图像 mipmap 级数
        image_create_info.arrayLayers = 1;//图像数组层数量
        image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;//采样模式
        image_create_info.tiling = VK_IMAGE_TILING_LINEAR;//采用线性瓦片组织方式
        image_create_info.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;//初始布局
        image_create_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT;//图像用途
        image_create_info.queueFamilyIndexCount = 0;//队列族数量
        image_create_info.pQueueFamilyIndices = nullptr;//队列族索引列表
        image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;//共享模式
        image_create_info.flags = 0;//标志
        VkImage textureImage;//纹理对应的图像
        VkResult result = vkCreateImage(device, &image_create_info, nullptr, &textureImage);
        assert(result == VK_SUCCESS);//检查图像创建是否成功
        textureImageList[texName] = textureImage;//添加到纹理图像列表
        VkMemoryAllocateInfo mem_alloc = {};//构建内存分配信息结构体实例
        mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;//结构体的类型
        mem_alloc.pNext = nullptr;//自定义数据的指针
        mem_alloc.allocationSize = 0;//内存字节数
        mem_alloc.memoryTypeIndex = 0;//内存类型索引
        VkMemoryRequirements mem_reqs;//纹理图像的内存需求
        vkGetImageMemoryRequirements(device, textureImage, &mem_reqs);//获取纹理图像的内存需求
        mem_alloc.allocationSize = mem_reqs.size;//实际分配的内存字节数
        bool flag = memoryTypeFromProperties(memoryProperties, mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &mem_alloc.memoryTypeIndex);//获取内存类型索引
        VkDeviceMemory textureMemory;//创建设备内存实例
        result = vkAllocateMemory(device, &mem_alloc, nullptr, &(textureMemory));//分配设备内存
        textureMemoryList[texName] = textureMemory;//添加到纹理内存列表
        result = vkBindImageMemory(device, textureImage, textureMemory, 0);//绑定图像和内存
        uint8_t *pData;// CPU 访问时的辅助指针
        vkMapMemory(device, textureMemory, 0, mem_reqs.size, 0, (void **) (&pData));//映射内存为 CPU 可访问
        memcpy(pData, ctdo->data, mem_reqs.size);//将纹理数据拷贝进设备内存
        vkUnmapMemory(device, textureMemory);//解除内存映射
    }
    VkImageViewCreateInfo view_info = {};//构建图像视图创建信息结构体实例
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;//结构的类型
    view_info.pNext = nullptr;//自定义数据的指针
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;//图像视图的类型
    view_info.format = format;//图像视图的像素格式
    view_info.components.r = VK_COMPONENT_SWIZZLE_R;//设置 R 通道调和
    view_info.components.g = VK_COMPONENT_SWIZZLE_G;//设置 G 通道调和
    view_info.components.b = VK_COMPONENT_SWIZZLE_B;//设置 B 通道调和
    view_info.components.a = VK_COMPONENT_SWIZZLE_A;//设置 A 通道调和
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;//图像视图使用方面
    view_info.subresourceRange.baseMipLevel = 0;//基础 Mipmap 级别
    view_info.subresourceRange.levelCount = 1;//Mipmap 级别的数量
    view_info.subresourceRange.baseArrayLayer = 0;//基础数组层
    view_info.subresourceRange.layerCount = 1;//数组层的数量
    view_info.image = textureImageList[texName];//对应的图像
    VkImageView viewTexture;//纹理图像对应的图像视图
    VkResult result = vkCreateImageView(device, &view_info, nullptr, &viewTexture);
    viewTextureList[texName] = viewTexture;//添加到图像视图列表
    VkDescriptorImageInfo texImageInfo;//构建图像描述信息实例
    texImageInfo.imageView = viewTexture;//采用的图像视图
    texImageInfo.sampler = samplerList[0];//采用的采样器
    texImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;//图像布局
    texImageInfoList[texName] = texImageInfo;//添加到纹理图像描述信息列表
    delete ctdo;//删除内存中的纹理数据
}
void TextureManager::initTextures(VkDevice &device,//加载所有纹理的方法
                                  VkPhysicalDevice &gpu,
                                  VkPhysicalDeviceMemoryProperties &memoryProperties,
                                  VkCommandBuffer &cmdBuffer,
                                  VkQueue &queueGraphics) {
    initSampler(device, gpu);//初始化采样器
    for (int i = 0; i < texNames.size(); i++) {//遍历纹理文件名称列表
        TexDataObject *ctdo = FileUtil::loadCommonTexData(texNames[i]);//加载纹理文件数据
        printf("%s w %d h %d", texNames[i].c_str(), ctdo->width, ctdo->height);//打印纹理数据信息
        init_SPEC_2D_Textures(texNames[i], device, gpu, memoryProperties, cmdBuffer, queueGraphics, VK_FORMAT_R8G8B8A8_UNORM, ctdo);//加载 2D 纹理
    }
}
void TextureManager::destroyTextures(VkDevice &device) {//销毁纹理的方法
    for (int i = 0; i < SAMPLER_COUNT; i++) {//遍历所有采样器
        vkDestroySampler(device, samplerList[i], nullptr);//销毁采样器
    }
    for (int i = 0; i < texNames.size(); i++) {//遍历所有纹理
        vkDestroyImageView(device, viewTextureList[texNames[i]], nullptr);//销毁图像视图
        vkDestroyImage(device, textureImageList[texNames[i]], nullptr);//销毁图像
        vkFreeMemory(device, textureMemoryList[texNames[i]], nullptr);//释放设备内存
    }
}
int TextureManager::getVkDescriptorSetIndex(std::string texName) {
    int result = -1;//初始化结果值为-1
    for (int i = 0; i < texNames.size(); i++) {//遍历所有纹理
        if (texNames[i].compare(texName.c_str()) == 0) {//判断名称是否相同
            result = i;//以当前索引值为结果
            break;//退出遍历循环
        }
    }
    assert(result != -1);//检查结果值是否为-1
    return result;//返回索引
}
