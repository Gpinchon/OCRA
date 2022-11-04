#pragma once

#include <RenderPass.hpp>
#include <WeakHandle.hpp>

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
struct SubPass {
	void Begin() const;
	const std::vector<uint32_t>      drawBuffers; //hidden GLenum
};
struct Impl {
    Impl(const Device::Handle& a_Device, const Info& a_Info);
    void BeginRenderPass(const Command::Buffer::ExecutionState& a_ExecutionState) const;
	void BeginSubPass(const Command::Buffer::ExecutionState& a_ExecutionState) const;
	void ApplyClearOps(const Command::Buffer::ExecutionState& a_ExecutionState) const;
    Device::WeakHandle device;
    const Info info;
	const std::vector<SubPass>		 subPasses;
    const std::vector<ClearBufferOp> clearOps;
};
}