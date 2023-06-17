#pragma once

#include <VK/LazyConstructor.hpp>

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_hash.hpp>

#include <vector>
#include <unordered_map>

namespace std {
template <>
struct hash<std::vector<vk::PushConstantRange>> {
    std::size_t operator()(std::vector<vk::PushConstantRange> const& a_Vec) const noexcept
    {
        std::size_t seed = 0;
        VULKAN_HPP_HASH_COMBINE(seed, a_Vec.size());
        for (auto& pushConstant : a_Vec) {
            //combine the hash using vector size & hash result of each key
            VULKAN_HPP_HASH_COMBINE(seed, pushConstant);
        }
        return seed;
    }
};
template <>
struct hash<std::pair<vk::DescriptorSetLayout, std::vector<vk::PushConstantRange>>> {
    std::size_t operator()(std::pair<vk::DescriptorSetLayout, std::vector<vk::PushConstantRange>> const& a_Pair) const noexcept
    {
        std::size_t seed = 0;
        VULKAN_HPP_HASH_COMBINE(seed, a_Pair.first);
        VULKAN_HPP_HASH_COMBINE(seed, a_Pair.second);
        return seed;
    }
};
}

namespace OCRA
{
struct PipelineLayoutCache {
    using KeyType = std::pair<vk::DescriptorSetLayout, std::vector<vk::PushConstantRange>>;
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
        return *storage.try_emplace({ a_Layout, a_PushConstants }, lazyConstructor).first->second;
    }
    std::unordered_map<KeyType, vk::raii::PipelineLayout> storage;
};
}