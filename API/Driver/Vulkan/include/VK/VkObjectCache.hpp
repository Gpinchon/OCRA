#pragma once

#include <vulkan/vulkan_hash.hpp>

#include <tuple>
#include <unordered_map>

namespace OCRA {
template <typename... Types>
struct TupleHasher {
    using type = std::tuple<Types...>;
    constexpr inline auto operator()(type const& a_Tuple) const noexcept
    {
        std::size_t seed = 0;
        std::apply(
            [&seed](auto&&... a_Args) {
                ((VULKAN_HPP_HASH_COMBINE(seed, a_Args)), ...);
            },
            a_Tuple);
        return seed;
    }
};

template<typename...>
struct VkObjectCacheKey {};

//to make compiler happy
template<typename, typename>
struct VkObjectCache;

template <typename... Keys, typename Type>
class VkObjectCache<VkObjectCacheKey<Keys...>, Type> {
public:
    using type     = Type;
    using key_type = std::tuple<Keys...>;
    using hasher   = TupleHasher<Keys...>;
    template <typename Factory>
    constexpr inline auto GetOrCreate(
        const vk::raii::Device& a_Device,
        const key_type& a_Key,
        const Factory& a_Factory)
    {
        return *m_storage.try_emplace(a_Key, a_Factory).first->second;
    }
    void clear() {
        m_storage.clear();
    }

private:
    std::unordered_map<key_type, type, hasher> m_storage;
};
}