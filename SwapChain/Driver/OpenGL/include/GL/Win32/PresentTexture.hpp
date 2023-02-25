#pragma once

#include <OCRA/Core.hpp>

namespace OCRA::SwapChain::Win32 {
    struct PresentTexture {
        PresentTexture(const Image::Handle& a_FromImage);
        ~PresentTexture();
        void Bind() const;
        void Unbind() const;
        unsigned handle{ 0 };
        unsigned samplerHandle{ 0 };
        const unsigned target{ 0 };
        const unsigned dataType{ 0 };
        const unsigned dataFormat{ 0 };
        const unsigned internalFormat{ 0 };
        const Extent3D extent{ 0, 0, 0 };
    };
}