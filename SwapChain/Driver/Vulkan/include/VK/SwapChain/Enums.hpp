#pragma once

#include <OCRA/SwapChain/Enums.hpp>

#include <vulkan/vulkan.hpp>

namespace OCRA {
static inline auto GetVkPresentMode(const SwapChainPresentMode& a_PresentMode)
{
    switch (a_PresentMode) {
    case SwapChainPresentMode::Immediate:
        return vk::PresentModeKHR::eImmediate;
    case SwapChainPresentMode::Fifo:
        return vk::PresentModeKHR::eFifo;
    case SwapChainPresentMode::Mailbox:
        return vk::PresentModeKHR::eMailbox;
    }
    return vk::PresentModeKHR(-1);
}
}
