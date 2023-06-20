#pragma once

#include <VK/LazyConstructor.hpp>
#include <VK/RangeHasher.hpp>
#include <VK/VkObjectCache.hpp>

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_hash.hpp>

#include <vector>

namespace std {
template <>
struct hash<std::vector<vk::DescriptorSetLayoutBinding>> {
    using type = std::vector<vk::DescriptorSetLayoutBinding>;
    std::size_t operator()(type const& a_Vec) const noexcept
    {
        return OCRA::HashRange(a_Vec.begin(), a_Vec.end());
    }
};
}

namespace OCRA
{
using DescriptorSetLayoutCacheKey = VkObjectCacheKey<vk::DescriptorSetLayoutCreateFlags, std::vector<vk::DescriptorSetLayoutBinding>>;
struct DescriptorSetLayoutCache : VkObjectCache<DescriptorSetLayoutCacheKey, vk::raii::DescriptorSetLayout> {
    inline auto GetOrCreate(
        const vk::raii::Device& a_Device,
        const vk::DescriptorSetLayoutCreateFlags& a_Flags,
        const std::vector<vk::DescriptorSetLayoutBinding>& a_Bindings)
    {
        //use a lazy constructor to build a new pipeline only if needed
        auto lazyConstructor = LazyConstructor(
            [
                &a_Device,
                &a_Flags,
                &a_Bindings
            ] () {
                vk::DescriptorSetLayoutCreateInfo info(a_Flags, a_Bindings);
                return std::move(a_Device.createDescriptorSetLayout(info));
            });
        return VkObjectCache::GetOrCreate(a_Device, { a_Flags, a_Bindings }, lazyConstructor);
    }
};
}