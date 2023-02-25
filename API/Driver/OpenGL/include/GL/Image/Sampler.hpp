#pragma once

#include <OCRA/Core.hpp>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Image::Sampler {
struct Impl {
    Impl(const Device::Handle& a_Device, const CreateSamplerInfo& a_Info);
    ~Impl();
    const Device::WeakHandle device;
    const uint32_t handle;
    const CreateSamplerInfo info;
}; 
}