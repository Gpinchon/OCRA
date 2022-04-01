#include <Instance.hpp>
#include <PhysicalDevice.hpp>
#include <Device.hpp>
#include <Queue/Queue.hpp>
#include <Queue/Fence.hpp>
#include <Command/Buffer.hpp>
#include <Memory.hpp>
#include <Buffer.hpp>

#include <iostream>

using namespace OCRA;

//Create OCRA Instance
auto CreateInstance()
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
	return instance;
}

//Get queues from Physical Device
auto GetQueueInfos(const PhysicalDevice::Handle& a_PhysicalDevice)
{
	std::vector<Queue::Info> queueInfos;
	{
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
	return queueInfos;
}

//Find suitable device
auto GetPhysicalDevice(const Instance::Handle& a_Instance)
{
	const auto& physicalDevices = Instance::EnumeratePhysicalDevices(a_Instance);
	std::cout << "==== Physical Devices ====\n";
	for (const auto& physicalDevice : physicalDevices)
	{
		const auto& properties = PhysicalDevice::GetProperties(physicalDevice);
		std::cout << "  Name        : " << properties.deviceName << "\n";
		std::cout << "  Vendor      : " << properties.vendorName << "\n";
		std::cout << "  API version : " << properties.apiVersion << "\n";
		std::cout << "==========================\n";
	}
	std::cout << std::endl;
	return physicalDevices.front();
}

//Create a device with necessary queues
auto CreateDevice(const PhysicalDevice::Handle& a_PhysicalDevice)
{
	Device::Info deviceInfo;
	deviceInfo.queueInfos = GetQueueInfos(a_PhysicalDevice);
	auto device = Device::Create(a_PhysicalDevice, deviceInfo);
	return device;
}

auto GetQueue(const Device::Handle& a_Device)
{
	return Device::GetQueue(a_Device, 0, 0);
}

int CommandBuffer()
{
	auto instance = CreateInstance();
	auto physicalDevice = GetPhysicalDevice(instance);
	auto device = CreateDevice(physicalDevice);
	auto queue = GetQueue(device);
	
	//allocate GPU memory
	Memory::Info memoryInfo;
	memoryInfo.memoryTypeIndex = 0;
	memoryInfo.size = sizeof(int) * 3;
	auto memory = Memory::Allocate(device, memoryInfo);

	//create test buffers
	Buffer::Info bufferInfo;
	bufferInfo.size = sizeof(int);
	bufferInfo.usage = Buffer::UsageFlagBits::TransferDst | Buffer::UsageFlagBits::TransferSrc;
	auto buffer0 = Buffer::Create(device, bufferInfo);
	auto buffer1 = Buffer::Create(device, bufferInfo);
	auto buffer2 = Buffer::Create(device, bufferInfo);
	Buffer::BindMemory(device, buffer0, memory, sizeof(int) * 0);
	Buffer::BindMemory(device, buffer1, memory, sizeof(int) * 1);
	Buffer::BindMemory(device, buffer2, memory, sizeof(int) * 2);
	//write some value to the buffer0
	{
		auto bufferPtr = (int*)Memory::Map(device, memory, sizeof(int) * 0, sizeof(int));
		*bufferPtr = 42;
		Memory::Unmap(device, memory);
	}
	//write some value to the buffer1
	{
		auto bufferPtr = (int*)Memory::Map(device, memory, sizeof(int) * 1, sizeof(int));
		*bufferPtr = 666;
		Memory::Unmap(device, memory);
	}
	auto commandBuffer = Command::Buffer::Create(device);
	Command::CommandBufferBeginInfo commandBufferBeginInfo;
	commandBufferBeginInfo.flags = Command::CommandBufferUsageFlagBits::OneTimeSubmit;
	Command::BeginCommandBuffer(commandBuffer, commandBufferBeginInfo);
	{
		Command::BufferCopyRegion copyRegions;
		copyRegions.size = sizeof(int);
		Command::CopyBuffer(commandBuffer, buffer0, buffer2, { copyRegions });
		Command::CopyBuffer(commandBuffer, buffer1, buffer0, { copyRegions });
		Command::CopyBuffer(commandBuffer, buffer2, buffer1, { copyRegions });
	}
	Command::EndCommandBuffer(commandBuffer);
	{
		Queue::SubmitInfo submitInfo;
		auto fence = Queue::Fence::Create(device);
		submitInfo.commandBuffers.push_back(commandBuffer);
		Queue::Submit(queue, { submitInfo }, fence);
		Queue::Fence::WaitFor(device, { fence }, true, 10000);
	}
	//check if values were swapped
	int success = 0;
	{
		auto bufferPtr = (int*)Memory::Map(device, memory, sizeof(int) * 0, sizeof(int));
		success += *bufferPtr == 666 ? 0 : 1;
		std::cout << "Buffer value : " << *bufferPtr << std::endl;
		Memory::Unmap(device, memory);
	}
	{
		auto bufferPtr = (int*)Memory::Map(device, memory, sizeof(int) * 1, sizeof(int));
		success += *bufferPtr == 42 ? 0 : 1;
		std::cout << "Buffer value : " << *bufferPtr << std::endl;
		Memory::Unmap(device, memory);
	}
	return success;
}