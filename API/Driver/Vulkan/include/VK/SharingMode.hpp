#pragma once

#include <OCRA/Enums.hpp>

#include <vulkan/vulkan.h>

namespace OCRA
{
static inline auto GetVkSharingMode(const SharingMode& a_SharingMode) {
	switch (a_SharingMode)
	{
	case SharingMode::Exclusive :
		return VK_SHARING_MODE_EXCLUSIVE;
	case SharingMode::Concurrent :
		return VK_SHARING_MODE_EXCLUSIVE;
	}
    return VK_SHARING_MODE_MAX_ENUM;
}
}