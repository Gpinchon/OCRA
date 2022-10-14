#include <Common.hpp>
#include <Texture.hpp>

#include <Command/Pool.hpp>
#include <Command/Buffer.hpp>
#include <Common/Vec3.hpp>
#include <Common/Vec4.hpp>
#include <Device.hpp>
#include <Image/Image.hpp>
#include <Instance.hpp>
#include <Memory.hpp>
#include <Queue/Fence.hpp>
#include <Shader/Module.hpp>
#include <Shader/Stage.hpp>
#include <Surface.hpp>
#include <SwapChain.hpp>

#include <ShaderCompiler/Compiler.hpp>
#include <ShaderCompiler/Shader.hpp>

#include <Windows.h>

using namespace OCRA;

#define SWAPCHAIN_IMAGE_NBR 2

Vec3 HSVtoRGB(float fH, float fS, float fV) {
    float fC = fV * fS; // Chroma
    float fHPrime = fmod(fH / 60.0, 6);
    float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
    float fM = fV - fC;
    float fR, fG, fB;

    if (0 <= fHPrime && fHPrime < 1) {
        fR = fC;
        fG = fX;
        fB = 0;
    }
    else if (1 <= fHPrime && fHPrime < 2) {
        fR = fX;
        fG = fC;
        fB = 0;
    }
    else if (2 <= fHPrime && fHPrime < 3) {
        fR = 0;
        fG = fC;
        fB = fX;
    }
    else if (3 <= fHPrime && fHPrime < 4) {
        fR = 0;
        fG = fX;
        fB = fC;
    }
    else if (4 <= fHPrime && fHPrime < 5) {
        fR = fX;
        fG = 0;
        fB = fC;
    }
    else if (5 <= fHPrime && fHPrime < 6) {
        fR = fC;
        fG = 0;
        fB = fX;
    }
    else {
        fR = 0;
        fG = 0;
        fB = 0;
    }

    fR += fM;
    fG += fM;
    fB += fM;
    return { fR, fG, fB };
}

SwapChain::Handle CreateSwapChain(const Device::Handle& a_Device, const Surface::Handle& a_Surface, const SwapChain::Handle& a_OldSwapChain, const uint32_t& a_Width, const uint32_t& a_Height, const uint32_t& a_ImageNbr)
{
    SwapChain::Info info{};
    info.oldSwapchain = a_OldSwapChain;
    info.presentMode = SwapChain::PresentMode::Immediate;
    info.imageColorSpace = Image::ColorSpace::sRGB;
    info.imageFormat = Image::Format::Uint8_Normalized_RGBA;
    info.imageCount = a_ImageNbr;
    info.surface = a_Surface;
    info.imageExtent.width = a_Width;
    info.imageExtent.height = a_Height;
    return SwapChain::Create(a_Device, info);
}

Surface::Handle CreateSurface(const Instance::Handle& a_Instance, void* const a_HINSTANCE, void* const a_HWND)
{
    Surface::Win32::Info info{};
    info.hinstance = a_HINSTANCE;
    info.hwnd = a_HWND;
    return Surface::Win32::Create(a_Instance, info);
}

