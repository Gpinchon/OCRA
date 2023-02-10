#include <GL/Command/Buffer.hpp>
#include <GL/QueryPool.hpp>

OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);
OCRA_DECLARE_HANDLE(OCRA::QueryPool);

namespace OCRA::Command
{
void BeginQuery(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const QueryPool::Handle&        a_QueryPool,
    const uint32_t&                 a_QueryIndex)
{
    a_CommandBuffer->PushCommand<GenericCommand>([queryPool = a_QueryPool, queryIndex = a_QueryIndex](Buffer::ExecutionState& a_ExecutionState) {
        queryPool->Begin(queryIndex);
    });
}
void EndQuery(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const QueryPool::Handle&        a_QueryPool,
    const uint32_t&                 a_QueryIndex)
{
    a_CommandBuffer->PushCommand<GenericCommand>([queryPool = a_QueryPool, queryIndex = a_QueryIndex](Buffer::ExecutionState& a_ExecutionState) {
        queryPool->End(queryIndex);
    });
}
}