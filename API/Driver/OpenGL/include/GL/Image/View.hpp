/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:31:47
*/
#pragma once

#include <OCRA/Core.hpp>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Image::View {
struct Impl {
    Impl(const Device::Handle& a_Device, const CreateImageViewInfo& a_Info);
    ~Impl();
    const Device::WeakHandle  device;
    const CreateImageViewInfo info;
    const uint32_t target;
    const uint32_t format;
    const uint32_t handle;
};
}