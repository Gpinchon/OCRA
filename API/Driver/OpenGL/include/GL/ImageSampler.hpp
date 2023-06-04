#pragma once

#include <OCRA/Core.hpp>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Image::Sampler {
struct Impl {
    Impl(const Device::Handle& a_Device, const CreateImageSamplerInfo& a_Info);
    ~Impl();
    const Device::WeakHandle device;
    const CreateImageSamplerInfo info;
    uint32_t handle;
};
}
