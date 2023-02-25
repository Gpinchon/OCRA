#pragma once

#include <OCRA/Core.hpp>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Memory
{
struct Impl
{
    Impl(const Device::Handle& a_Device, const AllocateMemoryInfo& a_Info);
    ~Impl();
    const Device::WeakHandle device;
    const AllocateMemoryInfo info;
    uint32_t    handle{ 0 };
    uint32_t    mapFlags{ 0 };
};
}
