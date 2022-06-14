#include <GL/Win32/SwapChain.hpp>
#include <GL/Surface.hpp>

#include <GL/Win32/Error.hpp>
#include <GL/Win32/OpenGL.hpp>
#include <GL/Device.hpp>
#include <GL/Image/Format.hpp>
#include <GL/Image/Image.hpp>
#include <GL/Queue/Queue.hpp>
#include <GL/Queue/Semaphore.hpp>
#include <GL/Queue/Fence.hpp>

#include <GL/glew.h>
#include <GL/wglew.h>

#include <iostream>
#include <sstream>

namespace OCRA::SwapChain::Win32
{
void GLAPIENTRY MessageCallback(
    GLenum source,
    GLenum type,
    GLenum id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    if (type == GL_DEBUG_TYPE_ERROR)
    {
        std::stringstream ss{};
        ss << "GL CALLBACK : **GL ERROR * *\n" <<
            " type     = " << type << "\n" <<
            " severity = " << severity << "\n" <<
            " message  = " << message;
        std::cerr << ss.str() << std::endl;
        throw std::runtime_error(ss.str());
    }
}
static inline auto CreateImages(const Device::Handle& a_Device, const Info& a_Info)
{
    std::vector<Image::Handle> images;
    auto win32SwapChain = std::static_pointer_cast<SwapChain::Win32::Impl>(a_Info.oldSwapchain);
    for (auto i = 0u; i < a_Info.imageCount; ++i)
    {
        Image::Handle image;
        if (win32SwapChain != nullptr) {
            const auto& imageInfo = win32SwapChain->images.at(i)->info;
            const bool canRecycle = imageInfo.extent.width == a_Info.imageExtent.width &&
                                    imageInfo.extent.height == a_Info.imageExtent.height &&
                                    imageInfo.arrayLayers == a_Info.imageArrayLayers &&
                                    imageInfo.format == a_Info.imageFormat;
            if (canRecycle) image = win32SwapChain->images.at(i);
        }
        if (image == nullptr) {
            Image::Info imageInfo{};
            imageInfo.type = Image::Type::Image2D;
            imageInfo.extent.width = a_Info.imageExtent.width;
            imageInfo.extent.height = a_Info.imageExtent.height;
            imageInfo.mipLevels = 1;
            imageInfo.arrayLayers = a_Info.imageArrayLayers;
            imageInfo.format = a_Info.imageFormat;
            image = Image::Create(a_Device, imageInfo);
        }
        images.push_back(image);
    }
    return images;
}

Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : SwapChain::Impl(a_Device, a_Info)
    , images(CreateImages(a_Device, a_Info))
{
    const auto pixelSize = GetPixelSize(info.imageFormat);
    pixelData.resize(info.imageExtent.width * info.imageExtent.height * pixelSize / 8, 0);
    if (info.oldSwapchain != nullptr && !info.oldSwapchain->retired) {
        auto win32SwapChain = std::static_pointer_cast<SwapChain::Win32::Impl>(info.oldSwapchain);
        win32SwapChain->workerThread.Wait();
        hdc = win32SwapChain->hdc;
        hglrc = win32SwapChain->hglrc;
        presentShader.swap(win32SwapChain->presentShader);
        info.oldSwapchain->Retire();
        info.oldSwapchain.reset();
    }
    else {
        hdc = GetDC(HWND(a_Info.surface->nativeWindow));
        OpenGL::Win32::Initialize();
        const int attribIList[] = {
            WGL_DRAW_TO_WINDOW_ARB, true,
            WGL_SUPPORT_OPENGL_ARB, true,
            WGL_DOUBLE_BUFFER_ARB,  true,
            WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
            WGL_COLORSPACE_EXT,     WGL_COLORSPACE_SRGB_EXT,
            WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB,     pixelSize,
            0
        };
        int32_t  pixelFormat = 0;
        uint32_t pixelFormatNbr = 0;
        WIN32_CHECK_ERROR(wglChoosePixelFormatARB(HDC(hdc), attribIList, nullptr, 1, &pixelFormat, &pixelFormatNbr));
        WIN32_CHECK_ERROR(pixelFormat != 0);
        WIN32_CHECK_ERROR(pixelFormatNbr != 0);
        WIN32_CHECK_ERROR(SetPixelFormat(HDC(hdc), pixelFormat, nullptr));
        hglrc = OpenGL::Win32::CreateContext(hdc);
    }
    workerThread.PushCommand([this] {
        WIN32_CHECK_ERROR(wglMakeCurrent(HDC(hdc), HGLRC(hglrc)));
        if (info.presentMode == PresentMode::Immediate) {
            WIN32_CHECK_ERROR(wglSwapIntervalEXT(0));
        }
        else WIN32_CHECK_ERROR(wglSwapIntervalEXT(1));
#ifdef _DEBUG
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(MessageCallback, 0);
#endif _DEBUG
        if (presentTexture == nullptr)
            presentTexture.reset(new PresentTexture(images.front()));
        if (presentShader == nullptr)
            presentShader.reset(new PresentShader);
        if (presentGeometry == nullptr)
            presentGeometry.reset(new PresentGeometry);
        glViewport(0, 0, presentTexture->extent.width, presentTexture->extent.height);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        presentGeometry->Bind();
        presentShader->Bind();
        glActiveTexture(GL_TEXTURE0);
        presentTexture->Bind();
        glBindSampler(0, presentTexture->samplerHandle);
    }, false);
}

Impl::~Impl()
{
    retired = true;
    workerThread.PushCommand([this] {
        presentTexture.reset();
        presentShader.reset();
        presentGeometry.reset();
        wglMakeCurrent(nullptr, nullptr);
    }, true);
    if (hdc != nullptr)
        ReleaseDC(HWND(info.surface->nativeWindow), HDC(hdc));
    if (hglrc != nullptr)
        wglDeleteContext(HGLRC(hglrc));
    hdc = nullptr;
    hglrc = nullptr;
}

void Impl::Retire() {
    SwapChain::Impl::Retire();
    workerThread.PushCommand([this] {
        presentShader.reset();
        presentTexture.reset();
        presentGeometry.reset();
        wglMakeCurrent(nullptr, nullptr);
    }, true);
    images.clear();
    hdc = nullptr;
    hglrc = nullptr;
}

void Impl::Present(const Queue::Handle& a_Queue)
{
    assert(!retired);
    const auto& image = images.at(backBufferIndex);
    const auto extent = image->info.extent;
    workerThread.PushCommand([this, queue = a_Queue, extent] {
        queue->PushCommand([this] {
            const auto& image = images.at(backBufferIndex);
            glBindTexture(image->target, image->handle);
            glGetTexImage(
                image->target,
                0,
                image->dataFormat,
                image->dataType,
                pixelData.data());
            glBindTexture(image->target, 0);
        }, true);
        presentTexture->UploadData(pixelData);
        presentGeometry->Draw();
        WIN32_CHECK_ERROR(SwapBuffers(HDC(hdc)));
    }, info.presentMode != PresentMode::Immediate);
    backBufferIndex = (backBufferIndex + 1) % info.imageCount;
}

//TODO: implement a timeout
Image::Handle Impl::AcquireNextImage(
    const std::chrono::nanoseconds& a_Timeout,
    const Queue::Semaphore::Handle& a_Semaphore,
    const Queue::Fence::Handle& a_Fence)
{
    workerThread.Wait();
    //We do not need to synchronize with the GPU for real here
    if (a_Semaphore != nullptr) {
        if (a_Semaphore->type == Queue::Semaphore::Type::Binary)
            std::static_pointer_cast<Queue::Semaphore::Binary>(a_Semaphore)->SignalNoSync();
        else throw std::runtime_error("Cannot wait on Timeline Semaphores when presenting");
    }
    if (a_Fence != nullptr) a_Fence->SignalNoSync();
    return images.at(backBufferIndex);
}
static inline auto CheckShaderCompilation(GLuint a_Shader)
{
    GLint result;
    glGetShaderiv(a_Shader, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
        GLsizei length{ 0 };
        glGetShaderiv(a_Shader, GL_INFO_LOG_LENGTH, &length);
        if (length > 1) {
            std::vector<char> infoLog(length, 0);
            glGetShaderInfoLog(a_Shader, length, nullptr, infoLog.data());
            std::string logString(infoLog.begin(), infoLog.end());
            throw std::runtime_error(logString);
        }
        else throw std::runtime_error("Unknown Error");
        return false;
    }
    return true;
}
PresentShader::PresentShader()
{
    const auto vertCode =
        "#version 430                                       \n"
        "out vec2 UV;                                       \n"
        "void main() {                                      \n"
        "   float x = -1.0 + float((gl_VertexID & 1) << 2); \n"
        "   float y = -1.0 + float((gl_VertexID & 2) << 1); \n"
        "   UV.x = (x + 1.0) * 0.5;                         \n"
        "   UV.y = 1 - (y + 1.0) * 0.5;                     \n"
        "   gl_Position = vec4(x, y, 0, 1);                 \n"
        "}                                                  \n"
    ;
    const int vertLen = strlen(vertCode);
    const auto vertHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertHandle, 1, &vertCode, &vertLen);
    glCompileShader(vertHandle);
    CheckShaderCompilation(vertHandle);
    
