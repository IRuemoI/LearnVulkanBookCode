#pragma once
#include "base.h"
#include "vulkanWrapper/buffer.h"
#include "vulkanWrapper/device.h"
#include "vulkanWrapper/descriptorSet.h"
#include "vulkanWrapper/description.h"

namespace FF
{
    class BodyPart : public std::enable_shared_from_this<BodyPart>
    {
    public:
        using Ptr = std::shared_ptr<BodyPart>;

        static Ptr create(const Wrapper::Device::Ptr& device, const std::string& name)
        {
            return std::make_shared<BodyPart>(device, name);
        }

        BodyPart(const Wrapper::Device::Ptr& device, const std::string& name);

        ~BodyPart()
        {
        }

        // 绘制身体部件
        void drawSelf(VkCommandBuffer cmd, VkPipelineLayout pipelineLayout, VkPipeline pipeline,
                      VkDescriptorSet descriptorSet);

        // 更新变换矩阵
        void updateTransform();

        // 设置父部件
        void setParent(BodyPart::Ptr parent) { mParent = parent; }

        // 添加子部件
        void addChild(BodyPart::Ptr child)
        {
            mChildren.push_back(child);
            child->setParent(shared_from_this());
        }

        // 设置位置
        void setPosition(const glm::vec3& position) { mPosition = position; }

        // 设置旋转
        void setRotation(const glm::vec3& rotation) { mRotation = rotation; }

        // 设置缩放
        void setScale(const glm::vec3& scale) { mScale = scale; }

        // 获取变换矩阵
        [[nodiscard]] const glm::mat4& getTransformMatrix() const { return mTransformMatrix; }

        // 获取世界变换矩阵
        [[nodiscard]] glm::mat4 getWorldTransformMatrix() const;

        // 获取名称
        [[nodiscard]] const std::string& getName() const { return mName; }

        // 设置动画参数
        void setAnimationParams(const glm::vec3& positionOffset, const glm::vec3& rotationOffset, float time);

    private:
        Wrapper::Device::Ptr mDevice{nullptr};
        std::string mName;

        // 变换参数
        glm::vec3 mPosition{0.0f};
        glm::vec3 mRotation{0.0f};
        glm::vec3 mScale{1.0f};

        // 动画参数
        glm::vec3 mPositionOffset{0.0f};
        glm::vec3 mRotationOffset{0.0f};
        float mAnimationTime{0.0f};

        // 变换矩阵
        glm::mat4 mTransformMatrix{1.0f};

        // 层次结构
        BodyPart::Ptr mParent{nullptr};
        std::vector<BodyPart::Ptr> mChildren{};

        // 几何数据
        std::vector<float> mPositions{};
        std::vector<float> mColors{};
        std::vector<uint32_t> mIndices{};

        Wrapper::Buffer::Ptr mPositionBuffer{nullptr};
        Wrapper::Buffer::Ptr mColorBuffer{nullptr};
        Wrapper::Buffer::Ptr mIndexBuffer{nullptr};

        // 初始化几何数据
        void initGeometry();
    };
}
