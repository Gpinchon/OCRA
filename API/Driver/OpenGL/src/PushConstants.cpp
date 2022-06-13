#include <GL/PushConstants.hpp>
#include <GL/Device.hpp>

#include <GL/glew.h>

namespace OCRA
{
constexpr auto PushConstantBinding = 64;
constexpr auto PushConstantMultiBuffering = 3;
int RoundUp(int numToRound, int multiple)
{
	if (multiple == 0)
		return numToRound;

	int remainder = numToRound % multiple;
	if (remainder == 0)
		return numToRound;

	return numToRound + multiple - remainder;
}

static inline auto PushConstantOffsetAlignment(const Device::Handle& a_Device) {
	GLint offset = 0;
	a_Device->PushCommand([&offset] {
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &offset);
		}, true);
	return offset;
}

static inline auto CreatePushConstantBuffer(const Device::Handle& a_Device, const size_t& a_Size) {
	const auto size = RoundUp(a_Size, PushConstantOffsetAlignment(a_Device)) * PushConstantMultiBuffering;
	GLuint handle = 0;
	if (size > 0) {
		a_Device->PushCommand([&handle, size] {
			glGenBuffers(1, &handle);
			glBindBuffer(GL_UNIFORM_BUFFER, handle);
			glBufferStorage(GL_UNIFORM_BUFFER, size, nullptr, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}, true);
	}
	return handle;
}

PushConstants::PushConstants(const Device::Handle& a_Device)
	: device(a_Device)
	, size(256)
	, offsetAlignment(PushConstantOffsetAlignment(a_Device))
	, bufferHandle(CreatePushConstantBuffer(a_Device, size))
{}

PushConstants::~PushConstants()
{
	device.lock()->PushCommand([handle = bufferHandle] {
		glDeleteBuffers(1, &handle);
	}, false);
}

void PushConstants::Bind() {
	if (size > 0) {
		const auto currentOffset = offset * offsetAlignment;
		glBindBufferRange(GL_UNIFORM_BUFFER, PushConstantBinding, bufferHandle, currentOffset, size);
	}
}

void PushConstants::Update(const size_t& a_Offset, const std::vector<char>& a_Data) {
	++offset %= PushConstantMultiBuffering;
	const auto currentOffset = offset * offsetAlignment;
	glBindBuffer(GL_UNIFORM_BUFFER, bufferHandle);
	glBufferSubData(GL_UNIFORM_BUFFER,
		a_Offset + currentOffset,
		a_Data.size(),
		a_Data.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
}