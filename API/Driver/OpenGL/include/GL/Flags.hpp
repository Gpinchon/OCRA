#pragma once

#include <OCRA/Flags.hpp>

#include <GL/glew.h>

namespace OCRA {
static inline auto GetGLBarrierAccessBits(const AccessFlags& a_AccessFlags)
{
    GLbitfield barrier = 0;
    barrier |= (a_AccessFlags & AccessFlagBits::ColorAttachmentRead) != 0 ? GL_FRAMEBUFFER_BARRIER_BIT : 0;
    barrier |= (a_AccessFlags & AccessFlagBits::ColorAttachmentWrite) != 0 ? GL_FRAMEBUFFER_BARRIER_BIT : 0;
    barrier |= (a_AccessFlags & AccessFlagBits::DepthStencilAttachmentRead) != 0 ? GL_FRAMEBUFFER_BARRIER_BIT : 0;
    barrier |= (a_AccessFlags & AccessFlagBits::DepthStencilAttachmentWrite) != 0 ? GL_FRAMEBUFFER_BARRIER_BIT : 0;
    barrier |= (a_AccessFlags & AccessFlagBits::HostRead) != 0 ? GL_BUFFER_UPDATE_BARRIER_BIT : 0;
    barrier |= (a_AccessFlags & AccessFlagBits::HostWrite) != 0 ? GL_BUFFER_UPDATE_BARRIER_BIT : 0;
    barrier |= (a_AccessFlags & AccessFlagBits::IndexRead) != 0 ? GL_ELEMENT_ARRAY_BARRIER_BIT : 0;
    barrier |= (a_AccessFlags & AccessFlagBits::IndirectCommandRead) != 0 ? GL_COMMAND_BARRIER_BIT : 0;
    barrier |= (a_AccessFlags & AccessFlagBits::InputAttachmentRead) != 0 ? GL_SHADER_IMAGE_ACCESS_BARRIER_BIT : 0;
    barrier |= (a_AccessFlags & AccessFlagBits::MemoryRead) != 0 ? GL_BUFFER_UPDATE_BARRIER_BIT : 0;
    barrier |= (a_AccessFlags & AccessFlagBits::MemoryWrite) != 0 ? GL_BUFFER_UPDATE_BARRIER_BIT : 0;
    barrier |= (a_AccessFlags & AccessFlagBits::ShaderRead) != 0 ? GL_SHADER_STORAGE_BARRIER_BIT : 0;
    barrier |= (a_AccessFlags & AccessFlagBits::ShaderWrite) != 0 ? GL_SHADER_STORAGE_BARRIER_BIT : 0;
    barrier |= (a_AccessFlags & AccessFlagBits::TransferRead) != 0 ? GL_TEXTURE_UPDATE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT : 0;
    barrier |= (a_AccessFlags & AccessFlagBits::TransferWrite) != 0 ? GL_TEXTURE_UPDATE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT : 0;
    barrier |= (a_AccessFlags & AccessFlagBits::UniformRead) != 0 ? GL_UNIFORM_BARRIER_BIT : 0;
    return barrier;
}
}
