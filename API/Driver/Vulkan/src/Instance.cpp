#include <OCRA/Instance.hpp>

#include <VK/Instance.hpp>
#include <VK/PhysicalDevice.hpp>

#include <vulkan/vulkan.hpp>

namespace OCRA::Instance
{
auto CreateInstance(const Info& a_Info) {
    VkApplicationInfo appInfo{};
    appInfo.applicationVersion = a_Info.applicationInfo.applicationVersion;
    appInfo.pApplicationName = a_Info.applicationInfo.name.c_str();
    appInfo.engineVersion = a_Info.applicationInfo.engineVersion;
    appInfo.pEngineName = a_Info.applicationInfo.engineName.c_str();
    VkInstanceCreateInfo info{};
    info.pApplicationInfo = &appInfo;
    VkInstance instance = nullptr;
    vkCreateInstance(&info, nullptr, &instance);
    return instance;
}
static inline auto EnumeratePhysicalDevices(const VkInstance& instance) {
    uint32_t devicesCount = 0;
    vkEnumeratePhysicalDevices(instance, &devicesCount, nullptr);
    std::vector<VkPhysicalDevice>       vkPhysicaldevices{ devicesCount };
    std::vector<PhysicalDevice::Handle> ocPhysicalDevices{ devicesCount };
    vkEnumeratePhysicalDevices(instance, &devicesCount, vkPhysicaldevices.data());
    for (auto i = 0u; i < devicesCount; ++i)
        ocPhysicalDevices.at(i).reset(new PhysicalDevice::Impl(vkPhysicaldevices.at(i)));
    return ocPhysicalDevices;
}

Impl::Impl(const Info& a_Info)
    : instance(CreateInstance(a_Info))
    , info(a_Info)
{}

Impl::~Impl()
{
    vkDestroyInstance(instance, nullptr);
}

Handle Create(
    const Info& a_Info,
    const AllocationCallback* a_Allocator)
{
    return Handle(new Impl(a_Info));
}

const std::string GetType(const Handle& a_Instance)
{
    return "Vulkan";
}

const std::vector<PhysicalDevice::Handle> EnumeratePhysicalDevices(const Instance::Handle& a_Instance)
{
    return EnumeratePhysicalDevices(*a_Instance);
}

const Info& GetInfo(const Instance::Handle& a_Instance)
{
    return a_Instance->info;
}
}