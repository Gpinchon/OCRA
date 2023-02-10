#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Common/SharingMode.hpp>

#include <bitset>
#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Buffer);
OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Memory);

namespace OCRA {
struct AllocationCallback;
}

namespace OCRA::Buffer {
using CreateFlags = std::bitset<5>;
namespace CreateFlagBits {
static CreateFlags None                       = 0b00000;
static CreateFlags SparseBinding              = 0b10000;
static CreateFlags SparseResidency            = 0b01000;
static CreateFlags SparseAliased              = 0b00100;
static CreateFlags Protected                  = 0b00010;// VK_VERSION_1_1
static CreateFlags DeviceAddressCaptureReplay = 0b00001;// VK_VERSION_1_2
}
using UsageFlags = std::bitset<10>;
namespace UsageFlagBits {
static UsageFlags None                 = 0b0000000000;
static UsageFlags TransferSrc          = 0b1000000000;
static UsageFlags TransferDst          = 0b0100000000;
static UsageFlags UniformTexelBuffer   = 0b0010000000;
static UsageFlags StorageTexelBuffer   = 0b0001000000;
static UsageFlags UniformBuffer        = 0b0000100000;
static UsageFlags StorageBuffer        = 0b0000010000;
static UsageFlags IndexBuffer          = 0b0000001000;
static UsageFlags VertexBuffer         = 0b0000000100;
static UsageFlags IndirectBuffer       = 0b0000000010;
static UsageFlags ShaderDeviceAddress  = 0b0000000001;// Provided by VK_VERSION_1_2
}
struct Info {
    CreateFlags             flags{ CreateFlagBits::None };;
    uint64_t                size{ 0 };
    UsageFlags              usage{ UsageFlagBits::None };
    SharingMode             sharingMode{ SharingMode::Exclusive };
    std::vector<uint32_t>   queueFamilyIndices;
};

/**
* @brief Creates an empty buffer, unuseable without binding Memory to it
*/
Handle Create(
    const Device::Handle&       a_Device,
    const Info&                 a_Info,
    const AllocationCallback*   a_Allocator = nullptr);

/**
* @brief Binds the specified memory to the specified Buffer.
* On OpenGL, this will update internal structure to use a certain range of the GPU buffer.
* This comes with limitations, especially regarding indices and instanciated draw calls.
*/
void BindMemory(
    const Device::Handle&   a_Device,
    const Handle&           a_Buffer,
    const Memory::Handle&   a_Memory,
    const size_t&           a_MemoryOffset
);
}

#include <OCRA/Command/Buffer.hpp>
#include <OCRA/Common/IndexType.hpp>

namespace OCRA::Command {
struct BufferCopyRegion
{
    uint64_t readOffset{ 0 }, writeOffset{ 0 }, size{ 0 };
};

/**
* Update a buffer's contents from host memory
* Size is limited to 65536
*/
void UpdateBuffer(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const OCRA::Buffer::Handle&     a_DstBuffer,
    const size_t&                   a_Offset,
    const size_t&                   a_Size,
    const std::byte*                a_Data);

/**
* Update a buffer's contents from host memory
* Size is limited to 65536
*/
void UpdateBuffer(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const OCRA::Buffer::Handle&     a_DstBuffer,
    const size_t&                   a_Offset,
    const std::vector<std::byte>&   a_Data);

/**
* @brief Copies data from srcBuffer to dstBuffer
*/
void CopyBuffer(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Handle&    a_SrcBuffer,
    const OCRA::Buffer::Handle&    a_DstBuffer,
    const std::vector<BufferCopyRegion>&  a_Regions);

/**
* @brief Bind specified Vertex Buffers to Render Pass currently bound to this Command Buffer
*/
void BindVertexBuffers(
    const Command::Buffer::Handle&              a_CommandBuffer,
    const uint32_t                              a_FirstBinding,
    const std::vector<OCRA::Buffer::Handle>&    a_VertexBuffers,
    const std::vector<uint64_t>&                a_Offsets);

/**
* @brief Bind specified Vertex Buffer to Render Pass currently bound to this Command Buffer
*/
void BindIndexBuffer(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const OCRA::Buffer::Handle&     a_IndexBuffer,
    const uint64_t                  a_Offset,
    const IndexType                 a_IndexType);
}
