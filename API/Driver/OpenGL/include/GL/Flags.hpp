#pragma once

#include <OCRA/Flags.hpp>

#include <GL/glew.h>

namespace OCRA
{
static inline auto GetGLBarrierAccessBits(const AccessFlags& a_AccessFlags)
{
    GLbitfield barrier = 0;
    if ((a_AccessFlags & AccessFlagBits::ColorAttachmentRead) != 0)
        barrier |= GL_FRAMEBUFFER_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::ColorAttachmentWrite) != 0)
        barrier |= GL_FRAMEBUFFER_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::DepthStencilAttachmentRead) != 0)
        barrier |= GL_FRAMEBUFFER_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::DepthStencilAttachmentWrite) != 0)
        barrier |= GL_FRAMEBUFFER_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::HostRead) != 0)
        barrier |= GL_BUFFER_UPDATE_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::HostWrite) != 0)
        barrier |= GL_BUFFER_UPDATE_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::IndexRead) != 0)
        barrier |= GL_ELEMENT_ARRAY_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::IndirectCommandRead) != 0)
        barrier |= GL_COMMAND_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::InputAttachmentRead) != 0)
        barrier |= GL_SHADER_IMAGE_ACCESS_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::MemoryRead) != 0)
        barrier |= GL_BUFFER_UPDATE_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::MemoryWrite) != 0)
        barrier |= GL_BUFFER_UPDATE_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::ShaderRead) != 0)
        barrier |= GL_SHADER_STORAGE_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::ShaderWrite) != 0)
        barrier |= GL_SHADER_STORAGE_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::TransferRead) != 0)
        barrier |= GL_TEXTURE_UPDATE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::TransferWrite) != 0)
        barrier |= GL_TEXTURE_UPDATE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::UniformRead) != 0)
        barrier |= GL_UNIFORM_BARRIER_BIT;
    return barrier;
}
}