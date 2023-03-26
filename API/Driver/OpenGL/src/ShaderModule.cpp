#include <OCRA/Core.hpp>

#include <GL/ShaderModule.hpp>

namespace OCRA::Device
{
Shader::Module::Handle CreateShaderModule(const Device::Handle& a_Device, const CreateShaderModuleInfo& a_Info)
{
    return std::make_shared<Shader::Module::Impl>(a_Device, a_Info);
}
}