/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:31:40
*/
#include <Buffer/Buffer.hpp>
#include <Handle.hpp>

#include <map>

#include <GL/glew.h>

namespace OCRA::Buffer {
struct Impl {
    ~Impl()
    {
        glDeleteBuffers(1, &buffer);
    }
    GLuint buffer { 0 };
    Info info;
};
static Handle s_CurrentHandle = 0;
static std::map<Handle, Impl> s_Buffers;
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{

    Impl impl;
    impl.info = a_Info;
    glGenBuffers(1, &impl.buffer);
    glNamedBufferStorage(impl.buffer, a_Info.size, nullptr, 0); //flag is zero for now
    ++s_CurrentHandle;
    s_Buffers[s_CurrentHandle] = impl;
    return s_CurrentHandle;
}
void Destroy(const Device::Handle& a_Device, const Handle& a_Handle)
{
    s_Buffers.erase(a_Handle);
}
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle)
{
    return s_Buffers.at(a_Handle).info;
}
}