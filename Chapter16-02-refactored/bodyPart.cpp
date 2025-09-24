#include "bodyPart.h"

namespace FF
{
    BodyPart::BodyPart(const Wrapper::Device::Ptr& device, const std::string& name)
        : mDevice(device), mName(name)
    {
        initGeometry();
    }

    void BodyPart::initGeometry()
    {
        // 创建一个简单的立方体几何体
        // 顶点数据：位置 + 颜色
        mPositions = {
            // 前面
            -0.5f, -0.5f, 0.5f, // 0
            0.5f, -0.5f, 0.5f, // 1
            0.5f, 0.5f, 0.5f, // 2
            -0.5f, 0.5f, 0.5f, // 3
            // 后面
            -0.5f, -0.5f, -0.5f, // 4
            0.5f, -0.5f, -0.5f, // 5
            0.5f, 0.5f, -0.5f, // 6
            -0.5f, 0.5f, -0.5f, // 7
        };

        // 颜色数据（每个顶点一个颜色）
        mColors = {
            // 前面 - 红色
            1.0f, 0.0f, 0.0f, // 0
            1.0f, 0.0f, 0.0f, // 1
            1.0f, 0.0f, 0.0f, // 2
            1.0f, 0.0f, 0.0f, // 3
            // 后面 - 绿色
            0.0f, 1.0f, 0.0f, // 4
            0.0f, 1.0f, 0.0f, // 5
            0.0f, 1.0f, 0.0f, // 6
            0.0f, 1.0f, 0.0f, // 7
        };

        // 索引数据
        mIndices = {
            // 前面
            0, 1, 2, 2, 3, 0,
            // 后面
            4, 5, 6, 6, 7, 4,
            // 左面
            7, 3, 0, 0, 4, 7,
            // 右面
            1, 5, 6, 6, 2, 1,
            // 上面
            3, 2, 6, 6, 7, 3,
            // 下面
            0, 1, 5, 5, 4, 0
        };

        // 创建缓冲区
        mPositionBuffer = Wrapper::Buffer::createVertexBuffer(mDevice, mPositions.size() * sizeof(float),
                                                              mPositions.data());
        mColorBuffer = Wrapper::Buffer::createVertexBuffer(mDevice, mColors.size() * sizeof(float), mColors.data());
        mIndexBuffer = Wrapper::Buffer::createIndexBuffer(mDevice, mIndices.size() * sizeof(uint32_t), mIndices.data());
    }

    void BodyPart::drawSelf(VkCommandBuffer cmd, VkPipelineLayout pipelineLayout, VkPipeline pipeline,
                            VkDescriptorSet descriptorSet)
    {
        // 更新变换矩阵
        updateTransform();

        // 计算世界变换矩阵
        glm::mat4 worldMatrix = getWorldTransformMatrix();

        // 创建对象uniform
        ObjectUniform objectUniform;
        objectUniform.mModelMatrix = worldMatrix;

        // 使用push constant来传递模型矩阵，避免uniform buffer的复杂性
        vkCmdPushConstants(cmd, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(ObjectUniform), &objectUniform);

        // 绑定描述符集
        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

        // 绑定顶点缓冲区
        VkBuffer vertexBuffers[] = {mPositionBuffer->getBuffer(), mColorBuffer->getBuffer()};
        VkDeviceSize offsets[] = {0, 0};
        vkCmdBindVertexBuffers(cmd, 0, 2, vertexBuffers, offsets);

        // 绑定索引缓冲区
        vkCmdBindIndexBuffer(cmd, mIndexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);

        // 绘制
        vkCmdDrawIndexed(cmd, static_cast<uint32_t>(mIndices.size()), 1, 0, 0, 0);
    }

    void BodyPart::updateTransform()
    {
        // 创建变换矩阵
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), mPosition);
        glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(mRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(mRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(mRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), mScale);

        // 应用动画偏移
        glm::mat4 animTranslation = glm::translate(glm::mat4(1.0f), mPositionOffset);
        glm::mat4 animRotationX = glm::rotate(glm::mat4(1.0f), glm::radians(mRotationOffset.x),
                                              glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 animRotationY = glm::rotate(glm::mat4(1.0f), glm::radians(mRotationOffset.y),
                                              glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 animRotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(mRotationOffset.z),
                                              glm::vec3(0.0f, 0.0f, 1.0f));

        // 组合变换矩阵
        mTransformMatrix = translation * rotationX * rotationY * rotationZ * scale *
            animTranslation * animRotationX * animRotationY * animRotationZ;
    }

    glm::mat4 BodyPart::getWorldTransformMatrix() const
    {
        if (mParent)
        {
            return mParent->getWorldTransformMatrix() * mTransformMatrix;
        }
        return mTransformMatrix;
    }

    void BodyPart::setAnimationParams(const glm::vec3& positionOffset, const glm::vec3& rotationOffset, float time)
    {
        mPositionOffset = positionOffset;
        mRotationOffset = rotationOffset;
        mAnimationTime = time;
    }
}
