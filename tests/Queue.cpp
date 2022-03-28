#include <Instance.hpp>
#include <PhysicalDevice.hpp>
#include <Device.hpp>

#include <iostream>

using namespace OCRA;

int Queue()
{
	Instance::Handle instance;
	std::cout << "==== Instance ====\n";
	{
		Instance::Info instanceInfo;
		instanceInfo.applicationInfo.name = "Test";
		instanceInfo.applicationInfo.applicationVersion = 1;
		instance = Instance::Create(instanceInfo);
		std::cout << "  Type           : " << Instance::GetType(instance) << "\n";
		std::cout << "  App Name       : " << Instance::GetInfo(instance).applicationInfo.name << "\n";
		std::cout << "  App Version    : " << Instance::GetInfo(instance).applicationInfo.applicationVersion << "\n";
		std::cout << "  Engine Name    : " << Instance::GetInfo(instance).applicationInfo.engineName << "\n";
		std::cout << "  Engine Version : " << Instance::GetInfo(instance).applicationInfo.engineVersion << "\n";
	}
	std::cout << std::endl;
	std::cout << "==== Physical Devices ====\n";
	for (const auto& physicalDevice : Instance::EnumeratePhysicalDevices(instance))
	{
		const auto& properties = PhysicalDevice::GetProperties(physicalDevice);
		std::cout << "  Name        : " << properties.deviceName << "\n";
		std::cout << "  Vendor      : " << properties.vendorName << "\n";
		std::cout << "  API version : " << properties.apiVersion << "\n";
		std::cout << "==========================\n";
	}
	std::cout << std::endl;
	std::cout << "==== Queue Families ====\n";
	std::vector<Queue::Info> queueInfos;
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
			std::cout << "  Index         : " << familyIndex << "\n";
			std::cout << "  Count         : " << queueFamily.queueCount << "\n";
			std::cout << " == Capabilities ==\n";
			std::cout << "  Graphics      : " << ((queueFamily.queueFlags & PhysicalDevice::QueueFlagsBits::Graphics) != 0) << "\n";
			std::cout << "  Compute       : " << ((queueFamily.queueFlags & PhysicalDevice::QueueFlagsBits::Compute) != 0) << "\n";
			std::cout << "  Protected     : " << ((queueFamily.queueFlags & PhysicalDevice::QueueFlagsBits::Protected) != 0) << "\n";
			std::cout << "  SparseBinding : " << ((queueFamily.queueFlags & PhysicalDevice::QueueFlagsBits::SparseBinding) != 0) << "\n";
			std::cout << "  Transfer      : " << ((queueFamily.queueFlags & PhysicalDevice::QueueFlagsBits::Transfer) != 0) << "\n";
			std::cout << "==========================\n";
			++familyIndex;
		}
	}
	std::cout << std::endl;
	{
		Device::Info deviceInfo;
		deviceInfo.queueInfos = queueInfos;
		//Get first physical device
		auto& physicalDevice = Instance::EnumeratePhysicalDevices(instance).front();
		auto logicalDevice = Device::Create(physicalDevice, deviceInfo);
		std::cout << "Queue Handle : " << Device::GetQueue(logicalDevice, 0, 0);
	}
	std::cout << std::endl;
	return 0;
}