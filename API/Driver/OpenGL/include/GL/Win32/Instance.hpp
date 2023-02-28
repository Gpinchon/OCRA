#pragma once
#include <OCRA/Structs.hpp>

#include <GL/Instance.hpp>

namespace OCRA::Win32
{
Instance::Handle CreateInstance(const CreateInstanceInfo& a_Info);
}

namespace OCRA::Instance::Win32
{
struct Impl : Instance::Impl
{
    Impl();
    ~Impl();
    //required for OGL context creation on Windows
    void* hwnd;
};
}
