#ifndef VULKANEXBASE_TexDrawableObject_H
#define VULKANEXBASE_TexDrawableObject_H
#include "TextureManager.h"
#include <string>
#include <vulkan/vulkan.h>
class TexDrawableObject {
public:
    VkDevice *devicePointer;
    float *vdata;
    int vCount;
    float *pushConstantData;
    VkBuffer vertexDatabuf;
    VkDeviceMemory vertexDataMem;
    VkDescriptorBufferInfo vertexDataBufferInfo;
    TexDrawableObject(float *vdataIn, int dataByteCount, int vCountIn, VkDevice &device, VkPhysicalDeviceMemoryProperties &memoryroperties);
    ~TexDrawableObject();
    void drawSelf(VkCommandBuffer &cmd, VkPipelineLayout &pipelineLayout, VkPipeline &pipeline, VkDescriptorSet *desSetPointer);
};
#endif
