#pragma once
#include "base.h"
#include "vulkanWrapper/buffer.h"
#include "vulkanWrapper/device.h"
#include "vulkanWrapper/descriptorSet.h"
#include "vulkanWrapper/description.h"

namespace FF
{
    class Model
    {
    public:
        using Ptr = std::shared_ptr<Model>;

        static Ptr create(const Wrapper::Device::Ptr& device)
        {
            return std::make_shared<Model>(device);
        }

        Model(const Wrapper::Device::Ptr& device)
        {
            // 创建一个简单的矩形，用于天空盒或纹理显示
            // 位置数据 (x, y, z)
            mPositions = {
                -1000.0f, 1000.0f, 0.0f,
                -1000.0f, -1000.0f, 0.0f,
                1000.0f, 1000.0f, 0.0f,
                -1000.0f, -1000.0f, 0.0f,
                1000.0f, -1000.0f, 0.0f,
                1000.0f, 1000.0f, 0.0f
            };

            // UV坐标数据
            mUVs = {
                0.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f,
                1.0f, 0.0f
            };

            // 索引数据
            mIndexDatas = {0, 1, 2, 3, 4, 5};

            // 创建缓冲区
            mPositionBuffer = Wrapper::Buffer::createVertexBuffer(device, mPositions.size() * sizeof(float),
                                                                  mPositions.data());
            mUVBuffer = Wrapper::Buffer::createVertexBuffer(device, mUVs.size() * sizeof(float), mUVs.data());
            mIndexBuffer = Wrapper::Buffer::createIndexBuffer(device, mIndexDatas.size() * sizeof(uint32_t),
                                                              mIndexDatas.data());

            // 初始化uniform
            mUniform.mModelMatrix = glm::mat4(1.0f);
        }

        ~Model()
        {
        }

        //顶点数组buffer相关的信息
        std::vector<VkVertexInputBindingDescription> getVertexInputBindingDescriptions()
        {
            std::vector<VkVertexInputBindingDescription> bindingDes{};
            bindingDes.resize(2);

            bindingDes[0].binding = 0;
            bindingDes[0].stride = sizeof(float) * 3;
            bindingDes[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            bindingDes[1].binding = 1;
            bindingDes[1].stride = sizeof(float) * 2;
            bindingDes[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDes;
        }

        //Attribute相关信息，与VertexShader里面的location相关
        std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions()
        {
            std::vector<VkVertexInputAttributeDescription> attributeDes{};
            attributeDes.resize(2);

            attributeDes[0].binding = 0;
            attributeDes[0].location = 0;
            attributeDes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDes[0].offset = 0;

            attributeDes[1].binding = 1;
            attributeDes[1].location = 1;
            attributeDes[1].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDes[1].offset = 0;

            return attributeDes;
        }

        [[nodiscard]] auto getVertexBuffers() const
        {
            std::vector<VkBuffer> buffers{mPositionBuffer->getBuffer(), mUVBuffer->getBuffer()};
            return buffers;
        }

        [[nodiscard]] auto getIndexBuffer() const { return mIndexBuffer; }

        [[nodiscard]] auto getIndexCount() const { return mIndexDatas.size(); }

        [[nodiscard]] auto getUniform() const { return mUniform; }

        void setModelMatrix(const glm::mat4 matrix) { mUniform.mModelMatrix = matrix; }

        void update()
        {
            // 可以在这里添加模型更新逻辑，比如旋转等
            // 暂时保持静态
        }

    private:
        std::vector<float> mPositions{};
        std::vector<float> mUVs{};
        std::vector<uint32_t> mIndexDatas{};

        Wrapper::Buffer::Ptr mPositionBuffer{nullptr};
        Wrapper::Buffer::Ptr mUVBuffer{nullptr};
        Wrapper::Buffer::Ptr mIndexBuffer{nullptr};

        ObjectUniform mUniform;
    };
}
