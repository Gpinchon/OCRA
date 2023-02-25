#pragma once

#include <OCRA/Core.hpp>

#include <GL/Common/Assert.hpp>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Buffer
{
struct MemoryBinding {
    Memory::Handle  memory;
    size_t          offset{ 0 };
};
struct Impl {
    Impl(const Device::Handle& a_Device, const CreateBufferInfo& a_Info)
        : device(a_Device)
        , info(a_Info)
    {
        OCRA_ASSERT(info.usage != BufferUsageFlagBits::None);
    }
    const Device::WeakHandle    device;
    const CreateBufferInfo      info;
    MemoryBinding               memoryBinding;
};
}