#include <OCRA/Structs.hpp>

#include <VK/CommandBuffer.hpp>
#include <VK/Enums.hpp>
#include <VK/Flags.hpp>

namespace OCRA::Command::Buffer {
void Begin(const Handle& a_CommandBuffer,
    const CommandBufferBeginInfo& a_BeginInfo)
{
    vk::CommandBufferBeginInfo info;
    info.flags = ConvertToVk(a_BeginInfo.flags);
    vk::CommandBufferInheritanceInfo vkInheritanceInfo;
    vk::CommandBufferInheritanceRenderingInfo inheritanceRenderingInfo;
    std::vector<vk::Format> colorAttachmentFormats;
    if (a_BeginInfo.inheritanceInfo.has_value()) {
        info.setPInheritanceInfo(&vkInheritanceInfo);
        vkInheritanceInfo.setPNext(&inheritanceRenderingInfo);
        const auto& inheritanceInfo = a_BeginInfo.inheritanceInfo.value();
        for (const auto& format : inheritanceInfo.colorAttachmentFormats)
            colorAttachmentFormats.push_back(ConvertToVk(format));
        inheritanceRenderingInfo.setColorAttachmentFormats(colorAttachmentFormats);
        inheritanceRenderingInfo.setDepthAttachmentFormat(ConvertToVk(inheritanceInfo.depthAttachmentFormat));
        inheritanceRenderingInfo.setStencilAttachmentFormat(ConvertToVk(inheritanceInfo.stencilAttachmentFormat));
    }
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
