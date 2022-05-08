/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:45
*/
#pragma once

#include <Handle.hpp>
#include <Common/Extent3D.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::FrameBuffer);
OCRA_DECLARE_HANDLE(OCRA::Image::View);
OCRA_DECLARE_HANDLE(OCRA::RenderPass);

namespace OCRA {
struct AllocationCallback;
}

namespace OCRA::FrameBuffer {
struct Info {
    RenderPass::Handle                  renderPass;
    std::vector<Image::View::Handle>    attachments; //Image View handles
    Extent<3, uint16_t>                 extent; //extents, rarelly used except for layers
};
Handle Create(
    const Device::Handle&       a_Device,
    const Info&                 a_Info,
    const AllocationCallback*   a_Allocator = nullptr);
}
