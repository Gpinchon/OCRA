#pragma once

#include <Handle.hpp>
#include <Descriptor/Type.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::Pool);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::SetLayout);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::Set);

namespace OCRA {
struct AllocationCallback;
}

/**
* One muse always make sure Descriptor Pool outlives its Descriptor Sets
*/
namespace OCRA::Descriptor::Pool
{
struct Size
{
    Type        type{ Type::Unknown };
    uint32_t    count{ 0 }; //nbr of allocated types, byte size for InlineUniformBlock
};
struct Info
{
    uint32_t            maxSets{ 0 };
    std::vector<Size>   sizes;
};
struct AllocateInfo
{
    Pool::Handle                    pool;
    std::vector<SetLayout::Handle>  layouts;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info, const AllocationCallback* a_Allocator = nullptr);
std::vector<Set::Handle> AllocateSet(const Device::Handle& a_Device, const AllocateInfo& a_Info);
}