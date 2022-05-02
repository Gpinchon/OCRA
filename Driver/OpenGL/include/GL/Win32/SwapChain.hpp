#pragma once

#include <GL/SwapChain.hpp>

namespace OCRA::SwapChain::Win32
{
struct Impl : SwapChain::Impl
{
	Impl(const Device::Handle& a_Device, const Info& a_Info);
	virtual void Present(const Queue::Handle& a_Queue, const uint32_t& a_ImageIndex) override;
	void* device;
	void* deviceContext;
	void* swapChain;
	void* glHandleD3D;
	uint32_t renderTextureHandle;
}
}