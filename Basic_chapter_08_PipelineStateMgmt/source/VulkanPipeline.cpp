#include "VulkanPipeline.h"
#include "VulkanApplication.h"
#include "VulkanDevice.h"
#include "VulkanRenderer.h"
#include "VulkanShader.h"

VulkanPipeline::VulkanPipeline() {
    appObj = VulkanApplication::GetInstance();
    deviceObj = appObj->deviceObj;
}

VulkanPipeline::~VulkanPipeline() {
}

void VulkanPipeline::createPipelineCache() {
    VkResult result;

    VkPipelineCacheCreateInfo pipelineCacheInfo;
    pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    pipelineCacheInfo.pNext = nullptr;
    pipelineCacheInfo.initialDataSize = 0;
    pipelineCacheInfo.pInitialData = nullptr;
    pipelineCacheInfo.flags = 0;
    result = vkCreatePipelineCache(deviceObj->device, &pipelineCacheInfo, nullptr, &pipelineCache);
    assert(result == VK_SUCCESS);
}

bool VulkanPipeline::createPipeline(VulkanDrawable *drawableObj, VkPipeline *pipeline, VulkanShader *shaderObj, VkBool32 includeDepth, VkBool32 includeVi) {
    // Initialize the dynamic states, initially it's empty
    VkDynamicState dynamicStateEnables[2];
    memset(dynamicStateEnables, 0, sizeof dynamicStateEnables);

    // Specify the dynamic state information to pipeline through
    // VkPipelineDynamicStateCreateInfo control structure.
    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pNext = nullptr;
    dynamicState.pDynamicStates = dynamicStateEnables;
    dynamicState.dynamicStateCount = 0;

    VkPipelineVertexInputStateCreateInfo vertexInputStateInfo = {};
    vertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputStateInfo.pNext = nullptr;
    vertexInputStateInfo.flags = 0;
    if (includeVi) {
        vertexInputStateInfo.vertexBindingDescriptionCount = sizeof(drawableObj->viIpBind) / sizeof(VkVertexInputBindingDescription);
        vertexInputStateInfo.pVertexBindingDescriptions = &drawableObj->viIpBind;
        vertexInputStateInfo.vertexAttributeDescriptionCount = sizeof(drawableObj->viIpAttrb) / sizeof(VkVertexInputAttributeDescription);
        vertexInputStateInfo.pVertexAttributeDescriptions = drawableObj->viIpAttrb;
    }
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
    inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyInfo.pNext = nullptr;
    inputAssemblyInfo.flags = 0;
    inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
    inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineRasterizationStateCreateInfo rasterStateInfo = {};
    rasterStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterStateInfo.pNext = nullptr;
    rasterStateInfo.flags = 0;
    rasterStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterStateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterStateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterStateInfo.depthClampEnable = includeDepth;
    rasterStateInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterStateInfo.depthBiasEnable = VK_FALSE;
    rasterStateInfo.depthBiasConstantFactor = 0;
    rasterStateInfo.depthBiasClamp = 0;
    rasterStateInfo.depthBiasSlopeFactor = 0;
    rasterStateInfo.lineWidth = 1.0f;

    // Create the viewport state create info and provide the
    // the number of viewport and scissors being used in the
    // rendering pipeline.
    VkPipelineColorBlendAttachmentState colorBlendAttachmentStateInfo[1] = {};
    colorBlendAttachmentStateInfo[0].colorWriteMask = 0xf;
    colorBlendAttachmentStateInfo[0].blendEnable = VK_FALSE;
    colorBlendAttachmentStateInfo[0].alphaBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachmentStateInfo[0].colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachmentStateInfo[0].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachmentStateInfo[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachmentStateInfo[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachmentStateInfo[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;

    VkPipelineColorBlendStateCreateInfo colorBlendStateInfo = {};
    colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendStateInfo.flags = 0;
    colorBlendStateInfo.pNext = nullptr;
    colorBlendStateInfo.attachmentCount = 1;
    colorBlendStateInfo.pAttachments = colorBlendAttachmentStateInfo;
    colorBlendStateInfo.logicOpEnable = VK_FALSE;
    colorBlendStateInfo.logicOp = VK_LOGIC_OP_NO_OP;
    colorBlendStateInfo.blendConstants[0] = 1.0f;
    colorBlendStateInfo.blendConstants[1] = 1.0f;
    colorBlendStateInfo.blendConstants[2] = 1.0f;
    colorBlendStateInfo.blendConstants[3] = 1.0f;

    VkPipelineViewportStateCreateInfo viewportStateInfo = {};
    viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateInfo.pNext = nullptr;
    viewportStateInfo.flags = 0;
    viewportStateInfo.viewportCount = NUMBER_OF_VIEWPORTS;
    viewportStateInfo.scissorCount = NUMBER_OF_SCISSORS;
    viewportStateInfo.pScissors = nullptr;
    viewportStateInfo.pViewports = nullptr;

    // Specify the dynamic state count and VkDynamicState enum stating which
    // dynamic state will use the values from dynamic state commands rather
    // than from the pipeline state creation info.
    dynamicStateEnables[dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_VIEWPORT;
    dynamicStateEnables[dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_SCISSOR;

    VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo = {};
    depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilStateInfo.pNext = nullptr;
    depthStencilStateInfo.flags = 0;
    depthStencilStateInfo.depthTestEnable = includeDepth;
    depthStencilStateInfo.depthWriteEnable = includeDepth;
    depthStencilStateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    depthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;
    depthStencilStateInfo.stencilTestEnable = VK_FALSE;
    depthStencilStateInfo.back.failOp = VK_STENCIL_OP_KEEP;
    depthStencilStateInfo.back.passOp = VK_STENCIL_OP_KEEP;
    depthStencilStateInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
    depthStencilStateInfo.back.compareMask = 0;
    depthStencilStateInfo.back.reference = 0;
    depthStencilStateInfo.back.depthFailOp = VK_STENCIL_OP_KEEP;
    depthStencilStateInfo.back.writeMask = 0;
    depthStencilStateInfo.minDepthBounds = 0;
    depthStencilStateInfo.maxDepthBounds = 0;
    depthStencilStateInfo.stencilTestEnable = VK_FALSE;
    depthStencilStateInfo.front = depthStencilStateInfo.back;

    VkPipelineMultisampleStateCreateInfo multiSampleStateInfo = {};
    multiSampleStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multiSampleStateInfo.pNext = nullptr;
    multiSampleStateInfo.flags = 0;
    multiSampleStateInfo.pSampleMask = nullptr;
    multiSampleStateInfo.rasterizationSamples = NUM_SAMPLES;
    multiSampleStateInfo.sampleShadingEnable = VK_FALSE;
    multiSampleStateInfo.alphaToCoverageEnable = VK_FALSE;
    multiSampleStateInfo.alphaToOneEnable = VK_FALSE;
    multiSampleStateInfo.minSampleShading = 0.0;

    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
    pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    VkResult result;
    result = vkCreatePipelineLayout(deviceObj->device, &pPipelineLayoutCreateInfo, nullptr, &pipelineLayout);

    // Populate the VkGraphicsPipelineCreateInfo structure to specify
    // programmable stages, fixed-function pipeline stages render
    // pass, sub-passes and pipeline layouts
    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = nullptr;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.basePipelineHandle = 0;
    pipelineInfo.basePipelineIndex = 0;
    pipelineInfo.flags = 0;
    pipelineInfo.pVertexInputState = &vertexInputStateInfo;
    pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
    pipelineInfo.pRasterizationState = &rasterStateInfo;
    pipelineInfo.pColorBlendState = &colorBlendStateInfo;
    pipelineInfo.pTessellationState = nullptr;
    pipelineInfo.pMultisampleState = &multiSampleStateInfo;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.pViewportState = &viewportStateInfo;
    pipelineInfo.pDepthStencilState = &depthStencilStateInfo;
    pipelineInfo.pStages = shaderObj->shaderStages;
    pipelineInfo.stageCount = 2;
    pipelineInfo.renderPass = appObj->rendererObj->renderPass;
    pipelineInfo.subpass = 0;

    // 使用VkGraphicsPipelineCreateInfo对象中的元数据存储创建管道
    if (vkCreateGraphicsPipelines(deviceObj->device, pipelineCache, 1, &pipelineInfo, nullptr, pipeline) == VK_SUCCESS) {
        return true;
    } else {
        return false;
    }
}

// 当不再需要时，销毁管道缓存对象
void VulkanPipeline::destroyPipelineCache() {
    vkDestroyPipelineCache(deviceObj->device, pipelineCache, nullptr);
    vkDestroyPipelineLayout(deviceObj->device, pipelineLayout, nullptr);
}
