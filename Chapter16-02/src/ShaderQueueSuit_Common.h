#pragma once
#include <vector>
#include <vulkan/vulkan.h>
class ShaderQueueSuit_Common {
private:
    VkBuffer uniformBuf;
    VkDescriptorBufferInfo uniformBufferInfo;
    std::vector<VkDescriptorSetLayout> descLayouts;
    VkPipelineShaderStageCreateInfo shaderStages[2];
    VkVertexInputBindingDescription vertexBinding;
    VkVertexInputAttributeDescription vertexAttribs[3];
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
    void initVertexAttributeInfo();
    void create_pipe_line(VkDevice &device, VkRenderPass &renderPass);
    void destroy_pipe_line(VkDevice &device);

public:
    int bufferByteCount;
    VkDeviceMemory memUniformBuf;
    VkWriteDescriptorSet writes[2];
    std::vector<VkDescriptorSet> descSet;
    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;
    ShaderQueueSuit_Common(VkDevice *deviceIn, VkRenderPass &renderPass, VkPhysicalDeviceMemoryProperties &memoryroperties);
    ~ShaderQueueSuit_Common();
};