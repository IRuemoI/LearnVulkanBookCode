#pragma once
#include "Headers.h"
class VulkanRenderer;

class VulkanDrawable {
public:
    VulkanDrawable(VulkanRenderer *parent = 0);
    ~VulkanDrawable();

    void createVertexBuffer(const void *vertexData, uint32_t dataSize, uint32_t dataStride, bool useTexture);
    void prepare();
    void render();
    void setPipeline(VkPipeline *vulkanPipeline) { pipeline = vulkanPipeline; }
    VkPipeline *getPipeline() { return pipeline; }

    void initViewports(VkCommandBuffer *cmd);
    void initScissors(VkCommandBuffer *cmd);

    void destroyVertexBuffer();
    void destroyCommandBuffer();
    void destroySynchronizationObjects();

public:
    // ���㻺��Ԫ���ݽṹ��
    struct
    {
        VkBuffer buf;
        VkDeviceMemory mem;
        VkDescriptorBufferInfo bufferInfo;
    } VertexBuffer;

    // ���涨����������
    VkVertexInputBindingDescription viIpBind;
    // �������ݽ��͹��������õ�Ԫ����
    VkVertexInputAttributeDescription viIpAttrb[2];

private:
    std::vector<VkCommandBuffer> vecCmdDraw;// Command buffer for drawing
    void recordCommandBuffer(int currentImage, VkCommandBuffer *cmdDraw);
    VkViewport viewport;
    VkRect2D scissor;
    VkSemaphore presentCompleteSemaphore;
    VkSemaphore drawingCompleteSemaphore;

    VulkanRenderer *rendererObj;
    VkPipeline *pipeline;
};