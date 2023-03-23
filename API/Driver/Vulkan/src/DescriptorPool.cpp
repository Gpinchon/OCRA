#include <OCRA/Core.hpp>

#include <VK/DescriptorPool.hpp>
#include <VK/DescriptorSet.hpp>
#include <VK/DescriptorSetLayout.hpp>
#include <VK/Enums.hpp>

#include <vector>

namespace OCRA::Descriptor::Pool
{
Descriptor::Set::Handle AllocateDescriptorSet(
    const Descriptor::Pool::Handle&  a_Pool,
    const AllocateDescriptorSetInfo& a_Info)
{
    auto& pool = *a_Pool;
    auto& device = pool.device;
    auto& descriptorSetLayout = **a_Info.layout;
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