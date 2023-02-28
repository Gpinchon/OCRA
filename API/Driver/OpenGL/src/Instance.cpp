#include <OCRA/Core.hpp>

#include <GL/Instance.hpp>
#include <GL/Win32/Instance.hpp>

namespace OCRA
{
Instance::Handle CreateInstance(
    const CreateInstanceInfo& a_Info,
    const AllocationCallback* a_Allocator)
{
#ifdef _WIN32
    return Win32::CreateInstance(a_Info);
#endif //_WIN32
}
}

namespace OCRA::Instance
{
//for multi instances
static auto s_ID = 0u;
Impl::Impl()
    : id(s_ID++)
{}

const std::string GetType(const Handle& a_Instance)
{
    return Impl::type;
}

const std::vector<PhysicalDevice::Handle> EnumeratePhysicalDevices(const Instance::Handle& a_Instance)
{
    return a_Instance->physicalDevices;
}
}