#pragma once

namespace OCRA::SwapChain::Win32 {
struct PresentPixels {
    PresentPixels(const size_t& a_Size);
    ~PresentPixels();
    void Bind();
    void Unbind();
    void Flush();
    void* GetPtr()
    {
        return (char*)ptr + offset;
    }
    unsigned handle;
    const size_t size;
    void* ptr;
    size_t offsetIndex { 0 };
    size_t offset { 0 };
};
}
