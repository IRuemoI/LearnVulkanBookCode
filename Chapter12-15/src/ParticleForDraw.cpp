#include "ParticleForDraw.h"
#include "HelpFunction.h"
#include "MatrixState3D.h"
#include "ParticleDataConstant.h"
#include <cassert>
#include <cstring>
ParticleForDraw::ParticleForDraw(float *vdataIn, int dataByteCount, int vCountIn, VkDevice &device, VkPhysicalDeviceMemoryProperties &memoryProperties) {
    pushConstantData = new float[41];
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
    result = vkBindBufferMemory(device, vertexDatabuf, vertexDataMem, 0);
    assert(result == VK_SUCCESS);
    vertexDataBufferInfo.buffer = vertexDatabuf;
    vertexDataBufferInfo.offset = 0;
    vertexDataBufferInfo.range = mem_reqs.size;
}
void ParticleForDraw::updateVertexData(float *vdataIn, int dataByteCount, VkDevice &device) {
    uint8_t *pData;
    VkResult result = vkMapMemory(device, vertexDataMem, 0, dataByteCount, 0, (void **) &pData);
    assert(result == VK_SUCCESS);
    memcpy(pData, vdataIn, dataByteCount);
    vkUnmapMemory(device, vertexDataMem);
}
ParticleForDraw::~ParticleForDraw() {
    delete[] vdata;
    vdata = nullptr;
    delete[] pushConstantData;
    pushConstantData = nullptr;
    vkDestroyBuffer(*devicePointer, vertexDatabuf, nullptr);
    vkFreeMemory(*devicePointer, vertexDataMem, nullptr);
}
void ParticleForDraw::drawSelf(VkCommandBuffer &cmd, VkPipelineLayout &pipelineLayout, VkPipeline &pipeline, VkDescriptorSet *desSetPointer, int id) {
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, desSetPointer, 0, nullptr);
    const VkDeviceSize offsetsVertex[1] = {0};
    vkCmdBindVertexBuffers(cmd, 0, 1, &(vertexDatabuf), offsetsVertex);

    float *mvp = MatrixState3D::getFinalMatrix();
    float *mm = MatrixState3D::getMMatrix();
    memcpy(pushConstantData, mvp, sizeof(float) * 16);
    memcpy(pushConstantData + 16, mm, sizeof(float) * 16);
    vkCmdPushConstants(cmd, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float) * 32, pushConstantData);

    memcpy(pushConstantData + 32, START_COLOR[id], sizeof(float) * 4);
    memcpy(pushConstantData + 36, END_COLOR[id], sizeof(float) * 4);
    memcpy(pushConstantData + 40, RADIS + id, sizeof(float) * 1);

    vkCmdPushConstants(cmd, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 128, sizeof(float) * 9, pushConstantData + 32);
    vkCmdDraw(cmd, vCount, 1, 0, 0);
}
