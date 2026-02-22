#pragma once

#include "../base.h"
#include "device.h"
#include "shader.h"
#include "renderPass.h"

namespace FF::Wrapper
{
    class Pipeline
    {
    public:
        using Ptr = std::shared_ptr<Pipeline>;

        static Ptr create(const Device::Ptr& device, const RenderPass::Ptr& renderPass)
        {
            return std::make_shared<Pipeline>(device, renderPass);
        }

        Pipeline(const Device::Ptr& device, const RenderPass::Ptr& renderPass);

        ~Pipeline();

        //设置着色器(一般只包含顶点和偏远着色器)
        void setShaderGroup(const std::vector<Shader::Ptr>& shaderGroup);

        //设置视口
        void setViewports(const std::vector<VkViewport>& viewports) { mViewports = viewports; }

        //设置裁剪
        void setScissors(const std::vector<VkRect2D>& scissors) { mScissors = scissors; }

        //添加混合附件
        void pushBlendAttachment(const VkPipelineColorBlendAttachmentState& blendAttachment)
        {
            mBlendAttachmentStates.push_back(blendAttachment);
        }

        void build();

    public:
        VkPipelineVertexInputStateCreateInfo mVertexInputState{}; //顶点输入信息
        VkPipelineInputAssemblyStateCreateInfo mAssemblyState{}; //图元装配信息
        VkPipelineViewportStateCreateInfo mViewportState{}; //视口信息
        VkPipelineRasterizationStateCreateInfo mRasterState{}; //剪裁信息
        VkPipelineMultisampleStateCreateInfo mSampleState{}; //采样信息
        std::vector<VkPipelineColorBlendAttachmentState> mBlendAttachmentStates{}; //混合附件信息
        VkPipelineColorBlendStateCreateInfo mBlendState{}; //颜色混合信息
        VkPipelineDepthStencilStateCreateInfo mDepthStencilState{}; //深度模板信息
        VkPipelineLayoutCreateInfo mLayoutState{}; //流水线布局信息

        //TODO : needs render pass and subpass index
    private:
        VkPipeline mPipeline{VK_NULL_HANDLE};
        VkPipelineLayout mLayout{VK_NULL_HANDLE};
        Device::Ptr mDevice{nullptr};
        RenderPass::Ptr mRenderPass{nullptr};

        std::vector<Shader::Ptr> mShaders{};

        std::vector<VkViewport> mViewports{};
        std::vector<VkRect2D> mScissors{};
    };
}
