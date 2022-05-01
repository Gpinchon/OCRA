#pragma once

#include <GL/Win32/SwapChain.hpp>
#include <GL/Image/Image.hpp>
#include <GL/Queue/Queue.hpp>
#include <GL/Device.hpp>
#include <GL/PhysicalDevice.hpp>
#include <GL/Surface.hpp>

#include <GL/glew.h>
#include <GL/wglew.h>

#include <cassert>

namespace OCRA::Win32::SwapChain
{
void Impl::Present(const Queue::Handle& a_Queue, const uint32_t& a_ImageIndex)
{
	assert(!retired);
    const auto physicalDevice = device.lock()->physicalDevice.lock();
    physicalDevice->SetSurface(info.surface);
    a_Queue->PushCommand([this, imageIndex = a_ImageIndex] {
        RECT windowRect;
        GetWindowRect(
            HWND(info.surface->nativeWindow),
            &windowRect);
        const auto windowWidth = windowRect.right - windowRect.left;
        const auto windowHeight = windowRect.bottom - windowRect.top;
        glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBufferHandle);
        glFramebufferTexture2D(
            GL_READ_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            images.at(imageIndex)->handle,
            0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(
            0,
            0,
            windowWidth,
            windowHeight,
            0,
            0,
            windowWidth,
            windowHeight,
            GL_COLOR_BUFFER_BIT,
            GL_NEAREST);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    }, false);
    physicalDevice->SwapBuffers(info.presentMode);
}
}