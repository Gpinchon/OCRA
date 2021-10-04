/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:50
*/
#pragma once

#include <Scalar.hpp>

typedef Uint32 HandleType;

#define HANDLE(name)                                                                 \
    namespace name {                                                                 \
        typedef HandleType Handle;                                                   \
        struct Info;                                                                 \
        Handle Create(const Device::Handle& a_Device, const Info& a_Info);           \
        void Destroy(const Device::Handle& a_Device, const Handle& a_Handle);        \
        const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle); \
    }

namespace OCRA {
namespace Window {
    typedef HandleType Handle;
    struct Info;
    Handle Create(const Info& a_Info);
    void Destroy(const Handle& a_Handle);
}
namespace Context {
    typedef HandleType Handle;
    struct Info;
    Handle Create(const Info& a_Info);
    void Destroy(const Handle& a_Handle);
}
namespace Device {
    typedef HandleType Handle;
    struct Info;
    Handle Create(const Info& a_Info);
    void Destroy(const Handle& a_Handle);
}
HANDLE(Descriptor);
HANDLE(Descriptor::Layout);
HANDLE(Descriptor::Layout::Binding);
HANDLE(FrameBuffer);
HANDLE(Image);
HANDLE(Image::Descriptor);
HANDLE(Image::Sampler);
HANDLE(Image::View);
HANDLE(Buffer::Transfer);
HANDLE(Buffer::Vertex);
HANDLE(Shader::Module);
HANDLE(Pipeline::Graphics);
HANDLE(State::Draw);
HANDLE(State::Clear);
HANDLE(Pass);
HANDLE(ViewPort);
}