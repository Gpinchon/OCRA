#pragma once

#include <RenderPass.hpp>
#include <GL/WeakHandle.hpp>

#include <vector>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Command::Buffer
{
struct ExecutionState;
}

namespace OCRA::RenderPass
{
struct ClearBufferOp {
    uint32_t   buffer; //hidden GLenum
    int32_t    drawBuffer;
};
struct Impl {
    Impl(const Device::Handle& a_Device, const Info& a_Info);
    void BeginRenderPass(Command::Buffer::ExecutionState& a_ExecutionState);
    Device::WeakHandle device;
    const Info info;
    const std::vector<ClearBufferOp> clearOps;
    const std::vector<uint32_t>      drawBuffers; //hidden GLenum
};
}