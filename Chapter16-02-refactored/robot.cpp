#include "robot.h"

namespace FF
{
    Robot::Robot(const Wrapper::Device::Ptr& device) : mDevice(device)
    {
        initBodyParts();
    }

    void Robot::initBodyParts()
    {
        // 创建身体部件
        mRoot = BodyPart::create(mDevice, "Root");
        mBody = BodyPart::create(mDevice, "Body");
        mHead = BodyPart::create(mDevice, "Head");
        mLeftTop = BodyPart::create(mDevice, "LeftTop");
        mLeftBottom = BodyPart::create(mDevice, "LeftBottom");
        mRightTop = BodyPart::create(mDevice, "RightTop");
        mRightBottom = BodyPart::create(mDevice, "RightBottom");
        mRightLegTop = BodyPart::create(mDevice, "RightLegTop");
        mRightLegBottom = BodyPart::create(mDevice, "RightLegBottom");
        mLeftLegTop = BodyPart::create(mDevice, "LeftLegTop");
        mLeftLegBottom = BodyPart::create(mDevice, "LeftLegBottom");
        mLeftFoot = BodyPart::create(mDevice, "LeftFoot");
        mRightFoot = BodyPart::create(mDevice, "RightFoot");

        // 设置层次结构
        mRoot->addChild(mBody);
        mBody->addChild(mHead);
        mBody->addChild(mLeftTop);
        mBody->addChild(mLeftBottom);
        mBody->addChild(mRightTop);
        mBody->addChild(mRightBottom);
        mBody->addChild(mRightLegTop);
        mBody->addChild(mLeftLegTop);
        mRightLegTop->addChild(mRightLegBottom);
        mLeftLegTop->addChild(mLeftLegBottom);
        mRightLegBottom->addChild(mRightFoot);
        mLeftLegBottom->addChild(mLeftFoot);

        // 设置初始位置和大小
        mBody->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        mBody->setScale(glm::vec3(1.0f, 2.0f, 0.5f));

        mHead->setPosition(glm::vec3(0.0f, 1.5f, 0.0f));
        mHead->setScale(glm::vec3(0.8f, 0.8f, 0.8f));

        // 手臂
        mLeftTop->setPosition(glm::vec3(-0.8f, 0.5f, 0.0f));
        mLeftTop->setScale(glm::vec3(0.3f, 1.0f, 0.3f));
        mLeftBottom->setPosition(glm::vec3(0.0f, -0.6f, 0.0f));
        mLeftBottom->setScale(glm::vec3(0.3f, 0.8f, 0.3f));

        mRightTop->setPosition(glm::vec3(0.8f, 0.5f, 0.0f));
        mRightTop->setScale(glm::vec3(0.3f, 1.0f, 0.3f));
        mRightBottom->setPosition(glm::vec3(0.0f, -0.6f, 0.0f));
        mRightBottom->setScale(glm::vec3(0.3f, 0.8f, 0.3f));

        // 腿部
        mRightLegTop->setPosition(glm::vec3(0.3f, -1.2f, 0.0f));
        mRightLegTop->setScale(glm::vec3(0.4f, 1.0f, 0.4f));
        mRightLegBottom->setPosition(glm::vec3(0.0f, -0.6f, 0.0f));
        mRightLegBottom->setScale(glm::vec3(0.4f, 0.8f, 0.4f));

        mLeftLegTop->setPosition(glm::vec3(-0.3f, -1.2f, 0.0f));
        mLeftLegTop->setScale(glm::vec3(0.4f, 1.0f, 0.4f));
        mLeftLegBottom->setPosition(glm::vec3(0.0f, -0.6f, 0.0f));
        mLeftLegBottom->setScale(glm::vec3(0.4f, 0.8f, 0.4f));

        // 脚部
        mRightFoot->setPosition(glm::vec3(0.0f, -0.5f, 0.2f));
        mRightFoot->setScale(glm::vec3(0.5f, 0.2f, 0.8f));
        mLeftFoot->setPosition(glm::vec3(0.0f, -0.5f, 0.2f));
        mLeftFoot->setScale(glm::vec3(0.5f, 0.2f, 0.8f));

        // 添加到部件列表
        mBodyParts = {
            mRoot, mBody, mHead,
            mLeftTop, mLeftBottom, mRightTop, mRightBottom,
            mRightLegTop, mRightLegBottom, mLeftLegTop, mLeftLegBottom,
            mLeftFoot, mRightFoot
        };
    }

    void Robot::updateState()
    {
        // 更新所有身体部件的动画
        for (auto& bodyPart : mBodyParts)
        {
            bodyPart->updateTransform();
        }

        // 简单的行走动画
        float walkCycle = sin(mAnimationTime * 2.0f) * 0.3f;
        float armSwing = sin(mAnimationTime * 2.0f) * 0.5f;

        // 手臂摆动
        mLeftTop->setRotation(glm::vec3(0.0f, 0.0f, armSwing));
        mRightTop->setRotation(glm::vec3(0.0f, 0.0f, -armSwing));

        // 腿部摆动
        mLeftLegTop->setRotation(glm::vec3(walkCycle, 0.0f, 0.0f));
        mRightLegTop->setRotation(glm::vec3(-walkCycle, 0.0f, 0.0f));

        // 计算最低点
        calLowest();
    }

    void Robot::backToInit()
    {
        // 重置所有部件到初始状态
        for (auto& bodyPart : mBodyParts)
        {
            bodyPart->setPosition(glm::vec3(0.0f));
            bodyPart->setRotation(glm::vec3(0.0f));
            bodyPart->setScale(glm::vec3(1.0f));
        }

        // 重新设置初始位置和大小
        initBodyParts();
    }

    void Robot::calLowest()
    {
        mLowest = FLT_MAX;
        for (auto& bodyPart : mBodyParts)
        {
            glm::mat4 worldMatrix = bodyPart->getWorldTransformMatrix();
            glm::vec3 worldPos = glm::vec3(worldMatrix[3]);
            if (worldPos.y < mLowest)
            {
                mLowest = worldPos.y;
            }
        }
    }

    void Robot::drawSelf(VkCommandBuffer cmd, VkPipelineLayout pipelineLayout, VkPipeline pipeline,
                         VkDescriptorSet descriptorSet)
    {
        // 绘制所有身体部件
        for (auto& bodyPart : mBodyParts)
        {
            bodyPart->drawSelf(cmd, pipelineLayout, pipeline, descriptorSet);
        }
    }
}
