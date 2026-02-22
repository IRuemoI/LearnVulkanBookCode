#include "application.h"
#include "vulkanWrapper/image.h"

namespace FF
{
    void Application::run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanUp();
    }

    void Application::onMouseMove(double xpos, double ypos)
    {
        if (mFirstMouse)
        {
            mLastMouseX = xpos;
            mLastMouseY = ypos;
            mFirstMouse = false;
        }

        double xoffset = xpos - mLastMouseX;
        double yoffset = mLastMouseY - ypos; // reversed since y-coordinates go from bottom to top

        mLastMouseX = xpos;
        mLastMouseY = ypos;

        const float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        mYaw += xoffset;
        mPitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (mPitch > 89.0f)
            mPitch = 89.0f;
        if (mPitch < -89.0f)
            mPitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
        front.y = sin(glm::radians(mPitch));
        front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
        mCameraFront = glm::normalize(front);
    }

    void Application::onKeyDown(int key, int action)
    {
        const float cameraSpeed = 0.05f;
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            switch (key)
            {
            case GLFW_KEY_W:
                mCameraPos += cameraSpeed * mCameraFront;
                break;
            case GLFW_KEY_S:
                mCameraPos -= cameraSpeed * mCameraFront;
                break;
            case GLFW_KEY_A:
                mCameraPos -= glm::normalize(glm::cross(mCameraFront, mCameraUp)) * cameraSpeed;
                break;
            case GLFW_KEY_D:
                mCameraPos += glm::normalize(glm::cross(mCameraFront, mCameraUp)) * cameraSpeed;
                break;
            }
        }
    }

    void Application::initWindow()
    {
        mWindow = Wrapper::Window::create(mWidth, mHeight);

        // 初始化相机
        mVPMatrices.mViewMatrix = glm::lookAt(mCameraPos, mCameraPos + mCameraFront, mCameraUp);
        mVPMatrices.mProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f,
                                                         100.0f);
    }

    void Application::initVulkan()
    {
        mInstance = Wrapper::Instance::create(true);
        mSurface = Wrapper::WindowSurface::create(mInstance, mWindow);

        mDevice = Wrapper::Device::create(mInstance, mSurface);

        mCommandPool = Wrapper::CommandPool::create(mDevice);

        mSwapChain = Wrapper::SwapChain::create(mDevice, mWindow, mSurface, mCommandPool);
        mWidth = mSwapChain->getExtent().width;
        mHeight = mSwapChain->getExtent().height;

        mRenderPass = Wrapper::RenderPass::create(mDevice);
        createRenderPass();

        mSwapChain->createFrameBuffers(mRenderPass);

        //descriptor ===========================
        mUniformManager = UniformManager::create();
        mUniformManager->init(mDevice, mCommandPool, mSwapChain->getImageCount());

        //创建模型
        mModel = Model::create(mDevice);

        mPipeline = Wrapper::Pipeline::create(mDevice, mRenderPass);
        createPipeline();

        // 初始化纹理
        initTextures();

        // 初始化模型
        initModels();

        mCommandBuffers.resize(mSwapChain->getImageCount());
        createCommandBuffers();

        createSyncObjects();
    }

    void Application::mainLoop()
    {
        while (!mWindow->shouldClose())
        {
            processInput();
            render();
        }

        vkDeviceWaitIdle(mDevice->getDevice());
    }

    void Application::processInput()
    {
        mWindow->pollEvents();
    }

    void Application::render()
    {
        VkFence fence = mFences[mCurrentFrame]->getFence();
        vkWaitForFences(mDevice->getDevice(), 1, &fence, VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(mDevice->getDevice(), mSwapChain->getSwapChain(), UINT64_MAX,
                                                mImageAvailableSemaphores[mCurrentFrame]->getSemaphore(),
                                                VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        vkResetFences(mDevice->getDevice(), 1, &fence);

        // 更新uniform buffer
        mVPMatrices.mViewMatrix = glm::lookAt(mCameraPos, mCameraPos + mCameraFront, mCameraUp);
        mVPMatrices.mProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f,
                                                         100.0f);
        mUniformManager->updateUniformBuffer(mCurrentFrame, mVPMatrices);

        // 更新模型
        mModel->update();

        mCommandBuffers[mCurrentFrame]->begin();

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = mRenderPass->getRenderPass();
        renderPassInfo.framebuffer = mSwapChain->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = mSwapChain->getExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(mCommandBuffers[mCurrentFrame]->getCommandBuffer(), &renderPassInfo,
                             VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(mCommandBuffers[mCurrentFrame]->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                          mPipeline->getPipeline());

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(mSwapChain->getExtent().width);
        viewport.height = static_cast<float>(mSwapChain->getExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(mCommandBuffers[mCurrentFrame]->getCommandBuffer(), 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = mSwapChain->getExtent();
        vkCmdSetScissor(mCommandBuffers[mCurrentFrame]->getCommandBuffer(), 0, 1, &scissor);

        drawObjects();

        vkCmdEndRenderPass(mCommandBuffers[mCurrentFrame]->getCommandBuffer());

        mCommandBuffers[mCurrentFrame]->end();

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {mImageAvailableSemaphores[mCurrentFrame]->getSemaphore()};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        VkCommandBuffer cmdBuffer = mCommandBuffers[mCurrentFrame]->getCommandBuffer();
        submitInfo.pCommandBuffers = &cmdBuffer;

        VkSemaphore signalSemaphores[] = {mRenderFinishedSemaphores[mCurrentFrame]->getSemaphore()};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(mDevice->getGraphicQueue(), 1, &submitInfo, fence) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {mSwapChain->getSwapChain()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(mDevice->getPresentQueue(), &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || mWindow->mWindowResized)
        {
            mWindow->mWindowResized = false;
            recreateSwapChain();
        }
        else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to present swap chain image!");
        }

        mCurrentFrame = (mCurrentFrame + 1) % mSwapChain->getImageCount();
    }

    void Application::cleanUp()
    {
        cleanupSwapChain();

        mUniformManager.reset();
        mModel.reset();

        for (size_t i = 0; i < mSwapChain->getImageCount(); i++)
        {
            mFences[i].reset();
            mRenderFinishedSemaphores[i].reset();
            mImageAvailableSemaphores[i].reset();
        }

        mCommandPool.reset();
        mDevice.reset();
        mSurface.reset();
        mInstance.reset();
        mWindow.reset();
    }

    void Application::createRenderPass()
    {
        // 交换链图像附件（解析目标）
        VkAttachmentDescription colorAttachmentResolve{};
        colorAttachmentResolve.format = mSwapChain->getFormat();
        colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        // 多采样颜色附件
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = mSwapChain->getFormat();
        colorAttachment.samples = mDevice->getMaxUsableSampleCount();
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        // 深度附件
        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = VK_FORMAT_D32_SFLOAT;
        depthAttachment.samples = mDevice->getMaxUsableSampleCount();
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference colorAttachmentResolveRef{};
        colorAttachmentResolveRef.attachment = 0;
        colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 1;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 2;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        // 使用RenderPass的buildRenderPass方法
        mRenderPass->addAttachment(colorAttachmentResolve);
        mRenderPass->addAttachment(colorAttachment);
        mRenderPass->addAttachment(depthAttachment);

        Wrapper::SubPass subpass;
        subpass.addColorAttachmentReference(colorAttachmentRef);
        subpass.setResolveAttachmentReference(colorAttachmentResolveRef);
        subpass.setDepthStencilAttachmentReference(depthAttachmentRef);
        mRenderPass->addSubPass(subpass);

        mRenderPass->addDependency(dependency);
        mRenderPass->buildRenderPass();
    }

    void Application::createPipeline()
    {
        auto bindingDescription = mModel->getVertexInputBindingDescriptions();
        auto attributeDescriptions = mModel->getAttributeDescriptions();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescription.size());
        vertexInputInfo.pVertexBindingDescriptions = bindingDescription.data();
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)mSwapChain->getExtent().width;
        viewport.height = (float)mSwapChain->getExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = mSwapChain->getExtent();

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = mDevice->getMaxUsableSampleCount();

        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.stencilTestEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = 2;
        VkDynamicState dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
        dynamicState.pDynamicStates = dynamicStates;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        VkDescriptorSetLayout layout = mUniformManager->getDescriptorSetLayout();
        pipelineLayoutInfo.pSetLayouts = &layout;
        pipelineLayoutInfo.pushConstantRangeCount = 0;

        // 设置Pipeline状态
        mPipeline->mVertexInputState = vertexInputInfo;
        mPipeline->mAssemblyState = inputAssembly;
        mPipeline->mViewportState = viewportState;
        mPipeline->mRasterState = rasterizer;
        mPipeline->mSampleState = multisampling;
        mPipeline->mDepthStencilState = depthStencil;
        mPipeline->mBlendState = colorBlending;
        mPipeline->mLayoutState = pipelineLayoutInfo;

        // 设置着色器
        auto vertexShader = Wrapper::Shader::create(mDevice, "shaders/shader.vert.spv", VK_SHADER_STAGE_VERTEX_BIT,
                                                    "main");
        auto fragmentShader = Wrapper::Shader::create(mDevice, "shaders/shader.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT,
                                                      "main");
        std::vector<Wrapper::Shader::Ptr> shaders = {vertexShader, fragmentShader};
        mPipeline->setShaderGroup(shaders);

        mPipeline->build();
    }

    void Application::createCommandBuffers()
    {
        for (size_t i = 0; i < mCommandBuffers.size(); i++)
        {
            mCommandBuffers[i] = Wrapper::CommandBuffer::create(mDevice, mCommandPool);
        }
    }

    void Application::createSyncObjects()
    {
        mImageAvailableSemaphores.resize(mSwapChain->getImageCount());
        mRenderFinishedSemaphores.resize(mSwapChain->getImageCount());
        mFences.resize(mSwapChain->getImageCount());

        for (size_t i = 0; i < mSwapChain->getImageCount(); i++)
        {
            mImageAvailableSemaphores[i] = Wrapper::Semaphore::create(mDevice);
            mRenderFinishedSemaphores[i] = Wrapper::Semaphore::create(mDevice);
            mFences[i] = Wrapper::Fence::create(mDevice);
        }
    }

    void Application::recreateSwapChain()
    {
        int width = 0, height = 0;
        glfwGetFramebufferSize(mWindow->getWindow(), &width, &height);
        while (width == 0 || height == 0)
        {
            glfwGetFramebufferSize(mWindow->getWindow(), &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(mDevice->getDevice());

        cleanupSwapChain();

        mSwapChain = Wrapper::SwapChain::create(mDevice, mWindow, mSurface, mCommandPool);
        mWidth = mSwapChain->getExtent().width;
        mHeight = mSwapChain->getExtent().height;

        mSwapChain->createFrameBuffers(mRenderPass);

        mCommandBuffers.resize(mSwapChain->getImageCount());
        createCommandBuffers();
    }

    void Application::cleanupSwapChain()
    {
        for (auto& commandBuffer : mCommandBuffers)
        {
            commandBuffer.reset();
        }
        mCommandBuffers.clear();

        mSwapChain.reset();
    }

    void Application::initTextures()
    {
        // 这里可以初始化纹理，暂时留空
        // 后续可以根据需要添加纹理加载逻辑
    }

    void Application::initModels()
    {
        // 模型已经在initVulkan中创建，这里可以添加额外的模型初始化逻辑
    }

    void Application::drawObjects()
    {
        // 绑定顶点缓冲区
        auto vertexBuffers = mModel->getVertexBuffers();
        VkBuffer vertexBufferHandles[vertexBuffers.size()];
        VkDeviceSize offsets[vertexBuffers.size()];
        for (size_t i = 0; i < vertexBuffers.size(); i++)
        {
            vertexBufferHandles[i] = vertexBuffers[i];
            offsets[i] = 0;
        }

        vkCmdBindVertexBuffers(mCommandBuffers[mCurrentFrame]->getCommandBuffer(), 0,
                               static_cast<uint32_t>(vertexBuffers.size()), vertexBufferHandles, offsets);

        // 绑定索引缓冲区
        vkCmdBindIndexBuffer(mCommandBuffers[mCurrentFrame]->getCommandBuffer(), mModel->getIndexBuffer()->getBuffer(),
                             0, VK_INDEX_TYPE_UINT32);

        // 绑定描述符集
        VkDescriptorSet descriptorSet = mUniformManager->getDescriptorSet(mCurrentFrame);
        vkCmdBindDescriptorSets(mCommandBuffers[mCurrentFrame]->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                                mPipeline->getLayout(), 0, 1, &descriptorSet, 0, nullptr);

        // 绘制
        vkCmdDrawIndexed(mCommandBuffers[mCurrentFrame]->getCommandBuffer(),
                         static_cast<uint32_t>(mModel->getIndexCount()), 1, 0, 0, 0);
    }
}
