#pragma once

#include <OCRA/Handles.hpp>
#include <OCRA/SwapChain/Core.hpp>

namespace OCRA::Device
{
/**
* Creates a SwapChain using the given settings
* /!\ When using OGL on Win32, you HAVE to recreate a SwapChain on resize /!\
*/
SwapChain::Handle CreateSwapChain(
    const Device::Handle&      a_Device,
    const CreateSwapChainInfo& a_Info);
}
