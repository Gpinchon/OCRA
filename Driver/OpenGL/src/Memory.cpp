#include <Memory.hpp>
#include <PhysicalDevice.hpp>

#include <GL/Device.hpp>
#include <GL/glew.h>

namespace OCRA::Memory
{
struct Impl
{
	Impl(const Device::Handle& a_Device, const Info& a_Info)
		: info(a_Info)
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
		glCreateBuffers(1, &handle);
		glNamedBufferStorageEXT(
			handle,
			info.size,
			nullptr,
			allocationFlags);
	}
	~Impl()
	{
		glDeleteBuffers(1, &handle);
	}
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
	return glMapNamedBufferRangeEXT(
		a_Memory->handle,
		a_Offset,
		a_Length,
		a_Memory->mapFlags);
}
void Unmap(
	const Device::Handle&  a_Device,
	const Handle&          a_Memory)
{
	glUnmapNamedBufferEXT(a_Memory->handle);
}
uint32_t GetGLHandle(const Handle& a_Memory)
{
	return a_Memory->handle;
}
}