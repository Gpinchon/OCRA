#pragma once

#include <OCRA/Handle.hpp>

#include <GL/PhysicalDevice.hpp>

OCRA_DECLARE_HANDLE(OCRA::Instance);

namespace OCRA::PhysicalDevice::Win32 {
struct Impl : PhysicalDevice::Impl {
    Impl(const Instance::Handle& a_Instance);
    ~Impl();
    const void* hdc;
    void* contextHandle;
};
}
