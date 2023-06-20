#pragma once

#include <VK/LazyConstructor.hpp>
#include <VK/RangeHasher.hpp>
#include <VK/VkObjectCache.hpp>

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_hash.hpp>

#include <vector>

namespace std {
template <>
struct hash<std::vector<vk::PushConstantRange>> {
    using type = std::vector<vk::PushConstantRange>;
    std::size_t operator()(type const& a_Vec) const noexcept
    {
        return OCRA::HashRange(a_Vec.begin(), a_Vec.end());
    }
};
}

namespace OCRA
{
using PipelineLayoutCacheKey = VkObjectCacheKey<vk::DescriptorSetLayout, std::vector<vk::PushConstantRange>>;
struct PipelineLayoutCache : VkObjectCache<PipelineLayoutCacheKey, vk::raii::PipelineLayout> {
    inline auto GetOrCreate(
        vk::raii::Device& a_Device,
        const vk::DescriptorSetLayout& a_Layout,
        const std::vector<vk::PushConstantRange>& a_PushConstants)
    {
        //use a lazy constructor to build a new pipeline only if needed
        auto lazyConstructor = LazyConstructor(
            [
                &a_Device,
                &a_Layout,
                &a_PushConstants
            ] () {
                vk::PipelineLayoutCreateInfo info({}, a_Layout, a_PushConstants);
                return std::move(a_Device.createPipelineLayout(info));
            });
        return VkObjectCache::GetOrCreate(a_Device, { a_Layout, a_PushConstants }, lazyConstructor);
    }
};
}