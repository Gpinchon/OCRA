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
    RenderPass::Handle               renderPass;
    std::vector<Image::View::Handle> attachments; //Image View handles
    Extent<3, uint16_t>              extent; //default FB extent
};
/**
 * Creates a framebuffer with the specified attachments.
 * In case of no attachments, an empty FB with size Info::extent will be created.
 */
Handle Create(
    const Device::Handle&       a_Device,
    const Info&                 a_Info,
    const AllocationCallback*   a_Allocator = nullptr);
}
