#include <PBRMaterial.hpp>

#include <OCRA/OCRA.hpp>

#include <OCRA/ShaderCompiler/Compiler.hpp>
#include <OCRA/ShaderCompiler/Shader.hpp>

namespace OCRA {
auto PBRFragmentShader(const Device::Handle& a_Device)
{
    const auto compiler = ShaderCompiler::Create();
    ShaderCompiler::Shader::Info shaderInfo;
    shaderInfo.type = ShaderCompiler::Shader::Type::Fragment;
    shaderInfo.entryPoint = "main";
    shaderInfo.source = {
        "#version 450                                           \n"
        "layout(location = 0) in vec3 vertColor;                \n"
        "layout(location = 1) in vec2 vertTexCoord;             \n"
        "layout(location = 0) out vec4 outColor;                \n"
        "layout(binding = 1) uniform PBRParameters {            \n"
        "    vec4 albedo;                                       \n"
        "    float roughness;                                   \n"
        "    float metallic;                                    \n"
        "} matParam;                                            \n"
        "layout(binding = 2) uniform sampler2D albedoTexture;   \n"
        "void main() {                                          \n"
        "    outColor = matParam.albedo * vec4(vertColor, 1.0); \n"
        "    outColor *= texture(albedoTexture, vertTexCoord);  \n"
        "}                                                      \n"
    };
    const auto fragmentShader = ShaderCompiler::Shader::Create(compiler, shaderInfo);
    CreateShaderModuleInfo shaderModuleInfo;
    shaderModuleInfo.code = ShaderCompiler::Shader::Compile(fragmentShader);
    const auto shaderModule = CreateShaderModule(a_Device, shaderModuleInfo);
    PipelineShaderStage shaderStageInfo;
    shaderStageInfo.entryPoint = "main";
    shaderStageInfo.stage = ShaderStageFlagBits::Fragment;
    shaderStageInfo.module = shaderModule;
    return shaderStageInfo;
}
PBRMaterial::PBRMaterial(
    const PhysicalDevice::Handle& a_PhysicalDevice,
    const Device::Handle& a_Device)
    : Material(a_Device, PBRParameters{}, { Texture2D(a_Device, Format::Uint8_Normalized_RGBA, 1, 1) }, PBRFragmentShader(a_Device))
{
    //Fill texture with white
    auto& texture = static_cast<const Texture2D&>(GetTexture(0));
    const auto textureSize = Image::GetDataSize(texture.GetImage());
    CreateBufferInfo bufferInfo;
    bufferInfo.size = textureSize;
    bufferInfo.usage = BufferUsageFlagBits::TransferSrc;
    auto textureTransferBuffer = CreateBuffer(a_Device, bufferInfo);
    auto textureTransferMemory = AllocateMemory(a_Device,
        Buffer::GetSizeRequirement(textureTransferBuffer), MemoryPropertyFlagBits::HostVisible);
    Buffer::BindMemory(textureTransferBuffer, textureTransferMemory, 0);
    MemoryMappedRange range;
    range.memory = textureTransferMemory;
    range.length = textureSize;
    range.offset = 0;
    auto bufferPtr = (Vec<4, uint8_t>*)Memory::Map(range);
    for (uint32_t x = 0; x < texture.GetWidth(); ++x) {
        for (uint32_t y = 0; y < texture.GetHeight(); ++y) {
            const size_t index = x + y * texture.GetWidth();
            bufferPtr[index].r = 255;
            bufferPtr[index].g = 255;
            bufferPtr[index].b = 255;
            bufferPtr[index].a = 255;
        }
    }
    Memory::Unmap(textureTransferMemory);
    ImageBufferCopy bufferCopy;
    bufferCopy.bufferImageHeight = texture.GetHeight();
    bufferCopy.bufferRowLength   = texture.GetWidth();
    bufferCopy.bufferOffset      = 0;
    bufferCopy.imageExtent.width  = texture.GetWidth();
    bufferCopy.imageExtent.height = texture.GetHeight();
    bufferCopy.imageExtent.depth  = 1;
    bufferCopy.imageSubresource.mipLevel = 0;
    Image::CopyBufferToImage(textureTransferBuffer, texture.GetImage(), { bufferCopy });
}
}