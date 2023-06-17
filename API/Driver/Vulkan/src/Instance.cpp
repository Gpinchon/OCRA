#include <OCRA/Core.hpp>

#include <VK/Assert.hpp>
#include <VK/Instance.hpp>
#include <VK/PhysicalDevice.hpp>

#include <stdexcept>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
#ifdef _WIN32
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#endif

#include <iostream>

namespace OCRA {
#ifdef _DEBUG
void PrintAvailableLayers()
{
    uint32_t layerCount;
    VK_INVOKE(vk::enumerateInstanceLayerProperties(&layerCount, nullptr));
    std::vector<vk::LayerProperties> availableLayers(layerCount);
    VK_INVOKE(vk::enumerateInstanceLayerProperties(&layerCount, availableLayers.data()));
    for (const auto& layerProperties : availableLayers) {
        std::cout << layerProperties.layerName.data() << '\n';
    }
}
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation",
};
bool CheckValidationLayerSupport()
{
    uint32_t layerCount;
    VK_INVOKE(vk::enumerateInstanceLayerProperties(&layerCount, nullptr));
    std::vector<vk::LayerProperties> availableLayers(layerCount);
    VK_INVOKE(vk::enumerateInstanceLayerProperties(&layerCount, availableLayers.data()));
    for (const char* layerName : validationLayers) {
        bool layerFound = false;
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            return false;
        }
    }
    return true;
}
#endif
Instance::Handle CreateInstance(
    const CreateInstanceInfo& a_Info,
    const AllocationCallback* a_Allocator)
{
    const std::vector<const char*> extensions {
#ifdef _WIN32
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
        VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef _DEBUG
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
#endif
    };
    static vk::raii::Context context;
    vk::InstanceCreateInfo info;
    vk::ApplicationInfo appInfo;
    appInfo.applicationVersion   = a_Info.applicationVersion;
    appInfo.pApplicationName     = a_Info.name.c_str();
    appInfo.engineVersion        = a_Info.engineVersion;
    appInfo.pEngineName          = a_Info.engineName.c_str();
    appInfo.apiVersion           = VK_API_VERSION_1_3;
    info.pApplicationInfo        = &appInfo;
    info.enabledExtensionCount   = extensions.size();
    info.ppEnabledExtensionNames = extensions.data();
#ifdef _DEBUG
    if (!CheckValidationLayerSupport()) {
        std::cerr << "!!! VK_LAYER_KHRONOS_validation NOT SUPPORTED !!!\n";
        std::cerr << "Available layers : \n";
        PrintAvailableLayers();
    } else {
        info.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
        info.ppEnabledLayerNames = validationLayers.data();
    }
#endif
    return std::make_shared<Instance::Impl>(context, info);
}
}

namespace OCRA::Instance {
const std::string GetType(const Handle& a_Instance)
{
    return "Vulkan";
}

const std::vector<PhysicalDevice::Handle> EnumeratePhysicalDevices(const Instance::Handle& a_Instance)
{
    auto vkPhysicaldevices = a_Instance->enumeratePhysicalDevices();
    std::vector<PhysicalDevice::Handle> ocPhysicalDevices { vkPhysicaldevices.size() };
    for (auto i = 0u; i < vkPhysicaldevices.size(); ++i)
        ocPhysicalDevices.at(i) = std::make_shared<PhysicalDevice::Impl>(*a_Instance, *vkPhysicaldevices.at(i));
    return ocPhysicalDevices;
}
}
