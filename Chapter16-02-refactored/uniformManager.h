#pragma once

#include "vulkanWrapper/buffer.h"
#include "vulkanWrapper/descriptorSetLayout.h"
#include "vulkanWrapper/descriptorPool.h"
#include "vulkanWrapper/descriptorSet.h"
#include "vulkanWrapper/description.h"
#include "vulkanWrapper/device.h"
#include "vulkanWrapper/commandPool.h"
#include "base.h"

using namespace FF;

class UniformManager
{
public:
    using Ptr = std::shared_ptr<UniformManager>;
    static Ptr create() { return std::make_shared<UniformManager>(); }

    UniformManager();

    ~UniformManager();

    void init(const Wrapper::Device::Ptr& device, const Wrapper::CommandPool::Ptr& commandPool, int frameCount);

    void updateUniformBuffer(int frameCount, const VPMatrices& vpMatrices);
    void updateObjectUniform(int frameCount, const ObjectUniform& objectUniform);

    [[nodiscard]] auto getDescriptorSetLayout() const { return mDescriptorSetLayout->getLayout(); }

    [[nodiscard]] auto getDescriptorSet(int frameCount) const { return mDescriptorSet->getDescriptorSet(frameCount); }

    [[nodiscard]] auto getShaderStages() const { return mShaderStages; }

private:
    Wrapper::Device::Ptr mDevice{nullptr};
    std::vector<Wrapper::UniformParameter::Ptr> mUniformParams;

    Wrapper::DescriptorSetLayout::Ptr mDescriptorSetLayout{nullptr};
    Wrapper::DescriptorPool::Ptr mDescriptorPool{nullptr};
    Wrapper::DescriptorSet::Ptr mDescriptorSet{nullptr};

    VkPipelineShaderStageCreateInfo mShaderStages[2];
};
