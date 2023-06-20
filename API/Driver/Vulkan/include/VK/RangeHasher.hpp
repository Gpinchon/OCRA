#pragma once

#include <vulkan/vulkan_hash.hpp>

namespace OCRA {
/**
* @brief hashes a range of values
*/
template<typename It>
size_t HashRange(It a_First, It const& a_Last)
{
    size_t seed = 0;
    VULKAN_HPP_HASH_COMBINE(seed, std::distance(a_First, a_Last));
    for (; a_First != a_Last; ++a_First)
        VULKAN_HPP_HASH_COMBINE(seed, *a_First);
    return seed;
}

template <typename It>
struct RangeHasher {
    using type = It;
    size_t operator()(It a_First, It const& a_Last) const noexcept
    {
        return HashRange(a_Fiste, a_Last);
    }
};
}