#include <GL/Pipeline/Layout.hpp>

#include <algorithm>
#include <cassert>

namespace OCRA::Pipeline::Layout
{
static inline auto PushConstantSize(const Info& a_Info)
{
    uint32_t size = 0;
    for (const auto& range : a_Info.pushConstants)
        size = std::max(size, range.offset + range.size);
    assert(size <= 128);
    return size;
}
Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : info(a_Info)
    , pushConstantSize(PushConstantSize(a_Info))
{}
Handle Create(const Device::Handle& a_Device, const Info& a_Info, const AllocationCallback* a_Allocator)
{
    return Handle(new Impl(a_Device, a_Info));
}
}