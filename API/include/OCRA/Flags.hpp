#pragma once

#include <bitset>

namespace OCRA
{
using BufferUsageFlags = std::bitset<10>;
namespace BufferUsageFlagBits {
    static BufferUsageFlags None                 = 0b0000000000;
    static BufferUsageFlags TransferSrc          = 0b1000000000;
    static BufferUsageFlags TransferDst          = 0b0100000000;
    static BufferUsageFlags UniformTexelBuffer   = 0b0010000000;
    static BufferUsageFlags StorageTexelBuffer   = 0b0001000000;
    static BufferUsageFlags UniformBuffer        = 0b0000100000;
    static BufferUsageFlags StorageBuffer        = 0b0000010000;
    static BufferUsageFlags IndexBuffer          = 0b0000001000;
    static BufferUsageFlags VertexBuffer         = 0b0000000100;
    static BufferUsageFlags IndirectBuffer       = 0b0000000010;
    static BufferUsageFlags ShaderDeviceAddress  = 0b0000000001;// Provided by VK_VERSION_1_2
}
using BlendColorMaskFlags = std::bitset<4>;
namespace BlendColorMaskFlagBits {
    constexpr BlendColorMaskFlags R = 0b1000;
    constexpr BlendColorMaskFlags G = 0b0100;
    constexpr BlendColorMaskFlags B = 0b0010;
    constexpr BlendColorMaskFlags A = 0b0001;
    constexpr BlendColorMaskFlags RGBA = 0b1111;
    constexpr BlendColorMaskFlags None = 0b0000;
}
using CommandBufferUsageFlags = std::bitset<3>;
namespace CommandBufferUsageFlagBits {
    static CommandBufferUsageFlags None = 0b000;
    static CommandBufferUsageFlags OneTimeSubmit = 0b100;
    static CommandBufferUsageFlags RenderPassContinue = 0b010;
    static CommandBufferUsageFlags SimultaneousUse = 0b001;
};
using CreateBufferFlags = std::bitset<5>;
namespace CreateBufferFlagBits {
    static CreateBufferFlags None                       = 0b00000;
    static CreateBufferFlags SparseBinding              = 0b10000;
    static CreateBufferFlags SparseResidency            = 0b01000;
    static CreateBufferFlags SparseAliased              = 0b00100;
    static CreateBufferFlags Protected                  = 0b00010;// VK_VERSION_1_1
    static CreateBufferFlags DeviceAddressCaptureReplay = 0b00001;// VK_VERSION_1_2
}
using CreateCommandPoolFlags = std::bitset<3>;
namespace CreateCommandPoolFlagBits {
    static CreateCommandPoolFlags None      = 0b000;
    static CreateCommandPoolFlags Transient = 0b100;
    static CreateCommandPoolFlags Reset     = 0b010; //enables command buffers to be reset to their initial state
    static CreateCommandPoolFlags Protected = 0b001;
}
using ImageUsageFlags = std::bitset<8>;
namespace ImageUsageFlagBits {
    constexpr ImageUsageFlags TransferSrc             = 0b10000000;
    constexpr ImageUsageFlags TransferDst             = 0b01000000;
    constexpr ImageUsageFlags Sampled                 = 0b00100000;
    constexpr ImageUsageFlags Storage                 = 0b00010000;
    constexpr ImageUsageFlags ColorAttachment         = 0b00001000;
    constexpr ImageUsageFlags DepthStencilAttachment  = 0b00000100;
    constexpr ImageUsageFlags TransientAttachment     = 0b00000010;
    constexpr ImageUsageFlags InputAttachment         = 0b00000001;
    //VK_VERSION_1_3
    constexpr ImageUsageFlags None                    = 0b00000000;
}
using MemoryHeapFlags = std::bitset<2>;
namespace MemoryHeapFlagBits {
    static MemoryHeapFlags None             = 0b00;
    static MemoryHeapFlags DeviceLocal      = 0b10;
    static MemoryHeapFlags MultiInstance    = 0b01; //allows for memory mapping
};
using MemoryPropertyFlags = std::bitset<6>;
namespace MemoryPropertyFlagBits {
    static MemoryPropertyFlags None             = 0b000000;
    static MemoryPropertyFlags DeviceLocal      = 0b100000;
    static MemoryPropertyFlags HostVisible      = 0b010000; //allows for memory mapping
    static MemoryPropertyFlags HostCoherent     = 0b001000; //flush and invalidate are not necesssary for sync with device
    static MemoryPropertyFlags HostCached       = 0b000100; //is backed by host's buffer, faster access
    static MemoryPropertyFlags LazilyAllocated  = 0b000010; //only allows access from device, do not use with HostVisible
    static MemoryPropertyFlags Protected        = 0b000001; //only allows access from device, allows protected queue operations, do not use with HostVisible, HostCoherent or HostCached
};
using PipelineStageFlags = std::bitset<32>;
namespace PipelineStageFlagBits {
    constexpr PipelineStageFlags TopOfPipe                    = 0b00000000000000000000000000000001;
    constexpr PipelineStageFlags DrawIndirect                 = 0b00000000000000000000000000000010;
    constexpr PipelineStageFlags VertexInput                  = 0b00000000000000000000000000000100;
    constexpr PipelineStageFlags VertexShader                 = 0b00000000000000000000000000001000;
    constexpr PipelineStageFlags TessellationControlShader    = 0b00000000000000000000000000010000;
    constexpr PipelineStageFlags TessellationEvaluationShader = 0b00000000000000000000000000100000;
    constexpr PipelineStageFlags GeometryShader               = 0b00000000000000000000000001000000;
    constexpr PipelineStageFlags FragmentShader               = 0b00000000000000000000000010000000;
    constexpr PipelineStageFlags EarlyFragmentTests           = 0b00000000000000000000000100000000;
    constexpr PipelineStageFlags LateFragmentTests            = 0b00000000000000000000001000000000;
    constexpr PipelineStageFlags ColorAttachmentOutput        = 0b00000000000000000000010000000000;
    constexpr PipelineStageFlags ComputeShader                = 0b00000000000000000000100000000000;
    constexpr PipelineStageFlags Transfer                     = 0b00000000000000000001000000000000;
    constexpr PipelineStageFlags BottomOfPipe                 = 0b00000000000000000010000000000000;
    constexpr PipelineStageFlags Host                         = 0b00000000000000000100000000000000;
    constexpr PipelineStageFlags AllGraphics                  = 0b00000000000000001000000000000000;
    constexpr PipelineStageFlags AllCommands                  = 0b00000000000000010000000000000000;
    //VK 1.3
    constexpr PipelineStageFlags None                         = 0b00000000000000000000000000000000;
}
using QueryPipelineStatisticFlags = std::bitset<11>;
namespace QueryPipelineStatisticFlagBits {
    static QueryPipelineStatisticFlags None = 0b00000000000;
    static QueryPipelineStatisticFlags InputAssemblyVertices = 0b10000000000;
    static QueryPipelineStatisticFlags InputAssemblyPrimitives = 0b01000000000;
    static QueryPipelineStatisticFlags VertexShaderInvocations = 0b00100000000;
    static QueryPipelineStatisticFlags GeometryShaderInvocations = 0b00010000000;
    static QueryPipelineStatisticFlags GeometryShaderPrimitives = 0b00001000000;
    static QueryPipelineStatisticFlags ClippingInvocations = 0b00000100000;
    static QueryPipelineStatisticFlags ClippingPrimitives = 0b00000010000;
    static QueryPipelineStatisticFlags FragmentShaderInvocations = 0b00000001000;
    static QueryPipelineStatisticFlags TessellationControlShaderPatches = 0b00000000100;
    static QueryPipelineStatisticFlags TessellationEvaluationShaderInvocations = 0b00000000010;
    static QueryPipelineStatisticFlags ComputeShaderInvocations = 0b00000000001;
}
using QueueFlags = std::bitset<5>;
namespace QueueFlagsBits {
    static QueueFlags None          = 0b00000;
    static QueueFlags Graphics      = 0b10000;
    static QueueFlags Compute       = 0b01000;
    static QueueFlags Transfer      = 0b00100;
    static QueueFlags SparseBinding = 0b00010;
    static QueueFlags Protected     = 0b00001;// VK_VERSION_1_1
}
using QueryResultFlags = std::bitset<4>;
namespace QueryResultFlagBits {
    static QueryResultFlags None = 0b0000;
    static QueryResultFlags Result64Bits = 0b1000; //result stored in 64 bits
    static QueryResultFlags Wait = 0b0100; //wait for the query result to be available
    static QueryResultFlags WithAvailability = 0b0010; //the availability status will accompany the results
    static QueryResultFlags Partial = 0b0001; //specifies that partial result is acceptable
}
using ShaderStageFlags = std::bitset<6>;
namespace ShaderStageFlagBits {
    constexpr ShaderStageFlags None = 0b000000;
    constexpr ShaderStageFlags Vertex = 0b100000;
    constexpr ShaderStageFlags Geometry = 0b010000;
    constexpr ShaderStageFlags Fragment = 0b001000;
    constexpr ShaderStageFlags Compute = 0b000100;
    constexpr ShaderStageFlags TessControl = 0b000010;
    constexpr ShaderStageFlags TessEval = 0b000001;
    constexpr ShaderStageFlags AllGraphics = 0b111000; //Vertex | Geometry | Fragment
    constexpr ShaderStageFlags All = 0b111111;
}
}