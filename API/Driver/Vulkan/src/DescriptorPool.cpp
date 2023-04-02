#include <OCRA/Core.hpp>

#include <VK/DescriptorPool.hpp>
#include <VK/DescriptorSet.hpp>
#include <VK/DescriptorSetLayout.hpp>
#include <VK/Enums.hpp>
#include <VK/Structs.hpp>

#include <vector>

namespace OCRA::Descriptor::Pool
{
Descriptor::Set::Handle AllocateDescriptorSet(
    const Descriptor::Pool::Handle&  a_Pool,
    const AllocateDescriptorSetInfo& a_Info)
{
    std::vector<vk::DescriptorSetLayoutBinding> bindings(a_Info.bindings.size());
    std::transform(
        a_Info.bindings.begin(), a_Info.bindings.end(),
        bindings.begin(), [](const DescriptorSetLayoutBinding& binding) { return ConvertToVk(binding); });
    auto& pool = *a_Pool;
    auto& device = pool.device;
    auto& descriptorSetLayout = device.GetOrCreateDescriptorSetLayout(bindings);
    vk::DescriptorSetAllocateInfo info;
    info.descriptorPool = *pool;
    info.descriptorSetCount = 1;
    info.pSetLayouts = &descriptorSetLayout;
    auto descriptorSets = device.allocateDescriptorSets(info);
    auto allocator = std::pmr::polymorphic_allocator<Descriptor::Set::Impl>(&pool.memoryResource);
    return std::allocate_shared<Descriptor::Set::Impl>(allocator,
        std::move(descriptorSets.front()));
}
}