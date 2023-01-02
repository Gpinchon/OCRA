/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/PhysicalDevice.hpp>
#include <OCRA/Queue/Queue.hpp>

#include <vector>
#include <string>

OCRA_DECLARE_HANDLE(OCRA::Device);

namespace OCRA {
struct AllocationCallback;
}

namespace OCRA::Device {
struct Info {
    std::vector<Queue::Info>    queueInfos;
    std::vector<std::string>    enabledExtensionNames;
    PhysicalDevice::Features    enabledFeatures;
};
/**
* @brief creates a logical device that'll use the specified physical device
*/
Handle Create(
    const PhysicalDevice::Handle& a_PhysicalDevice,
    const Info& a_Info,
    const AllocationCallback* a_Allocator = nullptr);

/**
* @brief on OpenGL, there is only one queue
* @return the handle to a que of specified Family.
* To get family index, use GetQueueFamilyProperties
*/
Queue::Handle GetQueue(
    const Handle& a_Device,
    uint32_t a_FamilyIndex,
    uint32_t a_QueueIndex);
/**
* Wait for the device to finish commands execution
* In OGL, this will push an empty synchronized command
*/
void WaitIdle(const Handle& a_Device);
}