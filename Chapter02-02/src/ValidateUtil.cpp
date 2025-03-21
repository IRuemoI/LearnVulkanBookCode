#include "ValidateUtil.h"
#include <cstring>

std::vector<VkLayerProperties> ValidateUtil::layerList;
PFN_vkCreateDebugReportCallbackEXT ValidateUtil::dbgCreateDebugReportCallback;
PFN_vkDestroyDebugReportCallbackEXT ValidateUtil::dbgDestroyDebugReportCallback;
VkDebugReportCallbackEXT ValidateUtil::debugReportCallback;
bool isContain(std::vector<const char *> inNames, char *inName) {//判断字符串是否在列表中的方法
    for (auto s: inNames) {//遍历字符串列表
        if (strcmp(s, inName) == 0) {//若给定字符串与当前字符串相同
            return true;//返回true，表示指定字符串在列表中
        }
    }
    return false;//返回false，表示指定字符串不在列表中
}
bool isContain(std::vector<std::string *> inNames, char *inName) {//判断字符串是否在列表中的方法
    for (auto s: inNames) {//遍历字符串列表
        if (strcmp((*s).c_str(), inName) == 0) {//若给定字符串与当前字符串相同
            return true;//返回true，表示指定字符串在列表中
        }
    }
    return false;//返回false，表示指定字符串不在列表中
}
DeviceLayerAndExtension ValidateUtil::getLayerProperties(std::vector<const char *> exceptedLayerNames) {
    DeviceLayerAndExtension result;//返回结果结构体实例
    uint32_t layerCount;//总的验证Layer的数量
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);//获取总的验证Layer数量
    printf("Layer的数量为 %d\n", layerCount);//打印总的验证Layer数量
    layerList.resize(layerCount);//更改列表长度
    vkEnumerateInstanceLayerProperties(&layerCount, layerList.data());//获取总的验证Layer属性列表
    for (int i = 0; i < layerList.size(); i++) {//遍历验证Layer属性列表
        VkLayerProperties lp = layerList[i];//获取当前验证Layer属性
        printf("----------------Layer %d----------------\n", i);//打印验证Layer序号
        printf("layer名称 %s\n", lp.layerName);//打印验证Layer名称
        printf("layer描述 %s\n", lp.description);//打印验证Layer描述信息
        bool flag = isContain(exceptedLayerNames, lp.layerName);//当前验证Layer是否需要
        if (flag) {//若需要，则将当前验证Layer名称记录到验证Layer名称结果列表
            result.layerNames.push_back(new std::string(lp.layerName));
        }
        uint32_t propertyCount;//此验证Layer对应的扩展属性数量
        vkEnumerateInstanceExtensionProperties(lp.layerName, &propertyCount, nullptr);
        std::vector<VkExtensionProperties> propertiesList;//扩展属性列表
        propertiesList.resize(propertyCount);//调整列表长度
        vkEnumerateInstanceExtensionProperties(lp.layerName, &propertyCount, propertiesList.data());
        for (auto ep: propertiesList) {//遍历此验证Layer对应的扩展属性列表
            printf("  所需扩展:%s\n", ep.extensionName);//打印扩展名称
            if (flag) {//若当前验证Layer是需要的
                if (!isContain(result.extensionNames, ep.extensionName)) {
                    result.extensionNames.push_back(new std::string(ep.extensionName));
                }
            }
        }
    }
    return result;//返回结果
}
std::vector<std::string *> ValidateUtil::getLayerDeviceExtension(VkPhysicalDevice &gpu, std::vector<std::string *> supportedlayerNames) {
    std::vector<std::string *> result;//所需设备扩展名称结果列表
    for (int i = 0; i < layerList.size(); i++) {//遍历所有验证Layer的属性列表
        VkLayerProperties lp = layerList[i];//获取当前验证Layer属性
        printf("----------------Layer %d----------------\n", i);//打印验证Layer序号
        printf("layer名称 %s\n", lp.layerName);//打印验证Layer名称
        printf("layer描述 %s\n", lp.description);//打印验证Layer描述信息
        uint32_t propertyCount;//设备扩展属性数量
        vkEnumerateDeviceExtensionProperties(gpu, lp.layerName, &propertyCount, nullptr);//获取当前验证Layer对应设备扩展属性数量
        std::vector<VkExtensionProperties> propertiesList;//设备扩展属性列表
        propertiesList.resize(propertyCount);//调整列表长度
        vkEnumerateDeviceExtensionProperties(gpu, lp.layerName, &propertyCount, propertiesList.data());//填充当前验证Layer对应设备扩展属性列表
        for (auto ep: propertiesList) {//遍历设备扩展属性列表
            printf("  所需设备扩展:%s\n", ep.extensionName);
            if (isContain(supportedlayerNames, lp.layerName)) {//判断当前验证Layer是否需要
                if (!isContain(result, ep.extensionName)) {//判断当前设备扩展是否已在列表中
                    result.push_back(new std::string(ep.extensionName));//将当前设备扩展名称添加进列表
                }
            }
        }
    }
    return result;//返回所需设备扩展名称结果列表
}

