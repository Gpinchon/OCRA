#pragma once

#include <Handle.hpp>
#include <Descriptor/Type.hpp>
#include <Image/Layout.hpp>

#include <vector>
#include <optional>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::Set);
OCRA_DECLARE_HANDLE(OCRA::Buffer::View);

namespace OCRA::Descriptor::Set
{
struct BufferInfo
{
    Buffer::Handle  buffer;
    size_t          offset{ 0 };
    size_t          range{ 0 };
};
struct ImageInfo
{
    Sampler::Handle     sampler;
    Image::View::Handle imageView;
    Image::Layout       imageLayout{ Image::Layout::Unknown };
};
struct Write
{
    Set::Handle dstSet;
    uint32_t    dstBinding{ 0 };
    uint32_t    dstArrayElement{ 0 };
    uint32_t    dstCount{ 0 };
    Type        type{ Type::Unknown };
    std::optional<std::vector<std::byte>>   inlineUniformBlock;
    std::optional<BufferInfo>               bufferInfo;
    std::optional<ImageInfo>                imageInfo;
    std::optional<Buffer::View::Handle>     texelBufferView;
};
struct Copy
{
    Set::Handle dstSet;
    uint32_t    dstBinding{ 0 };
    uint32_t    dstArrayElement{ 0 };
    uint32_t    count{ 0 };
    Set::Handle srcSet;
    uint32_t    srcBinding{ 0 };
    uint32_t    srcArrayElement{ 0 };
};
void Update(
    const Device::Handle&       a_Device,
    const std::vector<Write>    a_Writes,
    const std::vector<Copy>     a_Copies);
}

#include <Pipeline/Pipeline.hpp>

OCRA_DECLARE_HANDLE(OCRA::Pipeline::Layout);

namespace OCRA::Command
{
void BindDescriptorSets(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const Pipeline::BindingPoint&   a_BindingPoint,
    const Pipeline::Layout::Handle& a_PipelineLayout,
    const uint32_t&                 a_firstSet,
    const std::vector<Descriptor::Set::Handle>  a_DescriptorSets,
    const std::vector<uint32_t>                 a_DynamicOffsets);
}