#pragma once

#include <Handle.hpp>
#include <Surface.hpp>
#include <Common/Extent2D.hpp>

#include <WeakHandle.hpp>

#include <functional>
#include <string>
#include <vector>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Instance);
OCRA_DECLARE_HANDLE(OCRA::Instance);

namespace OCRA::Surface
{
struct Impl
{
    Impl(const Instance::Handle& a_Instance, const void* a_NativeWindow, const std::string& a_Type)
        : type(a_Type)
        , instance(a_Instance)
        , nativeWindow(a_NativeWindow)
    {}
    const std::string type;
    const Instance::WeakHandle instance;
    const void* nativeWindow;
};
}
