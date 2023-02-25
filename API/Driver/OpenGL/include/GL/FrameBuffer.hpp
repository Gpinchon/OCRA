#pragma once

#include <OCRA/Core.hpp>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::FrameBuffer
{
struct Impl {
    Impl(const Device::Handle& a_Device, const CreateFrameBufferInfo& a_Info);
    ~Impl();
    void Bind();
    void Unbind();
    const Device::WeakHandle    device;
    const CreateFrameBufferInfo info;
    const uint32_t handle{ 0 };
};
}