struct TexturesTestApp : TestApp
{
    TexturesTestApp()
        : TestApp("Test_Textures")
        , window(Window(name, 1280, 720))
        , surface(CreateSurface(instance, GetModuleHandle(0), (void*)window.nativeHandle))
        , physicalDevice(Instance::EnumeratePhysicalDevices(instance).front())
        , device(CreateDevice(physicalDevice))
    {
        window.OnResize = [this](const Window&, const uint32_t a_Width, const uint32_t a_Height) {
            render = true;
            //IMPORTANT, when destroyed, swapchain switches to windowed mode
            //If a resize is triggered by this switch it will result in a crash !
            if (!close) OnResize(a_Width, a_Height);
        };
        window.OnMaximize = window.OnResize;
        window.OnRestore = window.OnResize;
        window.OnMinimize = [this](const Window&, const uint32_t, const uint32_t) { render = false; };
        window.OnClose = [this](const Window&) { OnClose(); };
        const auto queueFamily = FindQueueFamily(physicalDevice, PhysicalDevice::QueueFlagsBits::Graphics);
        queue = Device::GetQueue(device, queueFamily, 0); //Get first available queue
        imageAcquisitionFence = Queue::Fence::Create(device);
        commandPool = CreateCommandPool(device, queueFamily);
        commandBuffer = CreateCommandBuffer(device, commandPool, Command::Pool::AllocateInfo::Level::Primary);
        CreateShaderStages();
        CreateTextureTranferBuffer();
        FillTextureTransferBuffer();
    }
    void Loop()
    {
        FPSCounter fpsCounter;
        auto printTime = std::chrono::high_resolution_clock::now();
        while (true) {
            fpsCounter.StartFrame();
            window.PushEvents();
            if (close) break;
            const auto swapChainImage = SwapChain::AcquireNextImage(device, swapChain, std::chrono::nanoseconds(15000000), nullptr, imageAcquisitionFence);
            Queue::Fence::WaitFor(device, imageAcquisitionFence, std::chrono::nanoseconds(15000000));
            Queue::Fence::Reset(device, { imageAcquisitionFence });
            RecordClearCommandBuffer(swapChainImage);
            SubmitCommandBuffer(queue, commandBuffer);
            SwapChain::Present(queue, presentInfo);
            const auto now = std::chrono::high_resolution_clock::now();
            fpsCounter.EndFrame();
            if (std::chrono::duration<double, std::milli>(now - printTime).count() >= 48) {
                printTime = now;
                fpsCounter.Print();
            }
        }
    }
    void CreateShaderStages()
    {
        const auto compiler = ShaderCompiler::Create();
        {
            ShaderCompiler::Shader::Info shaderInfo;
            shaderInfo.type = ShaderCompiler::Shader::Type::Vertex;
            shaderInfo.entryPoint = "main";
            shaderInfo.source = {
                "#version 450                                               \n"
                "layout(location = 0) in vec2 inPosition;                   \n"
                "layout(location = 1) in vec3 inColor;                      \n"
                "layout(binding = 0) uniform Projection {                   \n"
                "   mat4 matrix;                                            \n"
                "} proj;                                                    \n"
                "void main() {                                              \n"
                "   gl_Position = proj.matrix * vec4(inPosition, 0.0, 1.0); \n"
                "}                                                          \n"
            };
            const auto vertexShader = ShaderCompiler::Shader::Create(compiler, shaderInfo);
            Shader::Module::Info shaderModuleInfo;
            shaderModuleInfo.code = ShaderCompiler::Shader::Compile(vertexShader);
            const auto shaderModule = Shader::Module::Create(device, shaderModuleInfo);
            Shader::Stage::Info shaderStageInfo;
            shaderStageInfo.entryPoint = "main";
            shaderStageInfo.stage = Shader::Stage::StageFlagBits::Vertex;
            shaderStageInfo.module = shaderModule;
            shaderStages.push_back(Shader::Stage::Create(device, shaderStageInfo));
        }
        {
            ShaderCompiler::Shader::Info shaderInfo;
            shaderInfo.type = ShaderCompiler::Shader::Type::Fragment;
            shaderInfo.entryPoint = "main";
            shaderInfo.source = {
                "#version 450                                         \n"
                "layout(binding = 0) uniform sampler2D uTexture;      \n"
                "layout(location = 0) out vec4 outColor;              \n"
                "void main() {                                        \n"
                "    vec4 color = texture(uTexture, gl_FragCoord.xy); \n"
                "    outColor = vec4(color);                          \n"
                "}                                                    \n"
            };
            const auto fragmentShader = ShaderCompiler::Shader::Create(compiler, shaderInfo);
            Shader::Module::Info shaderModuleInfo;
            shaderModuleInfo.code = ShaderCompiler::Shader::Compile(fragmentShader);
            const auto shaderModule = Shader::Module::Create(device, shaderModuleInfo);
            Shader::Stage::Info shaderStageInfo;
            shaderStageInfo.entryPoint = "main";
            shaderStageInfo.stage = Shader::Stage::StageFlagBits::Fragment;
            shaderStageInfo.module = shaderModule;
            shaderStages.push_back(Shader::Stage::Create(device, shaderStageInfo));
        }
    }
    void CreateTextureTranferBuffer()
    {
        const auto textureSize = texture.GetWidth() * texture.GetHeight() * Image::GetPixelSize(texture.GetFormat());
        Buffer::Info bufferInfo;
        bufferInfo.size = textureSize;
        bufferInfo.usage = Buffer::UsageFlagBits::TransferSrc;
        textureTransferBuffer = Buffer::Create(device, bufferInfo);
        textureTransferMemory = AllocateMemory(
            physicalDevice, device,
            textureSize,
            PhysicalDevice::MemoryPropertyFlagBits::HostVisible);
        Buffer::BindMemory(device, textureTransferBuffer, textureTransferMemory, 0);
    }
    void FillTextureTransferBuffer() {
        const auto textureSize = texture.GetWidth() * texture.GetHeight() * Image::GetPixelSize(texture.GetFormat());
        Memory::MappedRange range;
        range.memory = textureTransferMemory;
        range.length = textureSize;
        range.offset = 0;
        auto bufferPtr = (Vec<4, uint8_t>*)Memory::Map(device, range);
        for (uint32_t x = 0; x < texture.GetWidth(); ++x) {
            for (uint32_t y = 0; y < texture.GetHeight(); ++y) {
                const auto index = x + y * texture.GetWidth();
                bufferPtr[index].r = 255;
                bufferPtr[index].g = (y % 2) ? 255 : 0;
                bufferPtr[index].b = 125;
                bufferPtr[index].a = 255;
            }
        }
        Memory::Unmap(device, textureTransferMemory);
    }
    void RecordClearCommandBuffer(const Image::Handle& a_Image)
    {
        static float hue = 0;
        static auto lastTime = std::chrono::high_resolution_clock::now();
        const auto now = std::chrono::high_resolution_clock::now();
        const auto delta = std::chrono::duration<double, std::milli>(now - lastTime).count();
        lastTime = now;
        hue += 0.05 * delta;
        hue = hue > 360 ? 0 : hue;
        const auto color = HSVtoRGB(hue, 0.5f, 1.f);
        Command::Buffer::BeginInfo bufferBeginInfo;
        bufferBeginInfo.flags = Command::Buffer::UsageFlagBits::None;
        Command::Buffer::Reset(commandBuffer);
        Command::Buffer::Begin(commandBuffer, bufferBeginInfo);
        {
            Command::BufferImageCopy copy;
            copy.bufferOffset = 0;
            copy.imageExtent.width = texture.GetWidth();
            copy.imageExtent.height = texture.GetHeight();
            copy.imageExtent.depth = 1;
            Command::CopyBufferToImage(commandBuffer, textureTransferBuffer, texture.GetImage(), { copy });
            ColorValue clearColor{ color.r, color.g, color.b, 1.f };
            Image::Subresource::Range range{};
            range.levelCount = 1;
            Command::ClearColorImage(commandBuffer, a_Image, Image::Layout::Unknown, clearColor, { range });
        }
        Command::Buffer::End(commandBuffer);
    }
    void OnResize(const uint32_t a_Width, const uint32_t a_Height)
    {
        swapChain = CreateSwapChain(device, surface, swapChain, a_Width, a_Height, 2);
        presentInfo.swapChains = { swapChain };
        extent = { a_Width, a_Height };
    }
    void OnClose()
    {
        close = true;
    }
    bool                    render{ false };
    bool                    close{ false };
    Window                  window;
    uExtent2D               extent;
    Surface::Handle         surface;
    PhysicalDevice::Handle  physicalDevice;
    Device::Handle          device;
    SwapChain::PresentInfo  presentInfo;
    SwapChain::Handle       swapChain;
    Queue::Handle           queue;
    Queue::Fence::Handle    imageAcquisitionFence;
    Command::Pool::Handle   commandPool;
    Command::Buffer::Handle commandBuffer;
    Texture2D               texture{ device, Image::Format::Uint8_Normalized_RGBA, 16, 16, 1 };
    Memory::Handle          textureTransferMemory;
    Buffer::Handle          textureTransferBuffer;
    std::vector<Shader::Stage::Handle>   shaderStages;
};

int main()
{
    int ret = 0;
    TexturesTestApp testApp;
    testApp.window.Show();
    testApp.Loop();
	return ret;
}
