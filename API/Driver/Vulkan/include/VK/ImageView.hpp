#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <OCRA/Handles.hpp>

namespace OCRA::Image::View
{
struct Impl : vk::raii::ImageView
{
    Impl(
        const vk::raii::Device& a_Device,
        const vk::ImageViewCreateInfo& a_Info,
        const Image::Handle& a_Image)
        : vk::raii::ImageView(a_Device, a_Info)
        , image(a_Image)
    {}
    const Image::Handle image;
};
}