#pragma once

#include <GL/SwapChain.hpp>

#include <d3d11.h>

namespace OCRA::SwapChain::Win32
{
struct Impl : SwapChain::Impl
{
	Impl(const Device::Handle& a_Device, const Info& a_Info);
	~Impl();
	virtual void Retire() override;
	virtual void Present(const Queue::Handle& a_Queue, const uint32_t& a_ImageIndex) override;
	ID3D11Device*		 deviceD3D;
	ID3D11DeviceContext* deviceContextD3D;
	IDXGISwapChain*		 swapChainD3D;
	HANDLE				 glHandleD3D;
	uint32_t			 renderTextureHandle;
};
}