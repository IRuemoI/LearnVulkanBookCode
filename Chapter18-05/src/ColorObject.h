#pragma once
#include <string>
#include <vulkan/vulkan.h>
class ColorObject {
public:
    VkDevice *devicePointer;
    float *vdata;
    int vCount;
    float *pushConstantData;
    float pointSize;
    VkBuffer vertexDatabuf;
    VkDeviceMemory vertexDataMem;
    VkDescriptorBufferInfo vertexDataBufferInfo;
    ColorObject(float *vdataIn, int dataByteCount, int vCountIn, VkDevice &device, VkPhysicalDeviceMemoryProperties &memoryProperties, float pointSizeIn);
    ~ColorObject();
    void drawSelf(VkCommandBuffer &cmd, VkPipelineLayout &pipelineLayout, VkPipeline &pipeline);
};