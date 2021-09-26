/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:53
*/
#include <Handle.hpp>
#include <Pipeline/Graphics.hpp>

#include <functional>
#include <map>
#include <vector>

#include <Driver/OpenGL/Pipeline/ColorBlendState.hpp>
#include <Driver/OpenGL/Pipeline/ShaderStage.hpp>
#include <GL/glew.h>

namespace OCRA::Pipeline::Graphics {
struct Impl {
    ~Impl()
    {
        glDeleteProgram(shaderProgram);
    }
    Info info;
    GLuint shaderProgram { 0 };
    std::function<void()> colorBlendState;
};
static Handle s_CurrentHandle = 0;
static std::map<Handle, Impl> s_GraphicsPipelines;
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
    Impl impl;
    impl.info = a_Info;
    impl.shaderProgram = ShaderStage::Compile(a_Device, a_Info.shaderStage);
    impl.colorBlendState = ColorBlendState::Compile(a_Info.colorBlendState);
    ++s_CurrentHandle;
    s_GraphicsPipelines[s_CurrentHandle] = impl;
    return s_CurrentHandle;
}
void Destroy(const Device::Handle& a_Device, const Handle& a_Handle)
{
    s_GraphicsPipelines.erase(a_Handle);
}
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle)
{
    return s_GraphicsPipelines.at(a_Handle).info;
}
}