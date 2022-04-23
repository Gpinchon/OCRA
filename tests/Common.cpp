#include <Instance.hpp>
#include <PhysicalDevice.hpp>
#include <Device.hpp>

#include <iostream>

using namespace OCRA;

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

//Get queues from Physical Device
std::vector<Queue::Info> GetQueueInfos(const PhysicalDevice::Handle& a_PhysicalDevice)
{
	std::vector<Queue::Info> queueInfos;
	{
		auto& queueFamilies = PhysicalDevice::GetQueueFamilyProperties(a_PhysicalDevice);
		uint32_t familyIndex = 0;
		for (auto& queueFamily : queueFamilies)
		{
			Queue::Info queueInfo;
			queueInfo.queueCount = queueFamily.queueCount;
			queueInfo.queueFamilyIndex = familyIndex;
			queueInfo.queuePriorities.resize(queueFamily.queueCount, 1.f);
			queueInfos.push_back(queueInfo);
			++familyIndex;
		}
	}
	return queueInfos;
}

Device::Handle CreateDevice(const PhysicalDevice::Handle& a_PhysicalDevice)
{
	Device::Info deviceInfo;
	deviceInfo.queueInfos = GetQueueInfos(a_PhysicalDevice);
	return Device::Create(a_PhysicalDevice, deviceInfo);
}