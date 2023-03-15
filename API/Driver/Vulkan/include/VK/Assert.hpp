#pragma once

/**
* Used for the functions returning a vk::result
*/

#ifdef _DEBUG
 #include <stdexcept>
 #include <vulkan/vk_enum_string_helper.h>
 #define VK_CHECK(result)                                     \
    {                                                         \
        if (result != vk::Result::eSuccess) {                 \
            throw std::runtime_error(string_VkResult(VkResult(result)));\
        }                                                     \
    }
#else
 #define VK_CHECK(result)
#endif

#define VK_INVOKE(function) \
    {                       \
        auto res = function;\
        VK_CHECK(res);      \
    }