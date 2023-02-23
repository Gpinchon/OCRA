#include <OCRA/Handle.hpp>
#include <OCRA/Instance.hpp>

#include <vulkan/vulkan.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::PhysicalDevice);

namespace OCRA::Instance
{
struct Impl {
    Impl(const Info& a_Info);
    ~Impl();
    const VkInstance instance;
    const std::vector<PhysicalDevice::Handle> physicalDevices;
    const Info info;
};
}