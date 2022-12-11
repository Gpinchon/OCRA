#pragma once

#include <OCRA/FrameBuffer.hpp>
#include <OCRA/Handle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::FrameBuffer
{
struct Impl {
    Impl(const Device::Handle& a_Device, const Info& a_Info);
    ~Impl();
    void Bind();
    void Unbind();
    const Device::WeakHandle device;
    const Info info;
    const uint32_t handle{ 0 };
};
}