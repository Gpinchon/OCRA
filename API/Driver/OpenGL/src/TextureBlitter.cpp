#include <GL/Device.hpp>
#include <GL/Image.hpp>
#include <GL/TextureBlitter.hpp>

#include <iostream>

constexpr char  ShaderCode[] = "                                    \n\
#version 430														\n\
uniform layout (binding = 0) sampler3D img_src;                     \n\
uniform layout (binding = 1) writeonly image3D img_dst;             \n\
layout(location = 0) uniform vec3 src_offset;                       \n\
layout(location = 1) uniform vec3 src_size;                         \n\
layout(location = 2) uniform vec3 dst_offset;                       \n\
layout(location = 3) uniform vec3 dst_size;                         \n\
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;    \n\
void main() {														\n\
    vec4 pixel = vec4(1, 0, 0, 1);									\n\
    ivec3 iuv = ivec3(gl_GlobalInvocationID);	                    \n\
    ivec3 ijk = ivec3(iuv + dst_offset);                            \n\
    vec3 uvBase = ijk + 0.5;                                        \n\
    {                                                               \n\
        ivec3 size = textureSize(img_src, 0);                       \n\
        vec3 scale  = src_size / dst_size;                          \n\
        vec3 uvOffset = uvBase - dst_offset;                        \n\
        vec3 uvScaled = uvOffset * scale;                           \n\
        vec3 uv = uvScaled + src_offset;                            \n\
        vec3 texCoord = uv / vec3(size);                            \n\
        pixel = texture(img_src, texCoord);                         \n\
    }                                                               \n\
    {                                                               \n\
        imageStore(img_dst, ijk, pixel);                            \n\
    }                                                               \n\
}                                                                   \n\
";
constexpr GLint CodeLength = sizeof(ShaderCode);

static inline bool CheckShader(GLuint shader)
{
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::string infoLog;
        infoLog.resize(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
        std::cerr << infoLog << std::endl;

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(shader); // Don't leak the shader.
        return false;
    }
    return true;
}

static inline bool CheckProgram(GLuint program)
{
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::string infoLog;
        infoLog.resize(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
        std::cerr << infoLog << std::endl;

        // The program is useless now. So delete it.
        glDeleteProgram(program);

        // Provide the infolog in whatever manner you deem best.
        // Exit with failure.
        return false;
    }
    return true;
}

OCRA::TextureBlitter::TextureBlitter(Device::Impl& a_Device)
    : device(a_Device)
{
    device.PushCommand([this] {
        glCreateFramebuffers(2, frameBuffers.data());

        glCreateSamplers(1, &samplerSrc);
        shaderProgram = glCreateProgram();
        auto shader = glCreateShader(GL_COMPUTE_SHADER);
        const char* codePtr = ShaderCode;
        glShaderSource(shader, 1, &codePtr, &CodeLength);
        glCompileShader(shader);
        if (!CheckShader(shader)) return;
        glAttachShader(shaderProgram, shader);
        glLinkProgram(shaderProgram);
        if (!CheckProgram(shaderProgram)) return;
        glDetachShader(shaderProgram, shader);
        glDeleteShader(shader);
    }, false);
}

OCRA::TextureBlitter::~TextureBlitter()
{
    device.PushCommand([frameBuffers = frameBuffers, shaderProgram = shaderProgram, samplerSrc = samplerSrc] {
        glDeleteFramebuffers(2, frameBuffers.data());
        glDeleteSamplers(1, &samplerSrc);
        glDeleteProgram(shaderProgram);
    }, false);
}

