#include "ObjObject.h"
#include "HelpFunction.h"
#include "MatrixState3D.h"
#include <assert.h>
#include <cfloat>
#include <string.h>
ObjObject::ObjObject(float *vdataIn, int dataByteCount, int vCountIn, VkDevice &device, VkPhysicalDeviceMemoryProperties &memoryProperties) {
    pushConstantData = new float[16];
    this->devicePointer = &device;
    this->vdata = vdataIn;
    this->vCount = vCountIn;
    VkBufferCreateInfo buf_info = {};
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.pNext = nullptr;
    buf_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buf_info.size = dataByteCount;
    buf_info.queueFamilyIndexCount = 0;
    buf_info.pQueueFamilyIndices = nullptr;
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buf_info.flags = 0;
    VkResult result = vkCreateBuffer(device, &buf_info, nullptr, &vertexDatabuf);
    assert(result == VK_SUCCESS);
    VkMemoryRequirements mem_reqs;
    vkGetBufferMemoryRequirements(device, vertexDatabuf, &mem_reqs);
    assert(dataByteCount <= mem_reqs.size);
    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.pNext = nullptr;
    alloc_info.memoryTypeIndex = 0;
    alloc_info.allocationSize = mem_reqs.size;
    VkFlags requirements_mask = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bool flag = memoryTypeFromProperties(memoryProperties, mem_reqs.memoryTypeBits, requirements_mask, &alloc_info.memoryTypeIndex);
    if (flag) {
        printf("确定内存类型成功 类型索引为%d\n", alloc_info.memoryTypeIndex);
    } else {
        printf("确定内存类型失败!\n");
    }
    result = vkAllocateMemory(device, &alloc_info, nullptr, &vertexDataMem);
    assert(result == VK_SUCCESS);
    uint8_t *pData;
    result = vkMapMemory(device, vertexDataMem, 0, mem_reqs.size, 0, (void **) &pData);
    assert(result == VK_SUCCESS);
    memcpy(pData, vdata, dataByteCount);
    vkUnmapMemory(device, vertexDataMem);
    result = vkBindBufferMemory(device, vertexDatabuf, vertexDataMem, 0);
    assert(result == VK_SUCCESS);
    vertexDataBufferInfo.buffer = vertexDatabuf;
    vertexDataBufferInfo.offset = 0;
    vertexDataBufferInfo.range = mem_reqs.size;

    float minX = FLT_MAX;
    float maxX = -FLT_MAX;
    float minY = FLT_MAX;//初始化最小值
    float maxY = -FLT_MAX;
    float minZ = FLT_MAX;
    float maxZ = -FLT_MAX;//初始化最大值
    for (int i = 0; i < vCount; i++) {//遍历物体中的各个顶点
        float currX = vdataIn[i * 8 + 0];//获取顶点的X 坐标
        float currY = vdataIn[i * 8 + 1];//获取顶点的Y 坐标
        float currZ = vdataIn[i * 8 + 2];//获取顶点的Z 坐标
        if (minX > currX) { minX = currX; }//更新X 轴坐标最小值
        if (minY > currY) { minY = currY; }//更新Y 轴坐标最小值
        if (minZ > currZ) { minZ = currZ; }//更新Z 轴坐标最小值
        if (maxX < currX) { maxX = currX; }//更新X 轴坐标最大值
        if (maxY < currY) { maxY = currY; }//更新Y 轴坐标最大值
        if (maxZ < currZ) { maxZ = currZ; }//更新Z 轴坐标最大值
    }
    aabb = new AABB(minX, maxX, minY, maxY, minZ, maxZ);
}
ObjObject::~ObjObject() {
    delete[] vdata;
    vkDestroyBuffer(*devicePointer, vertexDatabuf, nullptr);
    vkFreeMemory(*devicePointer, vertexDataMem, nullptr);
    delete aabb;
}
void ObjObject::drawSelf(VkCommandBuffer &cmd, VkPipelineLayout &pipelineLayout, VkPipeline &pipeline, VkDescriptorSet *desSetPointer) {
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, desSetPointer, 0, nullptr);
    const VkDeviceSize offsetsVertex[1] = {0};
    vkCmdBindVertexBuffers(
            cmd,
            0,
            1,
            &(vertexDatabuf),
            offsetsVertex);
    float *mvp = MatrixState3D::getFinalMatrix();
    memcpy(pushConstantData, mvp, sizeof(float) * 16);
    vkCmdPushConstants(cmd, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float) * 16, pushConstantData);
    vkCmdDraw(cmd, vCount, 1, 0, 0);
}
