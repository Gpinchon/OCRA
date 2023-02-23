#include <OCRA/Instance.hpp>

#include <vulkan/vulkan.hpp>

namespace OCRA::Instance
{
struct Impl {
    Impl(const Info& a_Info);
    ~Impl() {
        vkDestroyInstance(instance, nullptr);
    }
    operator VkInstance() const {
        return instance;
    }
    const VkInstance instance;
    const Info info;
};
}