#pragma once
/**
 * @brief Unit test's common tools
 */

#include <Handle.hpp>
#include <Queue/Queue.hpp>

#include <Common/Timer.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <bitset>

OCRA_DECLARE_HANDLE(OCRA::Instance);
OCRA_DECLARE_HANDLE(OCRA::PhysicalDevice);
OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Command::Pool);

namespace OCRA::PhysicalDevice
{
using QueueFlags = std::bitset<5>;
}

namespace OCRA {
struct VerboseTimer : Timer
{
	VerboseTimer(const std::string& a_Name) : name(a_Name) {}
	~VerboseTimer() {
		std::cout << "Timer \"" << name << "\" waited : " << Elapsed().count() << " nanoseconds\n";
	}
	const std::string name;
};

//Create an instance with app name a_Name
Instance::Handle CreateInstance(const std::string& a_Name);
//Create a device with all available queues
Device::Handle CreateDevice(const PhysicalDevice::Handle& a_PhysicalDevice);
//Get queues from Physical Device
std::vector<Queue::Info> GetQueueInfos(const PhysicalDevice::Handle& a_PhysicalDevice);
//Find suitable queue for the specified flags
uint32_t FindQueueFamily(const PhysicalDevice::Handle& a_PhysicalDevice, const PhysicalDevice::QueueFlags& a_QueueProperties);
//Create a command pool from the queuefamily
Command::Pool::Handle CreateCommandPool(const Device::Handle& a_Device, const uint32_t& a_QueueFamily);
}