#include <VK/Buffer.hpp>
#include <VK/DescriptorSet.hpp>
#include <VK/Device.hpp>
#include <VK/Enums.hpp>
#include <VK/ImageSampler.hpp>
#include <VK/ImageView.hpp>

#include <OCRA/Structs.hpp>

namespace OCRA::Descriptor::Set
{
void Update(
    const std::vector<DescriptorSetWrite>&  a_Writes,
    const std::vector<DescriptorSetCopy>&   a_Copies)
{
    vk::Device device;
    if (!a_Writes.empty()) device = a_Writes.front().dstSet->getDevice();
    else if (!a_Copies.empty()) device = a_Copies.front().dstSet->getDevice();
    std::vector<vk::WriteDescriptorSet>   vkWrites(a_Writes.size());
    std::vector<vk::DescriptorBufferInfo> vkWriteBufferInfo(a_Writes.size());
    std::vector<vk::DescriptorImageInfo>  vkWriteImageInfo(a_Writes.size());
    std::vector<vk::BufferView>           vkWriteBufferView(a_Writes.size());

    std::vector<vk::CopyDescriptorSet>    vkCopies(a_Copies.size());

    for (auto i = 0u; i < a_Writes.size(); ++i) {
        auto& write   = a_Writes.at(i);
        auto& vkWrite = vkWrites.at(i);
        vkWrite.dstSet     = **write.dstSet;
        vkWrite.dstBinding = write.dstBinding;
        vkWrite.dstArrayElement = write.dstArrayElement;
        vkWrite.descriptorCount = write.dstCount;
        vkWrite.descriptorType  = GetVkDescriptorType(write.type);
        if (write.bufferInfo.has_value()) {
            auto& bufferInfo   = write.bufferInfo.value();
            auto& vkBufferInfo = vkWriteBufferInfo.at(i);
            vkBufferInfo.buffer = **bufferInfo.buffer;
            vkBufferInfo.offset = bufferInfo.offset;
            vkBufferInfo.range = bufferInfo.range;
            vkWrite.pBufferInfo = &vkBufferInfo;
        }
        if (write.imageInfo.has_value()) {
            auto& imageInfo   = write.imageInfo.value();
            auto& vkImageInfo = vkWriteImageInfo.at(i);
            vkImageInfo.imageLayout = vk::ImageLayout::eGeneral;
            vkImageInfo.imageView   = *imageInfo.imageView;
            vkImageInfo.sampler     = *imageInfo.sampler;
        }
    }
    for (auto i = 0u; i < a_Copies.size(); ++i) {
        auto& copy = a_Copies.at(i);
        auto& vkCopy = vkCopies.at(i);
        vkCopy.srcSet = **copy.srcSet;
        vkCopy.dstSet = **copy.dstSet;
        vkCopy.srcBinding = copy.srcBinding;
        vkCopy.dstBinding = copy.dstBinding;
        vkCopy.srcArrayElement = copy.srcArrayElement;
        vkCopy.dstArrayElement = copy.dstArrayElement;
    }
    device.updateDescriptorSets(vkWrites, vkCopies);
}
}