VKAPI_ATTR VkBool32 VKAPI_CALL ValidateUtil::debugFunction(
        VkFlags msgFlags,//触发此回调执行的调试事件类型标志
        VkDebugReportObjectTypeEXT objType,//由此回调处理的对象类型
        uint64_t srcObject,//此回调创建或处理的对象的句柄
        size_t location,//描述对应调试事件代码的位置
        int32_t msgCode,//消息代码
        const char *layerPrefix,//触发此回调的验证Layer，比如是加载器还是验证Layer
        const char *msg,//消息字符串
        void *pUserData) {//用户自定义数据
    if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {//错误信息
        printf("[VK_DEBUG_REPORT] ERROR: [%s]Code%d:%s\n", layerPrefix, msgCode, msg);
    } else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {//警告信息
        printf("[VK_DEBUG_REPORT] WARNING: [%s]Code%d:%s\n", layerPrefix, msgCode, msg);
    } else if (msgFlags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {//消息信息
        printf("[VK_DEBUG_REPORT] INFORMATION:[%s]Code%d:%s\n", layerPrefix, msgCode, msg);
    } else if (msgFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {//性能警告信息
        printf("[VK_DEBUG_REPORT] PERFORMANCE: [%s]Code%d:%s\n", layerPrefix, msgCode, msg);
    } else if (msgFlags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {//调试信息
        printf("[VK_DEBUG_REPORT] DEBUG: [%s]Code%d:%s\n", layerPrefix, msgCode, msg);
    } else {
        return VK_FALSE;//其他未知情况
    }
    return VK_SUCCESS;
}

void ValidateUtil::createDebugReportCallbackSelf(VkInstance instance) {//创建调试报告回调相关
    dbgCreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    VkDebugReportCallbackCreateInfoEXT dbgReportCreateInfo = {};//构建调试报告回调创建用信息结构体实例
    dbgReportCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    dbgReportCreateInfo.pfnCallback = debugFunction;//指定回调方法
    dbgReportCreateInfo.pUserData = nullptr;//传递给回调的用户自定义数据
    dbgReportCreateInfo.pNext = nullptr;//指向自定义数据的指针
    dbgReportCreateInfo.flags =//所需的触发消息回调的事件类型
            VK_DEBUG_REPORT_WARNING_BIT_EXT |
            VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
            VK_DEBUG_REPORT_ERROR_BIT_EXT |
            VK_DEBUG_REPORT_DEBUG_BIT_EXT;
    VkResult result = dbgCreateDebugReportCallback(instance, &dbgReportCreateInfo, nullptr, &debugReportCallback);//创建调试报告回调实例
    if (result == VK_SUCCESS) {
        printf("调试报告回调对象创建成功！\n");
    }
}
void ValidateUtil::destroyDebugReportCallbackSelf(VkInstance instance)//销毁调试报告回调相关
{
    dbgDestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    dbgDestroyDebugReportCallback(instance, debugReportCallback, nullptr);
}
