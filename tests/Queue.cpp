#include <Instance.hpp>
#include <PhysicalDevice.hpp>
#include <Device.hpp>

#include <iostream>

using namespace OCRA;

int Queue()
{
	Instance::Handle instance;
	std::vector<Queue::Info> queueInfos;
	std::cout << "==== Instance ====\n";
	{
		Instance::Info instanceInfo;
		instanceInfo.applicationInfo.name = "Test_Queue";
		instanceInfo.applicationInfo.applicationVersion = 1;
		instance = Instance::Create(instanceInfo);
		std::cout << "  Type           : " << Instance::GetType(instance) << "\n";
		std::cout << "  App Name       : " << Instance::GetInfo(instance).applicationInfo.name << "\n";
		std::cout << "  App Version    : " << Instance::GetInfo(instance).applicationInfo.applicationVersion << "\n";
		std::cout << "  Engine Name    : " << Instance::GetInfo(instance).applicationInfo.engineName << "\n";
		std::cout << "  Engine Version : " << Instance::GetInfo(instance).applicationInfo.engineVersion << "\n";
	}
	std::cout << "==================\n";
	std::cout << "\n";
	std::cout << "==== Queue Families ====\n";
	{
		auto& physicalDevice = Instance::EnumeratePhysicalDevices(instance).at(0);
		auto& queueFamilies = PhysicalDevice::GetQueueFamilyProperties(physicalDevice);
		uint32_t familyIndex = 0;
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
	}
	std::cout << "========================\n";
	std::cout << "\n";
	std::cout << "===== Device's Queues =====\n";
	{
		Device::Info deviceInfo;
		deviceInfo.queueInfos = queueInfos;
		//Get first physical device
		auto& physicalDevice = Instance::EnumeratePhysicalDevices(instance).front();
		auto logicalDevice = Device::Create(physicalDevice, deviceInfo);
		for (const auto& queueInfo : queueInfos)
		{
			for (auto queueIndex = 0u; queueIndex < queueInfo.queueCount; ++queueIndex) {
				const auto queue = Device::GetQueue(logicalDevice, queueInfo.queueFamilyIndex, queueIndex);
				std::cout << "  == Queue ==\n";
				std::cout << "  Family    : " << queueInfo.queueFamilyIndex << "\n";
				std::cout << "  Index     : " << queueIndex << "\n";
				std::cout << "  Handle    : " << queue << "\n";
				std::cout << "  ===========\n";
			}
		}
	}
	std::cout << "===========================\n";
	std::cout << std::endl;
	return 0;
}