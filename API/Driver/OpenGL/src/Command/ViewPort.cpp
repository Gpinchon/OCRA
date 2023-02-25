#include <OCRA/Core.hpp>

#include <GL/Command/Buffer.hpp>

#include <vector>

namespace OCRA::Command
{
void SetViewPort(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const uint32_t&                 a_FirstViewPort,
    const std::vector<ViewPort>&    a_ViewPorts)
{
    a_CommandBuffer->PushCommand<GenericCommand>([firstViewPort = a_FirstViewPort, viewports = a_ViewPorts](Buffer::ExecutionState& a_ExecutionState) {
        a_ExecutionState.dynamicStates.viewPorts.resize(viewports.size());
        for (auto index = firstViewPort; index < viewports.size(); ++index)
            a_ExecutionState.dynamicStates.viewPorts.at(index) = viewports.at(index);
    });
}
}