void OCRA::TextureBlitter::Blit3D(
    const Image::Handle& a_SrcImage,
    const Image::Handle& a_DstImage,
    const ImageBlit& a_Blit,
    const Filter& a_Filter) const
{
    glSamplerParameteri(samplerSrc, GL_TEXTURE_MAG_FILTER, a_Filter == Filter::Linear ? GL_LINEAR : GL_NEAREST);
    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(a_SrcImage->target, a_SrcImage->handle);
    glBindSampler(0, samplerSrc);
    auto& blit = a_Blit;
    const Offset3D srcOffset = blit.srcOffsets[0];
    const Offset3D srcSize = {
        blit.srcOffsets[1].x - blit.srcOffsets[0].x,
        blit.srcOffsets[1].y - blit.srcOffsets[0].y,
        blit.srcOffsets[1].z - blit.srcOffsets[0].z,
    };
    const Offset3D dstOffset = blit.dstOffsets[0];
    const Offset3D dstSize = {
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
    glUniform3f(1, srcSize.x, srcSize.y, srcSize.z);
    glUniform3f(2, dstOffset.x, dstOffset.y, dstOffset.z);
    glUniform3f(3, dstSize.x, dstSize.y, dstSize.z);
    glDispatchCompute(dstSize.x, dstSize.y, dstSize.z);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindSampler(0, 0);
    glBindTextureUnit(1, 0);
    glUseProgram(0);
}

void OCRA::TextureBlitter::Blit(
    const Image::Handle& a_SrcImage,
    const Image::Handle& a_DstImage,
    const size_t a_BlitsCount, const ImageBlit* a_Blits,
    const Filter& a_Filter) const
{
    auto& srcTarget = a_SrcImage->target;
    auto& srcHandle = a_SrcImage->handle;
    auto& dstTarget = a_DstImage->target;
    auto& dstHandle = a_DstImage->handle;
    auto& target = srcTarget;
    const auto filter = a_Filter == Filter::Linear ? GL_LINEAR : GL_NEAREST;
    OCRA_ASSERT(srcTarget == dstTarget && "Textures must be of the same type to be blitted");
    for (auto blitPtr = a_Blits; blitPtr != a_Blits + a_BlitsCount; ++blitPtr)
    {
        auto& blit = *blitPtr;
        if (target == GL_TEXTURE_3D) {
            Blit3D(a_SrcImage, a_DstImage, blit, a_Filter);
            continue;
        }
        if (target == GL_TEXTURE_1D) {
            glNamedFramebufferTexture1DEXT(
                frameBuffers.at(0), GL_COLOR_ATTACHMENT0,
                target, srcHandle,
                blit.srcSubresource.mipLevel);
            glNamedFramebufferTexture1DEXT(
                frameBuffers.at(0), GL_COLOR_ATTACHMENT0,
                target, dstHandle,
                blit.dstSubresource.mipLevel);
        }
        else if (target == GL_TEXTURE_2D) {
            glNamedFramebufferTexture2DEXT(
                frameBuffers.at(0), GL_COLOR_ATTACHMENT0,
                target, srcHandle,
                blit.srcSubresource.mipLevel);
            glNamedFramebufferTexture2DEXT(
                frameBuffers.at(1), GL_COLOR_ATTACHMENT0,
                target, dstHandle,
                blit.dstSubresource.mipLevel);
        }
        glNamedFramebufferReadBuffer(frameBuffers.at(0), GL_COLOR_ATTACHMENT0);
        glNamedFramebufferDrawBuffer(frameBuffers.at(1), GL_COLOR_ATTACHMENT0);
        glBlitNamedFramebuffer(
            frameBuffers.at(0), frameBuffers.at(1),
            blit.srcOffsets[0].x, blit.srcOffsets[0].y,
            blit.srcOffsets[1].x, blit.srcOffsets[1].y,
            blit.dstOffsets[0].x, blit.dstOffsets[0].y,
            blit.dstOffsets[1].x, blit.dstOffsets[1].y,
            GL_COLOR_BUFFER_BIT,
            filter);
    }
}

void OCRA::TextureBlitter::Blit(
    const Image::Handle& a_SrcImage,
    const Image::Handle& a_DstImage,
    const std::vector<ImageBlit>& a_Blits,
    const Filter& a_Filter) const
{
    Blit(
        a_SrcImage, a_DstImage,
        a_Blits.size(), a_Blits.data(),
        a_Filter);
}