    const auto fragCode =
        "#version 430                                       \n"
        "layout(location = 0) out vec4 out_Color;           \n"
        "layout(binding = 0) uniform sampler2D in_Color;    \n"
        "in vec2 UV;                                        \n"
        "void main() {                                      \n"
        "   out_Color = texture(in_Color, UV);              \n"
        "}                                                  \n"
    ;
    const int fragLen = strlen(fragCode);
    const auto fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragHandle, 1, &fragCode, &fragLen);
    glCompileShader(fragHandle);
    CheckShaderCompilation(fragHandle);

    handle = glCreateProgram();
    glAttachShader(handle, vertHandle);
    glAttachShader(handle, fragHandle);
    glLinkProgram(handle);
    glDetachShader(handle, vertHandle);
    glDetachShader(handle, fragHandle);
    glDeleteShader(vertHandle);
    glDeleteShader(fragHandle);
}
PresentShader::~PresentShader()
{
    glDeleteProgram(handle);
}
void PresentShader::Bind() const
{
    glUseProgram(handle);
}
void PresentShader::Unbind() const
{
    glUseProgram(0);
}
PresentTexture::PresentTexture(const Image::Handle& a_FromImage)
    : target(a_FromImage->target)
    , dataType(a_FromImage->dataType)
    , dataFormat(a_FromImage->dataFormat)
    , internalFormat(a_FromImage->internalFormat)
    , extent(a_FromImage->info.extent)
{
    glGenSamplers(1, &samplerHandle);
    glGenTextures(1, &handle);
    glBindTexture(target, handle);
    glTexStorage2D(target, 1, internalFormat, extent.width, extent.height);
    glBindTexture(target, 0);
}
PresentTexture::~PresentTexture()
{
    glDeleteSamplers(1, &samplerHandle);
    glDeleteTextures(1, &handle);
}
void PresentTexture::Bind() const
{
    glBindTexture(target, handle);
}

void PresentTexture::Unbind() const
{
    glBindTexture(target, 0);
}
void PresentTexture::UploadData(const std::vector<unsigned char>& a_Data) const
{
    glTexSubImage2D(
        target, 0,
        0, 0, extent.width, extent.height,
        dataFormat, dataType, a_Data.data());
}
PresentGeometry::PresentGeometry()
{
    glGenBuffers(1, &VBOhandle);
    glBindBuffer(GL_ARRAY_BUFFER, VBOhandle);
    glBufferStorage(GL_ARRAY_BUFFER, 3, nullptr, 0);
    glGenVertexArrays(1, &VAOhandle);
    glBindVertexArray(VAOhandle);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_BYTE, false, 1, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
PresentGeometry::~PresentGeometry()
{
    glDeleteBuffers(1, &VBOhandle);
    glDeleteVertexArrays(1, &VAOhandle);
}
void PresentGeometry::Bind() const
{
    glBindVertexArray(VAOhandle);
}
void PresentGeometry::Unbind() const
{
    glBindVertexArray(0);
}
void PresentGeometry::Draw() const
{
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
}
