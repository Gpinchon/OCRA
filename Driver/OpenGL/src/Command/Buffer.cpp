/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-11-01 17:31:24
*/

/////TODO : REDO THIS SHIT !!!

#include <Command/RenderPass.hpp>
#include <FrameBuffer.hpp>
#include <Handle.hpp>
#include <RenderPass.hpp>

#include <GL/Buffer/Buffer.hpp>
#include <GL/Buffer/Vertex.hpp>
#include <GL/Command/RenderPass.hpp>
#include <GL/IndexType.hpp>

#include <array>
#include <cassert>
#include <functional>
#include <map>
#include <mutex>

namespace OCRA::Command::Buffer {
Pipeline::Handle GetCurrentPipeline(const Pipeline::BindingPoint& a_BindingPoint);
struct Cmd {
    enum class CmdType {
        Unknown,
        BindPipeline,
        BindVertexBuffer,
        BindIndexBuffer,
        SetViewPort,
        MaxValue
    } type { CmdType::Unknown };
    union {
        struct Common {
        };
        struct BindPipeline {
        };
        struct BindVertexBuffer {
            VertexBufferBinding vertexBufferBinding;
        };
        struct BindIndexBuffer {
            IndexBufferBinding indexBufferBinding;
        };
    };
};
struct Impl;
typedef std::function<void(Impl&)> CallBack;
struct Impl {
    Impl(const Device::Handle& a_Device)
        : device(a_Device)
    {
    }
    void Reset()
    {
        assert(!recording);
        compiled = false;
        commands.clear();
    }
    void StartRecording()
    {
        Reset();
        recording = true;
    }
    void PushCommand(const CallBack& a_Command)
    {
        commands.push_back(a_Command);
    }
    void EndRecording()
    {
        recording = false;
    }
    void Compile()
    {
        for (const auto& command : compilationCommands)
            command(*this);
        compiled = true;
        compilationCommands.clear();
    }
    void Execute()
    {
        assert(!recording);
        if (!compiled)
            Compile();
        for (const auto& command : commands)
            command(*this);
    }
    std::vector<CallBack> commands;
    std::vector<CallBack> compilationCommands;
    RenderPass renderPass {};
    bool compiled { false };
    bool recording { false };
    Device::Handle device;
};
static Handle s_CurrentHandle = 0;
static std::map<Handle, Impl> s_Buffers;
Handle Create(const Device::Handle& a_Device)
{
    ++s_CurrentHandle;
    s_Buffers.emplace(s_CurrentHandle, Impl(a_Device));
    return s_CurrentHandle;
}
auto& Get(const Handle& a_CommandBuffer)
{
    return s_Buffers.at(a_CommandBuffer);
}
void Destroy(const Device::Handle& a_Device, const Handle& a_CommandBuffer)
{
    s_Buffers.erase(a_CommandBuffer);
}
void Begin(const Handle& a_CommandBuffer)
{
    Get(a_CommandBuffer).StartRecording();
}
void PushCommand(const Handle& a_CommandBuffer, const CallBack& a_Command)
{
    Get(a_CommandBuffer).PushCommand(a_Command);
}
void End(const Handle& a_CommandBuffer)
{
    Get(a_CommandBuffer).EndRecording();
}
}

