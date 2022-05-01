#pragma once

#include <Handle.hpp>

#include <GL/PhysicalDevice.hpp>

OCRA_DECLARE_HANDLE(OCRA::Instance);

namespace OCRA::PhysicalDevice::Win32
{
struct Impl : PhysicalDevice::Impl
{
    Impl(const Instance::Handle& a_Instance);
    ~Impl();
    virtual void SetSurface(const Surface::Handle& a_Surface) override;
    virtual void SwapBuffers(const uint8_t a_SwapInterval) override;
};
}