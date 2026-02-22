#include "uniformManager.h"
#include "vulkanWrapper/shader.h"

UniformManager::UniformManager()
{
}

UniformManager::~UniformManager()
{
}

void UniformManager::init(const Wrapper::Device::Ptr& device, const Wrapper::CommandPool::Ptr& commandPool,
                          int frameCount)
{
    mDevice = device;

    auto vpParam = Wrapper::UniformParameter::create();
    vpParam->mBinding = 0;
    vpParam->mCount = 1;
    vpParam->mDescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    vpParam->mSize = sizeof(VPMatrices);
    vpParam->mStage = VK_SHADER_STAGE_VERTEX_BIT;

    for (int i = 0; i < frameCount; ++i)
    {
        auto buffer = Wrapper::Buffer::createUniformBuffer(device, vpParam->mSize, nullptr);
        vpParam->mBuffers.push_back(buffer);
    }

    mUniformParams.push_back(vpParam);

    auto objectParam = Wrapper::UniformParameter::create();
    objectParam->mBinding = 1;
    objectParam->mCount = 1;
    objectParam->mDescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    objectParam->mSize = sizeof(ObjectUniform);
    objectParam->mStage = VK_SHADER_STAGE_VERTEX_BIT;

    for (int i = 0; i < frameCount; ++i)
    {
        auto buffer = Wrapper::Buffer::createUniformBuffer(device, objectParam->mSize, nullptr);
        objectParam->mBuffers.push_back(buffer);
    }

    mUniformParams.push_back(objectParam);

    auto textureParam = Wrapper::UniformParameter::create();
    textureParam->mBinding = 2;
    textureParam->mCount = 1;
    textureParam->mDescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    textureParam->mStage = VK_SHADER_STAGE_FRAGMENT_BIT;
    // 暂时不加载纹理，后续可以添加
    // textureParam->mTexture = Texture::create(mDevice, commandPool, "assets/dragonBall.jpg");

    mUniformParams.push_back(textureParam);

    mDescriptorSetLayout = Wrapper::DescriptorSetLayout::create(device);
    mDescriptorSetLayout->build(mUniformParams);

    mDescriptorPool = Wrapper::DescriptorPool::create(device);
    mDescriptorPool->build(mUniformParams, frameCount);

    mDescriptorSet = Wrapper::DescriptorSet::create(device, mUniformParams, mDescriptorSetLayout, mDescriptorPool,
                                                    frameCount);

    // 创建着色器
    auto vertexShader = Wrapper::Shader::create(device, "shaders/shader.vert.spv", VK_SHADER_STAGE_VERTEX_BIT, "main");
    auto fragmentShader = Wrapper::Shader::create(device, "shaders/shader.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT,
                                                  "main");

    // 创建着色器阶段信息
    mShaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    mShaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    mShaderStages[0].module = vertexShader->getShaderModule();
    mShaderStages[0].pName = "main";

    mShaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    mShaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    mShaderStages[1].module = fragmentShader->getShaderModule();
    mShaderStages[1].pName = "main";
}

void UniformManager::updateUniformBuffer(int frameCount, const VPMatrices& vpMatrices)
{
    //update vp matrices
    mUniformParams[0]->mBuffers[frameCount]->updateBufferByMap((void*)(&vpMatrices), sizeof(VPMatrices));
}

void UniformManager::updateObjectUniform(int frameCount, const ObjectUniform& objectUniform)
{
    //update object uniform
    mUniformParams[1]->mBuffers[frameCount]->updateBufferByMap((void*)(&objectUniform), sizeof(ObjectUniform));
}
