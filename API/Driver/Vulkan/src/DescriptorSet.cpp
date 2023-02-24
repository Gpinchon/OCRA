#include <VK/DescriptorSet.hpp>
#include <VK/DescriptorType.hpp>

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
    std::vector<VkWriteDescriptorSet> vkWrites;
    std::vector<VkCopyDescriptorSet>  vkCopies;
    vkWrites.reserve(a_Writes.size());
    vkCopies.reserve(a_Copies.size());
    for (const auto& write : a_Writes) {
        VkWriteDescriptorSet vkWrite{ VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
        vkWrite.dstSet     = *write.dstSet;
        vkWrite.dstBinding = write.dstBinding;
        vkWrite.dstArrayElement = write.dstArrayElement;
        vkWrite.descriptorCount = write.dstCount;
        vkWrite.descriptorType  = GetVkDescriptorType(write.type);
        vkWrites.push_back(vkWrite);
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
        vkWrites.size(), vkWrites.data(),
        vkCopies.size(), vkCopies.data());
}
}
