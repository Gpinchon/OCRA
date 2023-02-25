#pragma once

#include <OCRA/Handles.hpp>
#include <OCRA/SwapChain/Core.hpp>

namespace OCRA::Instance
{
/**
* Creates a SwapChain using the given settings
* /!\ When using OGL on Win32, you HAVE to recreate a SwapChain on resize /!\
*/
Surface::Handle CreateSurface(
    const Instance::Handle& a_Instance,
    const CreateSurfaceInfo& a_Info);
}