namespace OCRA::Command {
//Begin Render Pass recording
void BeginRenderPass(const Command::Buffer::Handle& a_CommandBuffer, const RenderPassBeginInfo& a_BeginInfo, const SubPassContents& a_SubPassContents)
{
    Buffer::Get(a_CommandBuffer).PushCommand([beginInfo = a_BeginInfo](Command::Buffer::Impl& commandBuffer) {
        commandBuffer.renderPass.beginInfo = beginInfo;
    });
}
//End Render Pass recording
void EndRenderPass(const Command::Buffer::Handle& a_CommandBuffer)
{
    Buffer::Get(a_CommandBuffer).PushCommand([](Command::Buffer::Impl& commandBuffer) {
        commandBuffer.renderPass = {};
    });
}
void BindPipeline(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Pipeline::BindingPoint& a_BindingPoint,
    const Pipeline::Handle& a_Pipeline)
{
    auto commandBuffer{ Buffer::Get(a_CommandBuffer) };
    commandBuffer.PushCommand([&commandBuffer, bindingPoint = a_BindingPoint, pipeline = a_Pipeline](Command::Buffer::Impl& commandBuffer) {
        commandBuffer.renderPass.pipelines.at(size_t(bindingPoint)) = pipeline;
        if (bindingPoint == Pipeline::BindingPoint::Graphics)
            Pipeline::Graphics::ApplyGraphicsStates(commandBuffer.device, pipeline);
    });
}
//Bind specified Vertex Buffer to Render Pass currently bound to this Command Buffer
void BindIndexBuffer(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Vertex::Handle& a_IndexBuffer,
    const Uint64 a_Offset,
    const IndexType a_IndexType)
{
    Buffer::Get(a_CommandBuffer).PushCommand([indexBuffer = a_IndexBuffer, offset = a_Offset, indexType = a_IndexType](Command::Buffer::Impl& commandBuffer) {
        const auto& bufferHandle { OCRA::Buffer::Vertex::GetBufferHandle(indexBuffer) };
        commandBuffer.renderPass.indexBuffer.buffer = OCRA::Buffer::GetGLHandle(bufferHandle);
        commandBuffer.renderPass.indexBuffer.offset = offset;
        commandBuffer.renderPass.indexBuffer.indexType = GetGLIndexType(indexType);
    });
}
//Bind specified Vertex Buffers to Render Pass currently bound to this Command Buffer
void BindVertexBuffers(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Uint32 a_FirstBinding,
    const Uint32 a_BindingCount,
    const std::vector<OCRA::Buffer::Vertex::Handle>& a_VertexBuffers,
    const std::vector<Uint64>& a_Offsets)
{
    auto& commandBuffer { Buffer::Get(a_CommandBuffer) };
    VertexBufferBinding vertexBinding;
    vertexBinding.firstBinding = a_FirstBinding;
    vertexBinding.bindingCount = a_BindingCount;
    for (auto index = 0u; index < a_BindingCount; ++index) {
        const auto& bufferHandle{ OCRA::Buffer::Vertex::GetBufferHandle(a_VertexBuffers.at(index)) };
        vertexBinding.vertexBuffers.at(index) = OCRA::Buffer::GetGLHandle(bufferHandle);
        vertexBinding.offsets.at(index) = a_Offsets.at(index);
    }
    commandBuffer.commands.push_back([vertexBinding = vertexBinding](Command::Buffer::Impl& commandBuffer){
        const auto graphicsPipeline{ commandBuffer.renderPass.pipelines.at(size_t(Pipeline::BindingPoint::Graphics)) };
        if (graphicsPipeline == 0) return;
        const auto& graphicsPipelineInfo{ Pipeline::Graphics::GetInfo(graphicsPipeline) };
        const auto& bindingDescriptions{ graphicsPipelineInfo.vertexInputState.bindingDescriptions };
        for (auto i = 0u; i < vertexBinding.bindingCount; ++i)
        {
            const auto bindingIndex{ i + vertexBinding.firstBinding };
            const auto& binding{ bindingDescriptions.at(bindingIndex) };
            const auto& buffer{ vertexBinding.vertexBuffers.at(i) };
            const auto& offset{ vertexBinding.offsets.at(i) };
            glBindVertexBuffer(
                binding.binding,
                buffer,
                offset,
                binding.stride);
        }
    });
}
}

#include <GL/Buffer/Transfer.hpp>
#include <GL/Buffer/Buffer.hpp>

namespace OCRA::Command::VertexBuffer {
void ReadFrom(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Vertex::Handle& a_DstVBO,
    const OCRA::Buffer::Transfer::Handle& a_SrcTransferBuffer,
    Uint64 a_ReadOffset, Uint64 a_WriteOffset, Uint64 a_Size)
{
    OCRA::Buffer::CopyOperation copy;
    copy.srcBuffer = OCRA::Buffer::Transfer::GetBufferHandle(a_SrcTransferBuffer);
    copy.dstBuffer = OCRA::Buffer::Vertex::GetBufferHandle(a_DstVBO);
    copy.readOffset = a_ReadOffset;
    copy.writeOffset = a_WriteOffset;
    copy.size = a_WriteOffset;
    Buffer::Get(a_CommandBuffer).PushCommand([copy = copy](Command::Buffer::Impl& commandBuffer) {
        OCRA::Buffer::Copy(commandBuffer.device, copy);
    });
}
void WriteTo(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Vertex::Handle& a_SrcVBO,
    const OCRA::Buffer::Transfer::Handle& a_DstTransferBuffer,
    Uint64 a_ReadOffset, Uint64 a_WriteOffset, Uint64 a_Size)
{
    OCRA::Buffer::CopyOperation copy;
    copy.srcBuffer = OCRA::Buffer::Vertex::GetBufferHandle(a_SrcVBO);
    copy.dstBuffer = OCRA::Buffer::Transfer::GetBufferHandle(a_DstTransferBuffer);
    copy.readOffset = a_ReadOffset;
    copy.writeOffset = a_WriteOffset;
    copy.size = a_WriteOffset;
    Buffer::Get(a_CommandBuffer).PushCommand([copy = copy](Command::Buffer::Impl& commandBuffer) {
        OCRA::Buffer::Copy(commandBuffer.device, copy);
    });
}
}

