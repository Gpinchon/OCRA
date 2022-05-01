#pragma once

#include <SwapChain.hpp>

#include <GL/SwapChain.hpp>

namespace OCRA::Win32::SwapChain
{
struct Impl : OCRA::SwapChain::Impl
{
	Impl(const Device::Handle& a_Device, const OCRA::SwapChain::Info& a_Info) : OCRA::SwapChain::Impl(a_Device, a_Info) {};
	virtual void Present(const Queue::Handle& a_Queue, const uint32_t& a_ImageIndex) override;
};
}