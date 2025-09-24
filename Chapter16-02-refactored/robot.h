#pragma once
#include "base.h"
#include "vulkanWrapper/buffer.h"
#include "vulkanWrapper/device.h"
#include "vulkanWrapper/descriptorSet.h"
#include "vulkanWrapper/description.h"
#include "bodyPart.h"

namespace FF
{
    class Robot
    {
    public:
        using Ptr = std::shared_ptr<Robot>;

        static Ptr create(const Wrapper::Device::Ptr& device)
        {
            return std::make_shared<Robot>(device);
        }

        Robot(const Wrapper::Device::Ptr& device);

        ~Robot()
        {
        }

        // 更新机器人状态
        void updateState();

        // 重置到初始状态
        void backToInit();

        // 计算最低点
        void calLowest();

        // 绘制机器人
        void drawSelf(VkCommandBuffer cmd, VkPipelineLayout pipelineLayout, VkPipeline pipeline,
                      VkDescriptorSet descriptorSet);

        // 获取身体部件
        [[nodiscard]] const std::vector<BodyPart::Ptr>& getBodyParts() const { return mBodyParts; }

        // 获取最低点
        [[nodiscard]] float getLowest() const { return mLowest; }

        // 设置动画时间
        void setAnimationTime(float time) { mAnimationTime = time; }

    private:
        Wrapper::Device::Ptr mDevice{nullptr};

        // 身体部件
        BodyPart::Ptr mRoot{nullptr};
        BodyPart::Ptr mBody{nullptr};
        BodyPart::Ptr mHead{nullptr};
        BodyPart::Ptr mLeftTop{nullptr};
        BodyPart::Ptr mLeftBottom{nullptr};
        BodyPart::Ptr mRightTop{nullptr};
        BodyPart::Ptr mRightBottom{nullptr};
        BodyPart::Ptr mRightLegTop{nullptr};
        BodyPart::Ptr mRightLegBottom{nullptr};
        BodyPart::Ptr mLeftLegTop{nullptr};
        BodyPart::Ptr mLeftLegBottom{nullptr};
        BodyPart::Ptr mLeftFoot{nullptr};
        BodyPart::Ptr mRightFoot{nullptr};

        std::vector<BodyPart::Ptr> mBodyParts{};

        float mLowest{FLT_MAX};
        float mAnimationTime{0.0f};

        // 初始化身体部件
        void initBodyParts();
    };
}
