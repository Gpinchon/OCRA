#include <Instance.hpp>
#include <PhysicalDevice.hpp>
#include <Device.hpp>
#include <Queue/Queue.hpp>
#include <Command/Pool.hpp>

#include <iostream>
#include <vector>

namespace OCRA
{
//Create OCRA Instance
Instance::Handle CreateInstance(const std::string& a_Name)
{
	Instance::Handle instance;
	Instance::Info instanceInfo;
	instanceInfo.applicationInfo.name = a_Name;
	instanceInfo.applicationInfo.applicationVersion = 1;
	instance = Instance::Create(instanceInfo);
	std::cout << "==== Instance ====\n";
	std::cout << "  Type           : " << Instance::GetType(instance) << "\n";
	std::cout << "  App Name       : " << Instance::GetInfo(instance).applicationInfo.name << "\n";
	std::cout << "  App Version    : " << Instance::GetInfo(instance).applicationInfo.applicationVersion << "\n";
	std::cout << "  Engine Name    : " << Instance::GetInfo(instance).applicationInfo.engineName << "\n";
	std::cout << "  Engine Version : " << Instance::GetInfo(instance).applicationInfo.engineVersion << "\n";
	std::cout << "==================\n";
	std::cout << "\n";
	return instance;
}

std::vector<Queue::Info> GetQueueInfos(const PhysicalDevice::Handle& a_PhysicalDevice)
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

Device::Handle CreateDevice(const PhysicalDevice::Handle& a_PhysicalDevice)
{
	Device::Info deviceInfo;
	deviceInfo.queueInfos = GetQueueInfos(a_PhysicalDevice);
	return Device::Create(a_PhysicalDevice, deviceInfo);
}

uint32_t FindQueueFamily(const PhysicalDevice::Handle& a_PhysicalDevice, const PhysicalDevice::QueueFlags& a_QueueProperties)
{
	auto& queueProperties = PhysicalDevice::GetQueueFamilyProperties(a_PhysicalDevice);
	for (auto familyIndex = 0u; familyIndex < queueProperties.size(); ++familyIndex) {
		if (queueProperties.at(familyIndex).queueFlags == a_QueueProperties)
			return familyIndex;
	}
	return std::numeric_limits<uint32_t>::infinity();
}

Command::Pool::Handle CreateCommandPool(const Device::Handle& a_Device, const uint32_t& a_QueueFamily)
{
	Command::Pool::Info commandPoolInfo;
	commandPoolInfo.queueFamilyIndex = a_QueueFamily;
	return Command::Pool::Create(a_Device, commandPoolInfo);
}
}