#pragma once

#include <OCRA/Handles.hpp>

#include <string>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Instance);

namespace OCRA::Surface
{
struct Impl
{
    Impl(const Instance::Handle& a_Instance, const CreateSurfaceInfo& a_Info);
    ~Impl();
    const std::string type;
    const Instance::WeakHandle instance;
    const void* hwnd;
    const void* hdc;
};
}