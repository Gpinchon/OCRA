/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Common/Rect2D.hpp>
#include <Handle.hpp>
#include <Pipeline/Pipeline.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::RenderPass);
OCRA_DECLARE_HANDLE(OCRA::Device);

namespace OCRA::RenderPass
{
enum class LoadOperation {
    DontCare, //Nothing is done
    Load,     //Values are kept
    Clear,    //Values are cleared
    MaxValue
};
enum class StoreOperation {
    DontCare, //Nothing is stored
    Store,    //Values from the Fragment Shader are written
    MaxValue
};

struct AttachmentDescription {
    LoadOperation  loadOp{ LoadOperation::DontCare };   //determines what to do with the buffer before rendering
    StoreOperation storeOp{ StoreOperation::DontCare }; //determines what to do with the buffer after rendering
};

struct AttachmentReference {
    int8_t  location{ -1 }; //location of the output, -1 means none
};

struct SubPassDescription {
    Pipeline::BindingPoint           pipelineBindingPoint{ Pipeline::BindingPoint::Unknown };
    std::vector<AttachmentReference> colorAttachments{};
    AttachmentReference              depthAttachment{};
    AttachmentReference              stencilAttachment{};
};

/**
 * Specifies the outputs of the RenderPass and clear operations
 */
struct Info {
    std::vector<SubPassDescription>    subPasses;
    std::vector<AttachmentDescription> colorAttachments;
    AttachmentDescription              depthAttachment;
    AttachmentDescription              stencilAttachment;
};

Handle Create(const Device::Handle& a_Device, const Info& a_Info);
}

#include <Common/ClearValue.hpp>

OCRA_DECLARE_HANDLE(OCRA::FrameBuffer);

namespace OCRA::Command
{
/**
 * renderArea        : defines the area that this RenderPass is gonna render to
 * colorClearValues  : defines the values to be used to clear color buffers, must be the same size as colorAttachments member of RenderPass
 * depthClearValue   : defines the values to be used to clear the depth buffer
 * stencilClearValue : defines the values to be used to clear the stencil buffer
 */
struct RenderPassBeginInfo
{
    RenderPass::Handle      renderPass{ 0 };
    FrameBuffer::Handle     framebuffer{ 0 };
    Rect2D                  renderArea{ 0, 0, 0, 0 };
    std::vector<ColorValue> colorClearValues;
    float                   depthClearValue{ 0 };
    int32_t                 stencilClearValue{ 0 };
};
enum class SubPassContents
{
    Inline, SecondaryCommandBuffers
};
//Begin Render Pass recording
void BeginRenderPass(const Command::Buffer::Handle& a_CommandBuffer, const RenderPassBeginInfo& a_BeginInfo, const SubPassContents& a_SubPassContents);
void NextSubPass(const Command::Buffer::Handle& a_CommandBuffer, const SubPassContents& a_SubPassContents);
//End Render Pass recording
void EndRenderPass(const Command::Buffer::Handle& a_CommandBuffer);
}