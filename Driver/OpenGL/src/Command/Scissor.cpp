#include <Common/Rect2D.hpp>

#include <GL/Command/Buffer.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);

namespace OCRA::Command
{
void SetScissor(
    const Command::Buffer::Handle& a_CommandBuffer,
    const uint32_t&                a_FirstScissor,
    const std::vector<Rect2D>&     a_Scissors)
{
    a_CommandBuffer->PushCommand([firstScissor = a_FirstScissor, scissors = a_Scissors](Buffer::ExecutionState& a_ExecutionState) {
        a_ExecutionState.dynamicStates.scissors.resize(scissors.size());
        for (auto index = firstScissor; index < scissors.size(); ++index)
            a_ExecutionState.dynamicStates.scissors.at(index) = scissors.at(index);
    });
}
}