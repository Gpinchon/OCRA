#include <VK/CommandBuffer.hpp>

#include <OCRA/Command/Buffer.hpp>

namespace OCRA::Command::Buffer
{
//Begin Command Buffer recording, switching it to Recording state
void Begin(const Handle& a_CommandBuffer,
           const BeginInfo& a_BeginInfo)
{
    VkCommandBufferBeginInfo info{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    if (a_BeginInfo.inheritanceInfo.has_value()) {
        VkCommandBufferInheritanceInfo inheritanceInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO };
        //inheritanceInfo.framebuffer = *a_BeginInfo.inheritanceInfo->framebuffer;
        //inheritanceInfo.renderPass  = *a_BeginInfo.inheritanceInfo->renderPass;
        info.pInheritanceInfo = &inheritanceInfo;
    }
    vkBeginCommandBuffer(*a_CommandBuffer, &info);
}
//End Command Buffer recording, switching it to Executable state
void End(const Handle& a_CommandBuffer)
{
    vkEndCommandBuffer(*a_CommandBuffer);
}
//Reset Command Buffer to Initial state
void Reset(const Handle& a_CommandBuffer)
{
    vkResetCommandBuffer(*a_CommandBuffer, 0);
}
}
