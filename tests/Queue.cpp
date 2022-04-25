#include <Instance.hpp>
#include <PhysicalDevice.hpp>
#include <Device.hpp>

#include <Common.hpp>

#include <iostream>

using namespace OCRA;

//Find suitable device
static inline auto GetPhysicalDevice(const Instance::Handle& a_Instance)
{
	const auto& physicalDevices = Instance::EnumeratePhysicalDevices(a_Instance);
	return physicalDevices.front();
}

//Create a device with necessary queues
static inline auto CreateDevice(const PhysicalDevice::Handle& a_PhysicalDevice, const std::vector<Queue::Info>& a_QueueInfos)
{
	Device::Info deviceInfo;
	deviceInfo.queueInfos = a_QueueInfos;
	auto device = Device::Create(a_PhysicalDevice, deviceInfo);
	return device;
}

static inline void ListQueues(const Device::Handle& a_Device, const std::vector<Queue::Info>& a_QueueInfos)
{
	Device::Info deviceInfo;
	deviceInfo.queueInfos = a_QueueInfos;
	//Get first physical device
	std::cout << "===== Device's Queues =====\n";
	for (const auto& queueInfo : a_QueueInfos)
	{
		for (auto queueIndex = 0u; queueIndex < queueInfo.queueCount; ++queueIndex) {
			const auto queue = Device::GetQueue(a_Device, queueInfo.queueFamilyIndex, queueIndex);
			std::cout << "  == Queue ==\n";
			std::cout << "  Family    : " << queueInfo.queueFamilyIndex << "\n";
			std::cout << "  Index     : " << queueIndex << "\n";
			std::cout << "  Handle    : " << queue << "\n";
			std::cout << "  ===========\n";
		}
	}
	std::cout << "===========================\n";
	std::cout << "\n";
}

int Queue()
{
	const auto instance = CreateInstance("Test_Queue");
	const auto physicalDevice = GetPhysicalDevice(instance);
	const auto queueInfos = GetQueueInfos(physicalDevice);
	const auto device = CreateDevice(physicalDevice, queueInfos);
	ListQueues(device, queueInfos);
	std::cout << std::endl;
	return 0;
}