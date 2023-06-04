#pragma once

namespace OCRA::SwapChain::Win32 {
struct PresentGeometry {
    PresentGeometry();
    ~PresentGeometry();
    void Bind() const;
    void Unbind() const;
    void Draw() const;
    unsigned VBOhandle;
    unsigned VAOhandle;
};
}
