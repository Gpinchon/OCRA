#pragma once

#include <Image/Sampler.hpp>

#include <Handle.hpp>
#include <WeakHandle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Image::Sampler {
struct Impl {
	Impl(const Device::Handle& a_Device, const Info& a_Info);
	~Impl();
	const Device::WeakHandle device;
	const uint32_t handle;
	const Info     info;
};
}