#ifndef VULKANEXBASE_OBJOBJECT_H
#define VULKANEXBASE_OBJOBJECT_H

#include <vulkan/vulkan.h>
#include <string>
#include "TextureManager.h"
using namespace std;

class ObjObject
{
public:
    VkDevice* devicePointer;
    float* vdata;
    int vCount;
    float* pushConstantData;
    //×ø±ê
    int x;
    int y;
    int z;
    //Ðý×ª
    int Rx;
    int Ry;
    int Rz;
    int Rangle;
    int Goorientation;
    string texturename;
    VkBuffer vertexDatabuf;
    VkDeviceMemory vertexDataMem;
    VkDescriptorBufferInfo vertexDataBufferInfo;

    ObjObject(float* vdataIn, int dataByteCount, int vCountIn, VkDevice& device,
              VkPhysicalDeviceMemoryProperties& memoryroperties, string texturename);
    ~ObjObject();

    void drawSelf(VkCommandBuffer& cmd, VkPipelineLayout& pipelineLayout, VkPipeline& pipeline,
                  VkDescriptorSet* desSetPointer);
    void setPosition3D(int x, int y, int z);
};


#endif
