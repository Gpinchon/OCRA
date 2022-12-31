#include <PBRMaterial.hpp>

#include <OCRA/Memory.hpp>

#include <OCRA/ShaderCompiler/Compiler.hpp>
#include <OCRA/ShaderCompiler/Shader.hpp>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Shader::Stage);

namespace OCRA {
auto PBRFragmentShader(const Device::Handle& a_Device)
{
    static Shader::Stage::WeakHandle shaderStage;
    if (!shaderStage.expired()) return shaderStage.lock();
    const auto compiler = ShaderCompiler::Create();
    ShaderCompiler::Shader::Info shaderInfo;
    shaderInfo.type = ShaderCompiler::Shader::Type::Fragment;
    shaderInfo.entryPoint = "main";
    shaderInfo.source = {
        "#version 450                                           \n"
        "layout(location = 0) in vec3 vertColor;                \n"
        "layout(location = 0) out vec4 outColor;                \n"
        "layout(set = 1, binding = 1) uniform PBRParameters {   \n"
        "    vec4 albedo;                                       \n"
        "    float roughness;                                   \n"
        "    float metallic;                                    \n"
        "} matParam;                                            \n"
        "void main() {                                          \n"
        "    outColor = matParam.albedo * vec4(vertColor, 1.0); \n"
        "}                                                      \n"
    };
    const auto fragmentShader = ShaderCompiler::Shader::Create(compiler, shaderInfo);
    Shader::Module::Info shaderModuleInfo;
    shaderModuleInfo.code = ShaderCompiler::Shader::Compile(fragmentShader);
    const auto shaderModule = Shader::Module::Create(a_Device, shaderModuleInfo);
    Shader::Stage::Info shaderStageInfo;
    shaderStageInfo.entryPoint = "main";
    shaderStageInfo.stage = Shader::Stage::StageFlagBits::Fragment;
    shaderStageInfo.module = shaderModule;
    auto shaderStageHandle = Shader::Stage::Create(a_Device, shaderStageInfo);
    shaderStage = shaderStageHandle;
    return shaderStageHandle;
}
PBRMaterial::PBRMaterial(
    const PhysicalDevice::Handle& a_PhysicalDevice,
    const Device::Handle& a_Device)
    : Material(a_PhysicalDevice, a_Device, PBRParameters{}, { Texture2D(a_Device, Image::Format::Uint8_Normalized_RGBA, 1, 1) }, PBRFragmentShader(a_Device))
{
    //Fill texture with white
    auto& texture = static_cast<const Texture2D&>(GetTexture(0));
    const auto textureSize = texture.GetWidth() * texture.GetHeight() * Image::GetPixelSize(texture.GetImageInfo().format);
    Buffer::Info bufferInfo;
    bufferInfo.size = textureSize;
    bufferInfo.usage = Buffer::UsageFlagBits::TransferSrc;
    auto textureTransferBuffer = Buffer::Create(a_Device, bufferInfo);
    auto textureTransferMemory = AllocateMemory(
        a_PhysicalDevice, a_Device,
        bufferInfo.size,
        PhysicalDevice::MemoryPropertyFlagBits::HostVisible);
    Buffer::BindMemory(a_Device, textureTransferBuffer, textureTransferMemory, 0);
    Memory::MappedRange range;
    range.memory = textureTransferMemory;
    range.length = textureSize;
    range.offset = 0;
    auto bufferPtr = (Vec<4, uint8_t>*)Memory::Map(a_Device, range);
    for (uint32_t x = 0; x < texture.GetWidth(); ++x) {
        for (uint32_t y = 0; y < texture.GetHeight(); ++y) {
            const size_t index = x + y * texture.GetWidth();
            bufferPtr[index].r = 255;
            bufferPtr[index].g = 255;
            bufferPtr[index].b = 255;
            bufferPtr[index].a = 255;
        }
    }

    
}
}