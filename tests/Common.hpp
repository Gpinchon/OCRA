#pragma once
/**
 * @brief Unit test's common tools
 */

#include <Handle.hpp>
#include <Queue/Queue.hpp>
#include <Command/Pool.hpp>
#include <PhysicalDevice.hpp>

#include <Common/Timer.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <bitset>

OCRA_DECLARE_HANDLE(OCRA::Instance);
OCRA_DECLARE_HANDLE(OCRA::PhysicalDevice);
OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Surface);
OCRA_DECLARE_HANDLE(OCRA::SwapChain);
OCRA_DECLARE_HANDLE(OCRA::Memory);
OCRA_DECLARE_HANDLE(OCRA::Command::Pool);
OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);

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
//Creates a surface using the specified instance and window handles;
Surface::Handle CreateSurface(const Instance::Handle& a_Instance, void* const a_HINSTANCE, void* const a_HWND);
//Creates or recreates a swapchain
SwapChain::Handle CreateSwapChain(
	const Device::Handle& a_Device,
	const Surface::Handle& a_Surface,
	const SwapChain::Handle& a_OldSwapChain,
	const uint32_t& a_Width, const uint32_t& a_Height,
	const uint32_t& a_ImageNbr);
//Create a device with all available queues
Device::Handle CreateDevice(const PhysicalDevice::Handle& a_PhysicalDevice);
//Get queues from Physical Device
std::vector<Queue::Info> GetQueueInfos(const PhysicalDevice::Handle& a_PhysicalDevice);
//Prints queues informations to the console
void PrintQueueInfos(const PhysicalDevice::Handle& a_PhysicalDevice);
//Find suitable queue for the specified flags
uint32_t FindQueueFamily(const PhysicalDevice::Handle& a_PhysicalDevice, const PhysicalDevice::QueueFlags& a_QueueProperties);
//Create a command pool from the queuefamily
Command::Pool::Handle CreateCommandPool(const Device::Handle& a_Device, const uint32_t& a_QueueFamily);
//Creates a command buffer from the specified command pool
Command::Buffer::Handle CreateCommandBuffer(const Device::Handle& a_Device, const Command::Pool::Handle& a_CommandPool, const Command::Pool::AllocateInfo::Level& a_Level);
//Submits a Command Buffer to the specified Queue
void SubmitCommandBuffer(const Queue::Handle& a_Queue, const Command::Buffer::Handle& a_CommandBuffer);
//Returns the index of a memory type with specified properties
uint32_t FindProperMemoryType(const PhysicalDevice::Handle& a_PhysicalDevice, const PhysicalDevice::MemoryPropertyFlags& a_MemoryProperties);
//allocate GPU memory with the specified properties
Memory::Handle AllocateMemory(const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device, const uint64_t& a_Size, const PhysicalDevice::MemoryPropertyFlags& a_MemoryProperties);
}