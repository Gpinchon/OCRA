#pragma once

#include <Image/Sampler.hpp>

#include <GL/WeakHandle.hpp>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Image::Sampler {
struct Impl {
	const Device::WeakHandle device;
	const uint32_t handle;
	const Info     info;
};
}