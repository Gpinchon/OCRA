#pragma once

#include <OCRA/SwapChain/Enums.hpp>

#include <vulkan/vulkan.h>

namespace OCRA
{
static inline auto GetVkPresentMode(const SwapChainPresentMode& a_PresentMode)
{
	switch (a_PresentMode)
	{
	case SwapChainPresentMode::Immediate:
		return VkPresentModeKHR::VK_PRESENT_MODE_IMMEDIATE_KHR;
	case SwapChainPresentMode::Fifo:
		return VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
	case SwapChainPresentMode::Mailbox:
		return VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR;
	}
	return VK_PRESENT_MODE_MAX_ENUM_KHR;
}
}
