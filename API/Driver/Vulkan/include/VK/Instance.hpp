#include <OCRA/Handle.hpp>
#include <OCRA/Instance.hpp>

#include <vulkan/vulkan.hpp>

namespace OCRA::Instance
{
struct Impl {
    Impl(const Info& a_Info);
    ~Impl();
    operator VkInstance() {
        return instance;
    }
    const VkInstance instance;
    const Info info;
};
}