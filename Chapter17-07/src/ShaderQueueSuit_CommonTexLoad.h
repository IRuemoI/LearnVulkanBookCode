#ifndef SHADERQUEUESUIT_COMMONTEXLOAD_H
#define SHADERQUEUESUIT_COMMONTEXLOAD_H
#include <vector>
#include <vulkan/vulkan.h>
class ShaderQueueSuit_CommonTexLoad {
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
    VkDeviceMemory memUniformBuf;
    int bufferByteCount;
    VkWriteDescriptorSet writes[2];
    std::vector<VkDescriptorSet> descSet;
    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;
    ShaderQueueSuit_CommonTexLoad(VkDevice *deviceIn, VkRenderPass &renderPass, VkPhysicalDeviceMemoryProperties &memoryroperties);
    ~ShaderQueueSuit_CommonTexLoad();
};
#endif
