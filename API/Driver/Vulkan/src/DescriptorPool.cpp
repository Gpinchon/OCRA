#include <VK/Device.hpp>
#include <VK/DescriptorPool.hpp>
#include <VK/DescriptorSet.hpp>
#include <VK/DescriptorSetLayout.hpp>
#include <VK/Enums.hpp>

#include <OCRA/Structs.hpp>

namespace OCRA::Device
{
Descriptor::Pool::Handle CreateDescriptorPool(const Device::Handle& a_Device, const CreateDescriptorPoolInfo& a_Info, const AllocationCallback* a_Allocator)
{
    VkDescriptorPool pool = nullptr;
    VkDescriptorPoolCreateInfo info{ VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
    std::vector<VkDescriptorPoolSize> vkPoolSizes;
    vkPoolSizes.reserve(a_Info.sizes.size());
    for (const auto& size : a_Info.sizes) {
        VkDescriptorPoolSize vkSize{};
        vkSize.descriptorCount = size.count;
        vkSize.type = GetVkDescriptorType(size.type);
        vkPoolSizes.push_back(vkSize);
    }
    info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT | VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;
    info.poolSizeCount = vkPoolSizes.size();
    info.pPoolSizes = vkPoolSizes.data();
    info.maxSets = a_Info.maxSets;
    vkCreateDescriptorPool(*a_Device, &info, nullptr, &pool);
    return std::make_shared<Descriptor::Pool::Impl>(*a_Device, pool);
}
}

namespace OCRA::Descriptor::Pool
{
Descriptor::Set::Handle AllocateSet(const AllocateDescriptorSetInfo& a_Info)
{
    auto& pool = *a_Info.pool;
    VkDescriptorSet vkSet = nullptr;
    VkDescriptorSetAllocateInfo info{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
    info.descriptorPool = pool;
    info.descriptorSetCount = 1;
    info.pSetLayouts = &(const VkDescriptorSetLayout&)(*a_Info.layout);
    vkAllocateDescriptorSets(pool.device, &info, &vkSet);
    auto allocator = std::pmr::polymorphic_allocator<Set::Impl>(&pool.memoryResource);
    return std::allocate_shared<Set::Impl>(allocator,
        pool.device, (const VkDescriptorPool&)pool, vkSet);
}
}