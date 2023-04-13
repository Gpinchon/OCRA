#include <OCRA/Core.hpp>

#include <GL/ShaderModule.hpp>

namespace OCRA::Device
{
ShaderModule::Handle CreateShaderModule(const Device::Handle& a_Device, const CreateShaderModuleInfo& a_Info)
{
    return std::make_shared<ShaderModule::Impl>(a_Device, a_Info);
}
}