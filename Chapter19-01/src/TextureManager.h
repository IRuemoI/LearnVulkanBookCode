#ifndef VULKANEXBASE_TEXTUREMANAGER_H
#define VULKANEXBASE_TEXTUREMANAGER_H

#include <vector>
#include <map>
#include <string>
#include <vulkan/vulkan.h>
#include "TexDataObject.h"

#define SAMPLER_COUNT 1

class TextureManager
{
private:
    static std::vector<std::string> texNames;

public:
    static struct WindowInfo info;

    static std::vector<std::string> texNamesForCommonTexLight;

    static std::vector<std::string> texNamesForDashboard2D;

    static std::vector<VkSampler> samplerList;

    static std::map<std::string, VkImage> textureImageList;

    static std::map<std::string, VkDeviceMemory> textureMemoryList;

    static std::map<std::string, VkImageView> viewTextureList;
    static std::map<std::string, VkDescriptorImageInfo> texImageInfoList;
    static void initTextures(VkDevice& device, VkPhysicalDevice& gpu, VkPhysicalDeviceMemoryProperties& memoryroperties,
                             VkCommandBuffer& cmdBuffer, VkQueue& queueGraphics);
    static void deatroyTextures(VkDevice& device);
    static int getVkDescriptorSetIndexForCommonTexLight(std::string texName);
    static int getVkDescriptorSetIndexForDashboard2D(std::string texName);

private:
    static void initSampler(VkDevice& device, VkPhysicalDevice& gpu);
    static void init_DXT5_BC3_Textures(std::string texName, VkDevice& device, VkPhysicalDevice& gpu,
                                       VkPhysicalDeviceMemoryProperties& memoryroperties, VkCommandBuffer& cmdBuffer,
                                       VkQueue& queueGraphics, VkFormat format, TexDataObject* ctdo);
};


#endif //VULKANEXBASE_TEXTUREMANAGER_H
