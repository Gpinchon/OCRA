#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Descriptor/Type.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::Pool);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::Set);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::SetLayout);

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
    uint32_t maxSets = 0;
    std::vector<Size>   sizes;//the types the allocated descriptors can contain
};
struct AllocateInfo
{
    Pool::Handle       pool;
    SetLayout::Handle  layout;
};
/**
* Creates a descriptor pool with specified size
* Pools will always enable for modification after binding and freeing sets
*/
Handle Create(const Device::Handle& a_Device, const Info& a_Info, const AllocationCallback* a_Allocator = nullptr);
/**
* Allocates a descriptor with the corresponding layout
*/
Descriptor::Set::Handle AllocateSet(const Device::Handle& a_Device, const AllocateInfo& a_Info);
}