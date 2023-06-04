#pragma once

#include <vulkan/vulkan_raii.hpp>

#ifdef _DEBUG
#include <iostream>
#endif

namespace OCRA::Instance {
struct Impl : vk::raii::Instance {
    Impl(vk::raii::Context& a_Context, const vk::InstanceCreateInfo& a_Info)
        : vk::raii::Instance(a_Context, a_Info)
    {
#ifdef _DEBUG
        vk::DebugReportCallbackCreateInfoEXT debugInfo(
            vk::DebugReportFlagBitsEXT::eDebug | vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::ePerformanceWarning | vk::DebugReportFlagBitsEXT::eWarning,
            DebugCallback);
        debugCallback = createDebugReportCallbackEXT(debugInfo);
#endif
    }
#ifdef _DEBUG
    static VkBool32 DebugCallback(
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objectType,
        uint64_t object,
        size_t location,
        int32_t messageCode,
        const char* pLayerPrefix,
        const char* pMessage,
        void* pUserData)
    {
        std::cerr << pMessage << std::endl;
        return true;
    }
    vk::raii::DebugReportCallbackEXT debugCallback = nullptr;
#endif
};
}
