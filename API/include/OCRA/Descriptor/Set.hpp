#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Descriptor/Type.hpp>
#include <OCRA/Image/Layout.hpp>

#include <vector>
#include <optional>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::Set);
OCRA_DECLARE_HANDLE(OCRA::Buffer);
OCRA_DECLARE_HANDLE(OCRA::Image::Sampler);
OCRA_DECLARE_HANDLE(OCRA::Image::View);

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
    Image::Sampler::Handle sampler;
    Image::View::Handle    imageView;
    Image::Layout          imageLayout{ Image::Layout::Unknown };
};
struct WriteOperation
{
    Set::Handle dstSet;
    uint32_t    dstBinding{ 0 };
    uint32_t    dstArrayElement{ 0 };
    uint32_t    dstCount{ 0 };
    Type        type{ Type::Unknown };
    std::optional<std::vector<uint8_t>> inlineUniformBlock;
    std::optional<BufferInfo>           bufferInfo;
    std::optional<ImageInfo>            imageInfo;
    std::optional<Buffer::Handle>       texelBuffer;
};
struct CopyOperation
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
    const Device::Handle&               a_Device,
    const std::vector<WriteOperation>&  a_Writes,
    const std::vector<CopyOperation>&   a_Copies);
}

#include <OCRA/Pipeline/Pipeline.hpp>

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
void PushDescriptorSet(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const Pipeline::BindingPoint&   a_PipelineBindPoint,
    const Pipeline::Layout::Handle& a_Layout,
    const uint32_t&                 a_Set,
    const std::vector<Descriptor::Set::WriteOperation>& a_Writes);
}