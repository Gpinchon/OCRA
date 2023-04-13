////////////////////////////////////////////////////////////////////////////////
//  Command buffer commands
////////////////////////////////////////////////////////////////////////////////

#include <GL/Common/Assert.hpp>
#include <GL/CommandBuffer.hpp>

namespace OCRA::Command
{
struct ExecuteCommandBufferCommand : CommandI {
    ExecuteCommandBufferCommand(const Command::Buffer::Handle& a_CommandBuffer)
        : commandBuffer(a_CommandBuffer)
    {}
    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) {
        commandBuffer->ExecuteSecondary(a_ExecutionState);
    }
    Command::Buffer::Handle commandBuffer;
};

struct PushConstantCommand : CommandI {
    PushConstantCommand(
        OCRA::PushConstants& a_PushConstants,
        const Pipeline::Handle& a_Pipeline,
        const size_t& a_Offset,
        const size_t& a_Size,
        const std::byte* a_Data)
        : pushConstants(a_PushConstants)
        , pipeline(a_Pipeline)
        , offset(a_Offset)
        , size(a_Size)
    {
        OCRA_ASSERT(size < 256);
        std::copy(a_Data, a_Data + size, data.data());
    }
    virtual void operator()(Buffer::ExecutionState&) {
        pushConstants.Update(offset, size, data.data());
    }
    OCRA::PushConstants& pushConstants;
    const Pipeline::Handle pipeline;
    const size_t offset;
    const size_t size;
    std::array<std::byte, 256> data;
};

void ExecuteCommandBuffer(
    const Buffer::Handle& a_CommandBuffer,
    const Buffer::Handle& a_SecondaryCommandBuffer)
{
    a_CommandBuffer->PushCommand<ExecuteCommandBufferCommand>(a_SecondaryCommandBuffer);
}
void PushConstants(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Pipeline::Handle& a_Pipeline,
    const uint8_t a_Offset,
    const std::vector<std::byte>& a_Data)
{
    a_CommandBuffer->PushCommand<PushConstantCommand>(
        a_CommandBuffer->pushConstants,
        a_Pipeline,
        a_Offset,
        a_Data.size(),
        a_Data.data());
}
}