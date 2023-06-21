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
            }, a_Tuple);
        return seed;
    }
};

template<typename...>
struct VkObjectCacheKey {};

//to make compiler happy
template<typename, typename>
struct VkObjectCache;

template <typename... Keys, typename Type>
struct VkObjectCache<VkObjectCacheKey<Keys...>, Type> : std::unordered_map<std::tuple<Keys...>, Type, TupleHasher<Keys...>> {
    template <typename Factory>
    constexpr inline auto GetOrCreate(
        const Keys&... a_Keys,
        const Factory& a_Factory)
    {
        return *try_emplace({ a_Keys... }, a_Factory).first->second;
    }
};
}