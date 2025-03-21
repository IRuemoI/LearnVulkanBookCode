#ifndef VULKANEXBASE_ShaderQueueSuit_ComputeBD_H
#define VULKANEXBASE_ShaderQueueSuit_ComputeBD_H

#include <vector>
#include <vulkan/vulkan.h>

class ShaderQueueSuit_ComputeBD {
private:
    VkBuffer uniformBuf;
    VkDescriptorBufferInfo uniformBufferInfo;


    std::vector<VkDescriptorSetLayout> descLayouts;

    VkPipelineShaderStageCreateInfo shaderStages[1];

    VkPipelineCache pipelineCache;

    VkDevice *devicePointer;

    VkDescriptorPool descPool;

    void create_uniform_buffer(VkDevice &device, VkPhysicalDeviceMemoryProperties &memoryroperties);
    void destroy_uniform_buffer(VkDevice &device);
    void create_pipeline_layout(VkDevice &device);
    void destroy_pipeline_layout(VkDevice &device);
    void init_descriptor_set(VkDevice &device);
    void create_shader(VkDevice &device);
    void destroy_shader(VkDevice &device);
    void create_pipe_line(VkDevice &device);
    void destroy_pipe_line(VkDevice &device);

public:
    VkDeviceMemory memUniformBuf;
    int bufferByteCount;

    VkWriteDescriptorSet writes[3];

    std::vector<VkDescriptorSet> descSet;

    VkPipelineLayout pipelineLayout;

    VkPipeline pipeline;

    ShaderQueueSuit_ComputeBD(VkDevice *deviceIn, VkPhysicalDeviceMemoryProperties &memoryroperties);
    ~ShaderQueueSuit_ComputeBD();
};

#endif
