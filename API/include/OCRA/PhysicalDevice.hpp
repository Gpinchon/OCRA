#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Memory.hpp>
#include <OCRA/Structs.hpp>

#include <vector>

namespace OCRA::PhysicalDevice
{
const std::vector<QueueFamilyProperties> GetQueueFamilyProperties(const Handle& a_PhysicalDevice);

/**
 * @eturn the memory properties of the specified physical device
 */
const MemoryProperties GetMemoryProperties(const Handle& a_PhysicalDevice);

/**
 * @return the properties of the specified physical device
 */
const PhysicalDeviceProperties GetProperties(const Handle& a_PhysicalDevice);

/**
 * @return the available features of the specified physical device
 */
const PhysicalDeviceFeatures GetFeatures(const Handle& a_PhysicalDevice);

/**
 * @brief try finding a queue with all the specified flags, returns 'infinity' if none was found
 */
uint32_t FindQueueFamily(const Handle& a_PhysicalDevice, const QueueFlags& a_QueueFlags);

/**
* @brief creates a logical device that'll use the specified physical device
*/
Device::Handle CreateDevice(
    const PhysicalDevice::Handle& a_PhysicalDevice,
    const CreateDeviceInfo& a_CreateInfo,
    const AllocationCallback* a_Allocator = nullptr);
}