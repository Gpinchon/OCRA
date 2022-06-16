#pragma once

namespace OCRA::SwapChain::Win32 {
struct PresentShader {
    PresentShader();
    ~PresentShader();
    void Bind() const;
    void Unbind() const;
    unsigned handle{ 0 };
};
}