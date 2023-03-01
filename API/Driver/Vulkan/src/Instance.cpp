#include <VK/Instance.hpp>
#include <VK/PhysicalDevice.hpp>

#include <OCRA/Core.hpp>

#include <stdexcept>

#include <vulkan/vulkan.h>

namespace OCRA
{
#ifdef _DEBUG
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};
bool CheckValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
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
    VkInstance instance = nullptr;
    VkInstanceCreateInfo info{ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
    VkApplicationInfo appInfo{ VK_STRUCTURE_TYPE_APPLICATION_INFO };
    appInfo.applicationVersion = a_Info.applicationInfo.applicationVersion;
    appInfo.pApplicationName = a_Info.applicationInfo.name.c_str();
    appInfo.engineVersion = a_Info.applicationInfo.engineVersion;
    appInfo.pEngineName = a_Info.applicationInfo.engineName.c_str();
    info.pApplicationInfo = &appInfo;
#ifdef _DEBUG
    if (!CheckValidationLayerSupport())  throw std::runtime_error("Validation layers unavailable");
    info.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    info.ppEnabledLayerNames = validationLayers.data();
#endif
    vkCreateInstance(&info, nullptr, &instance);
    return std::make_shared<Instance::Impl>(instance);
}
}

namespace OCRA::Instance
{
const std::string GetType(const Handle& a_Instance)
{
    return "Vulkan";
}

const std::vector<PhysicalDevice::Handle> EnumeratePhysicalDevices(const Instance::Handle& a_Instance)
{
    uint32_t devicesCount = 0;
    vkEnumeratePhysicalDevices(*a_Instance, &devicesCount, nullptr);
    std::vector<VkPhysicalDevice>       vkPhysicaldevices{ devicesCount };
    std::vector<PhysicalDevice::Handle> ocPhysicalDevices{ devicesCount };
    vkEnumeratePhysicalDevices(*a_Instance, &devicesCount, vkPhysicaldevices.data());
    for (auto i = 0u; i < devicesCount; ++i)
        ocPhysicalDevices.at(i) = std::make_shared<PhysicalDevice::Impl>(vkPhysicaldevices.at(i));
    return ocPhysicalDevices;
}
}