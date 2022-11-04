#pragma once

#include <Handle.hpp>
#include <Buffer.hpp>

#include <WeakHandle.hpp>

#include <cassert>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Buffer);
OCRA_DECLARE_HANDLE(OCRA::Memory);

namespace OCRA::Buffer
{
struct MemoryBinding {
    Memory::Handle  memory;
    size_t          offset{ 0 };
};
struct Impl {
    Impl(const Device::Handle& a_Device, const Info& a_Info)
        : device(a_Device)
        , info(a_Info)
    {
        assert(info.usage != UsageFlagBits::None);
    }
    const Device::WeakHandle    device;
    const Info                  info;
    MemoryBinding               memoryBinding;
};
}