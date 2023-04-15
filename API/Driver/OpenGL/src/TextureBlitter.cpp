#include <GL/Device.hpp>
#include <GL/Image.hpp>
#include <GL/TextureBlitter.hpp>

constexpr char  ShaderCode[] = "                                    \n\
#version 430														\n\
uniform layout (binding = 0) sampler2D img_src;                     \n\
uniform layout (binding = 1) writeonly image2D img_dst;             \n\
layout(location = 0) uniform vec3 src_offset;                       \n\
layout(location = 1) uniform vec3 src_size;                         \n\
layout(location = 2) uniform vec3 dst_offset;                       \n\
layout(location = 3) uniform vec3 dst_size;                         \n\
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;    \n\
void main() {														\n\
    vec4 pixel = vec4(1, 0, 0, 1);									\n\
    ivec2 iuv = ivec2(gl_GlobalInvocationID);	                    \n\
    ivec2 ijk = ivec2(iuv + dst_offset.xy);                         \n\
    vec2 uvBase = ijk + 0.5;                                        \n\
    {                                                               \n\
        ivec2 size = textureSize(img_src, 0).xy;                    \n\
        vec2 scale  = src_size.xy / dst_size.xy;                    \n\
        vec2 uvOffset = uvBase - dst_offset.xy;                     \n\
        vec2 uvScaled = uvOffset * scale;                           \n\
        vec2 uv = uvScaled + src_offset.xy;                         \n\
        vec2 texCoord = uv / vec2(size);                            \n\
        pixel = texture(img_src, texCoord);                         \n\
    }                                                               \n\
    {                                                               \n\
        imageStore(img_dst, ijk, pixel);                            \n\
    }                                                               \n\
}                                                                   \n\
";
constexpr GLint CodeLength = sizeof(ShaderCode);

OCRA::TextureBlitter::TextureBlitter(Device::Impl& a_Device)
    : device(a_Device)
{
    device.PushCommand([this] {
        glCreateSamplers(1, &samplerSrc);

        shaderProgram = glCreateProgram();
        auto shader = glCreateShader(GL_COMPUTE_SHADER);
        const char* codePtr = ShaderCode;
        glShaderSource(shader, 1, &codePtr, &CodeLength);
        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::string errorLog;
            errorLog.resize(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

            // Provide the infolog in whatever manor you deem best.
            // Exit with failure.
            glDeleteShader(shader); // Don't leak the shader.
            return;
        }

        glAttachShader(shaderProgram, shader);
        glLinkProgram(shaderProgram);
        GLint isLinked = 0;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::string infoLog;
            infoLog.resize(maxLength);
            glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);

            // The program is useless now. So delete it.
            glDeleteProgram(shaderProgram);

            // Provide the infolog in whatever manner you deem best.
            // Exit with failure.
            return;
        }

        glDetachShader(shaderProgram, shader);
        glDeleteShader(shader);
    }, false);
}

OCRA::TextureBlitter::~TextureBlitter()
{
    device.PushCommand([shaderProgram = shaderProgram] {
        glDeleteProgram(shaderProgram);
    }, false);
}

void OCRA::TextureBlitter::Blit(
    const Image::Handle& a_SrcImage,
    const Image::Handle& a_DstImage,
    const std::vector<ImageBlit>& a_Blits,
    const Filter& a_Filter) const
{
    glSamplerParameteri(samplerSrc, GL_TEXTURE_MAG_FILTER, a_Filter == Filter::Linear ? GL_LINEAR : GL_NEAREST);
    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, a_SrcImage->handle);
    glBindSampler(0, samplerSrc);
    for (const auto& blit : a_Blits) {
        const Offset3D srcOffset = blit.srcOffsets[0];
        const Offset3D srcSize   = {
            blit.srcOffsets[1].x - blit.srcOffsets[0].x,
            blit.srcOffsets[1].y - blit.srcOffsets[0].y,
            blit.srcOffsets[1].z - blit.srcOffsets[0].z,
        };
        const Offset3D dstOffset = blit.dstOffsets[0];
        const Offset3D dstSize   = {
            blit.dstOffsets[1].x - blit.dstOffsets[0].x,
            blit.dstOffsets[1].y - blit.dstOffsets[0].y,
            blit.dstOffsets[1].z - blit.dstOffsets[0].z,
        };
        glBindImageTexture(1,
            a_DstImage->handle,
            blit.dstSubresource.mipLevel,
            true,
            blit.dstSubresource.baseArrayLayer,
            GL_WRITE_ONLY,
            a_DstImage->internalFormat);
        glUniform3f(0, srcOffset.x, srcOffset.y, srcOffset.z);
        glUniform3f(1, srcSize.x,   srcSize.y,   srcSize.z);
        glUniform3f(2, dstOffset.x, dstOffset.y, dstOffset.z);
        glUniform3f(3, dstSize.x,   dstSize.y,   dstSize.z);
        glDispatchCompute(dstSize.x, dstSize.y, dstSize.z);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindSampler(0, 0);
    glBindTextureUnit(1, 0);
    glUseProgram(0);
}
