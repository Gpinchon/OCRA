#include <OCRA/Core.hpp>

#include <GL/Command/Buffer.hpp>

#include <vector>

namespace OCRA::Command
{
void SetScissor(
    const Command::Buffer::Handle& a_CommandBuffer,
    const uint32_t&                a_FirstScissor,
    const std::vector<Rect2D>&     a_Scissors)
{
    a_CommandBuffer->PushCommand<GenericCommand>([firstScissor = a_FirstScissor, scissors = a_Scissors](Buffer::ExecutionState& a_ExecutionState) {
        a_ExecutionState.dynamicStates.scissors.resize(scissors.size());
        for (auto index = firstScissor; index < scissors.size(); ++index)
            a_ExecutionState.dynamicStates.scissors.at(index) = scissors.at(index);
    });
}
}