#include <OCRA/Structs.hpp>

#include <VK/CommandBuffer.hpp>
#include <VK/Flags.hpp>

namespace OCRA::Command::Buffer
{
void Begin(const Handle& a_CommandBuffer,
           const CommandBufferBeginInfo& a_BeginInfo)
{
    vk::CommandBufferBeginInfo info;
    info.flags = ConvertToVk(a_BeginInfo.flags);
    //if (a_BeginInfo.inheritanceInfo.has_value()) {
    //    vk::CommandBufferInheritanceInfo inheritanceInfo;
    //    //inheritanceInfo.framebuffer = *a_BeginInfo.inheritanceInfo->framebuffer;
    //    //inheritanceInfo.renderPass  = *a_BeginInfo.inheritanceInfo->renderPass;
    //    info.pInheritanceInfo = &inheritanceInfo;
    //}
    a_CommandBuffer->begin(info);
}

void End(const Handle& a_CommandBuffer)
{
    a_CommandBuffer->end();
}

void Reset(const Handle& a_CommandBuffer)
{
    a_CommandBuffer->reset();
}
}
