#pragma once

#include <Handle.hpp>

#include <GL/WeakHandle.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Layout);

namespace OCRA {
struct PushConstants {
	PushConstants(const Device::Handle& a_Device);
	~PushConstants();
	void Bind();
	void Update(const size_t& a_Offset, const std::vector<char>& a_Data);
	const Device::WeakHandle device;
	const uint32_t	size;
	const uint32_t	offsetAlignment;
	const uint32_t	bufferHandle;
	//the current offset of pushconstant buffer
	uint32_t		offset{ 0 };
};
}