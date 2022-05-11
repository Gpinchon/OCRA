#pragma once

namespace OCRA::Image
{
namespace Subresource
{
struct Layers {
    /*
    * //TODO support aspect copy for OGL
    enum class Aspect {
        Unknown = -1, Color, Depth, Stencil, DepthStencil, MaxValue
    } aspect{ Aspect::Unknown };
    */
    uint32_t level{ 0 }; //indicates the base level (mipmap or array layer) used for the copy
};
struct Range {
    uint32_t              baseMipLevel{ 0 };
    uint32_t              levelCount{ 0 };
    uint32_t              baseArrayLayer{ 0 };
    uint32_t              layerCount{ 0 };
};
}
}