#pragma once

#include <OCRA/Flags.hpp>

#include <vulkan/vulkan.h>

namespace OCRA
{
static inline auto GetVkBufferCreateFlags(const CreateBufferFlags& a_Flags)
{
	VkBufferCreateFlags vkFlags = 0;
    vkFlags |= (a_Flags & CreateBufferFlagBits::SparseBinding)              == CreateBufferFlagBits::SparseBinding              ? VK_BUFFER_CREATE_SPARSE_BINDING_BIT                : 0;
    vkFlags |= (a_Flags & CreateBufferFlagBits::SparseResidency)            == CreateBufferFlagBits::SparseResidency            ? VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT              : 0;
    vkFlags |= (a_Flags & CreateBufferFlagBits::SparseAliased)              == CreateBufferFlagBits::SparseAliased              ? VK_BUFFER_CREATE_SPARSE_ALIASED_BIT                : 0;
    vkFlags |= (a_Flags & CreateBufferFlagBits::Protected)                  == CreateBufferFlagBits::Protected                  ? VK_BUFFER_CREATE_PROTECTED_BIT                     : 0;
    vkFlags |= (a_Flags & CreateBufferFlagBits::DeviceAddressCaptureReplay) == CreateBufferFlagBits::DeviceAddressCaptureReplay ? VK_BUFFER_CREATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT : 0;
   return vkFlags;
}
}