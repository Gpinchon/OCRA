#include <Memory.hpp>
#include <PhysicalDevice.hpp>

#include <GL/Device.hpp>
#include <GL/WeakHandle.hpp>
#include <GL/glew.h>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Memory
{
struct Impl
{
	Impl(const Device::Handle& a_Device, const Info& a_Info)
		: device(a_Device)
		, info(a_Info)
	{
		auto& memoryProperties = PhysicalDevice::GetMemoryProperties(Device::GetPhysicalDevice(a_Device));
		auto& memoryTypeFlags = memoryProperties.memoryTypes.at(info.memoryTypeIndex).propertyFlags;
		GLbitfield  allocationFlags{ 0 };
		if ((memoryTypeFlags & PhysicalDevice::MemoryPropertyFlagBits::HostCached) != 0) {
			allocationFlags |= GL_CLIENT_STORAGE_BIT;
		}
		if ((memoryTypeFlags & PhysicalDevice::MemoryPropertyFlagBits::HostVisible) != 0) {
			allocationFlags |= GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
			mapFlags |= GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT;
		}
		if ((memoryTypeFlags & PhysicalDevice::MemoryPropertyFlagBits::HostCoherent) != 0) {
			allocationFlags |= GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT;
			mapFlags |= GL_MAP_COHERENT_BIT;
		}
		Device::PushCommand(a_Device, 0, 0, [this, allocationFlags] {
			glCreateBuffers(1, &handle);
			glNamedBufferStorageEXT(
				handle,
				info.size,
				nullptr,
				allocationFlags);
		}, true);
	}
	~Impl()
	{
		Device::PushCommand(device.lock(), 0, 0, [this] {
			glDeleteBuffers(1, &handle);
		}, false);
		
	}
	const Device::WeakHandle device;
	const Info	info;
	GLuint		handle{ 0 };
	GLbitfield	mapFlags{ 0 };
};
Handle Allocate(const Device::Handle& a_Device, const Info& a_Info)
{
	return Handle(new Impl(a_Device, a_Info));
}
void* Map(
	const Device::Handle&  a_Device,
	const Handle&          a_Memory,
	const uint64_t&        a_Offset,
	const uint64_t&        a_Length)
{
	void* ptr{ nullptr };
	Device::PushCommand(a_Device, 0, 0, [a_Memory, a_Offset, a_Length, &ptr] {
		ptr = glMapNamedBufferRangeEXT(
			a_Memory->handle,
			a_Offset,
			a_Length,
			a_Memory->mapFlags);
	}, true);
	return ptr;
}
void Unmap(
	const Device::Handle&  a_Device,
	const Handle&          a_Memory)
{
	Device::PushCommand(a_Device, 0, 0, [a_Memory] {
		glUnmapNamedBufferEXT(a_Memory->handle);
	}, false);
	
}
uint32_t GetGLHandle(const Handle& a_Memory)
{
	return a_Memory->handle;
}
}