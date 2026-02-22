#include "shader.h"

namespace FF::Wrapper
{
    //读取二进制文件
    static std::vector<char> readBinary(const std::string& fileName)
    {
        std::ifstream file(fileName.c_str(), std::ios::ate | std::ios::binary | std::ios::in);

        if (!file)
        {
            throw std::runtime_error("Error: failed to open shader file");
        }

        const size_t fileSize = file.tellg(); //打开文件时文件指针已经放在文件末尾，读取当前文件指针位置就是文件的大小
        std::vector<char> buffer(fileSize);

        file.seekg(0); //将文件指针移动到文件头
        file.read(buffer.data(), fileSize); //读取文件
        file.close();

        return buffer;
    }

    Shader::Shader(const Device::Ptr& device, const std::string& fileName, VkShaderStageFlagBits shaderStage,
                   const std::string& entryPoint)
    {
        mDevice = device;
        mShaderStage = shaderStage;
        mEntryPoint = entryPoint;

        std::vector<char> codeBuffer = readBinary(fileName);

        VkShaderModuleCreateInfo shaderCreateInfo{};
        shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderCreateInfo.codeSize = codeBuffer.size();
        shaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(codeBuffer.data());

        if (vkCreateShaderModule(mDevice->getDevice(), &shaderCreateInfo, nullptr, &mShaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("Error: failed to create shader");
        }
    }

    Shader::~Shader()
    {
        if (mShaderModule != VK_NULL_HANDLE)
        {
            vkDestroyShaderModule(mDevice->getDevice(), mShaderModule, nullptr);
        }
    }
}
