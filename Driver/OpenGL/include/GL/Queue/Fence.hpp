#pragma once

namespace OCRA::Queue::Fence {
void Signal(
	const Device::Handle& a_Device,
	const std::vector<Handle>& a_Fences);
}