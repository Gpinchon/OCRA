#include <VK/Buffer.hpp>
#include <VK/DescriptorSet.hpp>
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
    VkDevice device = nullptr;
    if (!a_Writes.empty()) device = a_Writes.front().dstSet->device;
    else if (!a_Copies.empty()) device = a_Copies.front().dstSet->device;
    std::vector<VkWriteDescriptorSet>   vkWrites(a_Writes.size(), { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET });
    std::vector<VkDescriptorBufferInfo> vkWriteBufferInfo(a_Writes.size());
    std::vector<VkDescriptorImageInfo>  vkWriteImageInfo(a_Writes.size());
    std::vector<VkBufferView>           vkWriteBufferView(a_Writes.size());

    std::vector<VkCopyDescriptorSet>    vkCopies(a_Copies.size());

    for (auto i = 0u; i < a_Writes.size(); ++i) {
        auto& write   = a_Writes.at(i);
        auto& vkWrite = vkWrites.at(i);
        vkWrite.dstSet     = *write.dstSet;
        vkWrite.dstBinding = write.dstBinding;
        vkWrite.dstArrayElement = write.dstArrayElement;
        vkWrite.descriptorCount = write.dstCount;
        vkWrite.descriptorType  = GetVkDescriptorType(write.type);
        if (write.bufferInfo.has_value()) {
            auto& bufferInfo   = write.bufferInfo.value();
            auto& vkBufferInfo = vkWriteBufferInfo.at(i);
            vkBufferInfo.buffer = *bufferInfo.buffer;
            vkBufferInfo.offset = bufferInfo.offset;
            vkBufferInfo.range = bufferInfo.range;
            vkWrite.pBufferInfo = &vkBufferInfo;
        }
        if (write.imageInfo.has_value()) {
            auto& imageInfo   = write.imageInfo.value();
            auto& vkImageInfo = vkWriteImageInfo.at(i);
            vkImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
            vkImageInfo.imageView   = *imageInfo.imageView;
            vkImageInfo.sampler     = *imageInfo.sampler;
        }
    }
    for (const auto& copy : a_Copies) {
        VkCopyDescriptorSet vkCopy{ VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET };
        vkCopy.srcSet = *copy.srcSet;
        vkCopy.dstSet = *copy.dstSet;
        vkCopy.srcBinding = copy.srcBinding;
        vkCopy.dstBinding = copy.dstBinding;
        vkCopy.srcArrayElement = copy.srcArrayElement;
        vkCopy.dstArrayElement = copy.dstArrayElement;
        vkCopies.push_back(vkCopy);
    }
    vkUpdateDescriptorSets(device,
        a_Writes.size(), vkWrites.data(),
        a_Copies.size(), vkCopies.data());
}
}
