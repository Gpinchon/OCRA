#pragma once

#include <OCRA/Core.hpp>
#include <OCRA/SwapChain/Core.hpp>

namespace OCRA::Queue
{
/**
* Presents the current Back Buffer to the Surface
*/
void Present(
    const Queue::Handle& a_Queue,
    const SwapChainPresentInfo& a_PresentInfo);
}