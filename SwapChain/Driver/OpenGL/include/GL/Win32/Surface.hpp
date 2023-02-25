#pragma once

#include <GL/Surface.hpp>

namespace OCRA::Surface::Win32
{
struct Impl : Surface::Impl
{
    Impl(const Instance::Handle& a_Instance, const CreateSurfaceInfo& a_Info);
    ~Impl();
    const void* hdc;
};
}