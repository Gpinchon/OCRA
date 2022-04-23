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

//Get queues from Physical Device
static inline auto GetQueueInfos(const PhysicalDevice::Handle& a_PhysicalDevice)
{
	std::vector<Queue::Info> queueInfos;
	auto& queueFamilies = PhysicalDevice::GetQueueFamilyProperties(a_PhysicalDevice);
	uint32_t familyIndex = 0;
	std::cout << "==== Queue Families ====\n";
	for (auto& queueFamily : queueFamilies)
	{
		Queue::Info queueInfo;
		queueInfo.queueCount = queueFamily.queueCount;
		queueInfo.queueFamilyIndex = familyIndex;
		queueInfo.queuePriorities.resize(queueFamily.queueCount, 1.f);
		queueInfos.push_back(queueInfo);
		std::cout << " ==================\n";
		std::cout << "  Index         : " << familyIndex << "\n";
		std::cout << "  Count         : " << queueFamily.queueCount << "\n";
		std::cout << " == Capabilities ==\n";
		std::cout << "  Graphics      : " << ((queueFamily.queueFlags & PhysicalDevice::QueueFlagsBits::Graphics) != 0) << "\n";
		std::cout << "  Compute       : " << ((queueFamily.queueFlags & PhysicalDevice::QueueFlagsBits::Compute) != 0) << "\n";
		std::cout << "  Protected     : " << ((queueFamily.queueFlags & PhysicalDevice::QueueFlagsBits::Protected) != 0) << "\n";
		std::cout << "  SparseBinding : " << ((queueFamily.queueFlags & PhysicalDevice::QueueFlagsBits::SparseBinding) != 0) << "\n";
		std::cout << "  Transfer      : " << ((queueFamily.queueFlags & PhysicalDevice::QueueFlagsBits::Transfer) != 0) << "\n";
		std::cout << " ==================\n";
		++familyIndex;
	}
	std::cout << "========================\n";
	std::cout << "\n";
	return queueInfos;
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