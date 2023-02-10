#pragma once

#include <OCRA/Handle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Layout);

namespace OCRA {
struct PushConstants {
    PushConstants(const Device::Handle& a_Device);
    ~PushConstants();
    void Bind();
    void Unbind();
    void Update(const size_t& a_Offset, const size_t& a_Size, const std::byte* a_Data);
    const Device::WeakHandle device;
    const uint16_t  size;
    const uint16_t  offsetAlignment;
    const uint32_t  bufferHandle;
    const void*     bufferPtr;
    //the current offset of pushconstant buffer
    uint32_t        offset{ 0 };
};
}
