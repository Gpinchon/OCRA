#include <VK/Device.hpp>
#include <VK/DescriptorSetLayout.hpp>
#include <VK/Enums.hpp>
#include <VK/Flags.hpp>

#include <OCRA/Core.hpp>

namespace OCRA::Device
{
Descriptor::SetLayout::Handle CreateDescriptorSetLayout(
    const Device::Handle& a_Device,
    const CreateDescriptorSetLayoutInfo& a_Info,
    const AllocationCallback* a_Allocator = nullptr)
{
    const VkDevice& device = *a_Device;
    VkDescriptorSetLayout setLayout = nullptr;
    VkDescriptorSetLayoutCreateInfo info{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
    std::vector<VkDescriptorSetLayoutBinding> vkBindings;
    vkBindings.reserve(a_Info.bindings.size());
    for (const auto& binding : a_Info.bindings)
    {
        VkDescriptorSetLayoutBinding vkBinding{};
        vkBinding.binding = binding.binding;
        vkBinding.descriptorCount = binding.count;
        vkBinding.descriptorType = GetVkDescriptorType(binding.type);
        vkBinding.stageFlags = GetVkShaderStage(binding.stageFlags);
        vkBindings.push_back(vkBinding);
    }
    info.bindingCount = vkBindings.size();
    info.pBindings    = vkBindings.data();
    vkCreateDescriptorSetLayout(device, &info, nullptr, &setLayout);
    return std::make_shared<Descriptor::SetLayout::Impl>(device, setLayout);
}
}