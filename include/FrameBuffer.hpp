/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:45
*/
#pragma once

#include <Handle.hpp>
#include <Scalar.hpp>

namespace OCRA::FrameBuffer {
constexpr auto MaxColorAttachments = 32;
struct Info {
    Uint16 width { 0 }, height { 0 }, layers { 1 }; //extents, rarelly used except for layers
    Uint8 attachmentCount { 0 };
    Image::View::Handle attachments[MaxColorAttachments]; //Image View handles
};
}
