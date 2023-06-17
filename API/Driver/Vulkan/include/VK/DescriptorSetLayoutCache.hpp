#pragma once

#include <VK/LazyConstructor.hpp>

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_hash.hpp>

#include <vector>
#include <unordered_map>

namespace std {
template <>
struct hash<std::vector<vk::DescriptorSetLayoutBinding>> {
    std::size_t operator()(std::vector<vk::DescriptorSetLayoutBinding> const& a_Vec) const noexcept
    {
        std::size_t seed = std::hash<size_t>{}(a_Vec.size());
        VULKAN_HPP_HASH_COMBINE(seed, a_Vec.size());
        for (auto& binding : a_Vec) {
            //combine the hash using vector size & hash result of each key
            VULKAN_HPP_HASH_COMBINE(seed, binding);
        }
        return seed;
    }
};
}

namespace OCRA
{
template<vk::DescriptorSetLayoutCreateFlagBits Flags>
struct DescriptorSetLayoutCache {
    using KeyType = std::vector<vk::DescriptorSetLayoutBinding>;
    inline auto GetOrCreate(
        const vk::raii::Device& a_Device,
        const std::vector<vk::DescriptorSetLayoutBinding>& a_Bindings)
    {
        auto lazyConstructor = LazyConstructor(
            [
                &a_Device,
                &a_Bindings
            ] () {
                vk::DescriptorSetLayoutCreateInfo info(Flags, a_Bindings);
                return std::move(a_Device.createDescriptorSetLayout(info));
            });
        return *storage.try_emplace(a_Bindings, lazyConstructor).first->second;
    }
    std::unordered_map<KeyType, vk::raii::DescriptorSetLayout> storage;
};
}