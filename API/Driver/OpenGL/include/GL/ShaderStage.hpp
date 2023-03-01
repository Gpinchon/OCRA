/*
* @Author: gpinchon
* @Date:   2021-10-04 20:24:27
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-04 20:25:30
*/
#pragma once

#include <OCRA/Core.hpp>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Shader::Stage {
struct Impl {
    Impl(const Device::Handle& a_Device, const CreateShaderStageInfo& a_Info);
    ~Impl();
    const Device::WeakHandle device;
    const uint32_t handle;
    const uint32_t stage;
    const uint32_t stageBits;
};
}