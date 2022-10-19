#include <Common.hpp>
#include <Window.hpp>
#include <Texture.hpp>

#include <Command/Buffer.hpp>
#include <Command/Pool.hpp>
#include <Command/Draw.hpp>
#include <Common/Rect2D.hpp>
#include <Common/Vec2.hpp>
#include <Common/Vec3.hpp>
#include <Common/Vec4.hpp>
#include <Common/ViewPort.hpp>
#include <Descriptor/Set.hpp>
#include <Device.hpp>
#include <FrameBuffer.hpp>
#include <Image/Image.hpp>
#include <Image/View.hpp>
#include <Instance.hpp>
#include <Memory.hpp>
#include <Pipeline/Graphics.hpp>
#include <Pipeline/Layout.hpp>
#include <Pipeline/VertexInputState.hpp>
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

struct Vertex {
    Vec2 pos;
    Vec2 UV;
    static auto GetBindingDescriptions() {
        std::vector<Pipeline::VertexInputState::BindingDescription> bindings(1);
        bindings.at(0).binding = 0;
        bindings.at(0).stride = sizeof(Vertex);
        bindings.at(0).inputRate = Pipeline::VertexInputState::BindingDescription::InputRate::Vertex;
        return bindings;
    }
    static auto GetAttributeDescription() {
        std::vector<Pipeline::VertexInputState::AttributeDescription> attribs(2);
        attribs.at(0).binding = 0;
        attribs.at(0).location = 0;
        attribs.at(0).format.size = decltype(pos)::length();
        attribs.at(0).format.normalized = false;
        attribs.at(0).format.type = VertexType::Float32;
        attribs.at(0).offset = offsetof(Vertex, pos);
        attribs.at(1).binding = 0;
        attribs.at(1).location = 1;
        attribs.at(1).format.size = decltype(UV)::length();
        attribs.at(1).format.normalized = false;
        attribs.at(1).format.type = VertexType::Float32;
        attribs.at(1).offset = offsetof(Vertex, UV);
        return attribs;
    }
};

struct TexturesTestApp : TestApp
{
    TexturesTestApp()
        : TestApp("Test_Textures")
        , physicalDevice(Instance::EnumeratePhysicalDevices(instance).front())
        , device(CreateDevice(physicalDevice))
        , window(Window(instance, physicalDevice, device, name, 1280, 720))
    {
        window.OnResize = [this](const Window&, const uint32_t, const uint32_t) {
            render = true;
            if (!window.IsClosing()) {
                CreateFrameBuffer();
                CreateGraphicsPipeline();
            }
        };
        window.OnMaximize = window.OnResize;
        window.OnRestore = window.OnResize;
        window.OnMinimize = [this](const Window&, const uint32_t, const uint32_t) { render = false; };
        const auto queueFamily = FindQueueFamily(physicalDevice, PhysicalDevice::QueueFlagsBits::Graphics);
        queue = Device::GetQueue(device, queueFamily, 0); //Get first available queue
        imageAcquisitionFence = Queue::Fence::Create(device);
        commandPool = CreateCommandPool(device, queueFamily);
        mainCommandBuffer = CreateCommandBuffer(device, commandPool, Command::Pool::AllocateInfo::Level::Primary);
        drawCommandBuffer = CreateCommandBuffer(device, commandPool, Command::Pool::AllocateInfo::Level::Secondary);
        CreateRenderPass();
        CreateShaderStages();
        CreateTextureTranferBuffer();
        FillTextureTransferBuffer();
    }
    void Loop()
    {
        FPSCounter fpsCounter;
        auto lastTime = std::chrono::high_resolution_clock::now();
        auto printTime = std::chrono::high_resolution_clock::now();
        while (true) {
            fpsCounter.StartFrame();
            window.PushEvents();
            if (window.IsClosing()) break;
            if (!render) break;
            const auto now = std::chrono::high_resolution_clock::now();
            const auto delta = std::chrono::duration<double, std::milli>(now - lastTime).count();
            lastTime = now;
            swapChainImage = window.AcquireNextImage(std::chrono::nanoseconds(15000000), nullptr, imageAcquisitionFence);
            Queue::Fence::WaitFor(device, imageAcquisitionFence, std::chrono::nanoseconds(15000000));
            Queue::Fence::Reset(device, { imageAcquisitionFence });
            RecordMainCommandBuffer(delta);
            SubmitCommandBuffer(queue, mainCommandBuffer);
            window.Present(queue);
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
                "#version 450                                       \n"
                "layout(location = 0) out vec2 UV;                  \n"
                "void main() {                                      \n"
                "   float x = -1.0 + float((gl_VertexIndex & 1) << 2); \n"
                "   float y = -1.0 + float((gl_VertexIndex & 2) << 1); \n"
                "   UV.x = (x + 1.0) * 0.5;                         \n"
                "   UV.y = (y + 1.0) * 0.5;                         \n"
                "   gl_Position = vec4(x, y, 0, 1);                 \n"
                "}                                                  \n"
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
                "layout(location = 0) in vec2 UV;                     \n"             
                "layout(location = 0) out vec4 outColor;              \n"
                "void main() {                                        \n"
                "    vec4 color = texture(uTexture, UV);              \n"
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
        Buffer::Info bufferInfo;
        bufferInfo.size = texture.GetWidth() * texture.GetHeight() * Image::GetPixelSize(texture.GetImageInfo().format);
        bufferInfo.usage = Buffer::UsageFlagBits::TransferSrc;
        textureTransferBuffer = Buffer::Create(device, bufferInfo);
        textureTransferMemory = AllocateMemory(
            physicalDevice, device,
            bufferInfo.size,
            PhysicalDevice::MemoryPropertyFlagBits::HostVisible);
        Buffer::BindMemory(device, textureTransferBuffer, textureTransferMemory, 0);
    }
    void FillTextureTransferBuffer() {
        const auto textureSize = texture.GetWidth() * texture.GetHeight() * Image::GetPixelSize(texture.GetImageInfo().format);
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
    void CreateFrameBuffer()
    {
        {
            Image::Info imageInfo;
            imageInfo.type = Image::Type::Image2D;
            imageInfo.extent.width = window.GetExtent().width;
            imageInfo.extent.height = window.GetExtent().height;
            imageInfo.format = Image::Format::Uint8_Normalized_RGBA;
            imageInfo.mipLevels = 1;
            frameBufferImage = Image::Create(device, imageInfo);
        }
        Image::View::Handle imageView{};
        {
            Image::View::Info imageViewInfo;
            imageViewInfo.image = frameBufferImage;
            imageViewInfo.format = Image::Format::Uint8_Normalized_RGBA;
            imageViewInfo.type = Image::View::Type::View2D;
            imageViewInfo.subRange.layerCount = 1;
            imageView = Image::View::Create(device, imageViewInfo);
        }
        {
            FrameBuffer::Info frameBufferInfo{};
            frameBufferInfo.attachments.push_back(imageView);
            frameBufferInfo.extent.depth = 1;
            frameBufferInfo.extent.width = window.GetExtent().width;
            frameBufferInfo.extent.height = window.GetExtent().height;
            frameBufferInfo.renderPass = renderPass;
            frameBuffer = FrameBuffer::Create(device, frameBufferInfo);
        }
    }
    void CreateGraphicsPipeline()
    {
        ViewPort viewport;
        viewport.rect.offset = { 0, 0 };
        viewport.rect.extent = window.GetExtent();
        viewport.depthRange = { 0, 1 };
        Rect2D  scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = window.GetExtent();
        Pipeline::Layout::Handle layout;
        {
            Pipeline::Layout::Info layoutInfo;
            layoutInfo.setLayouts.push_back(texture.GetDescriptorSetLayout());
            layout = Pipeline::Layout::Create(device, layoutInfo);
        }
        graphicsPipelineInfo.layout = layout;
        graphicsPipelineInfo.rasterizationState.cullMode = Pipeline::RasterizationState::CullMode::None;
        graphicsPipelineInfo.inputAssemblyState.topology = Primitive::Topology::TriangleList;
        graphicsPipelineInfo.inputAssemblyState.primitiveRestartEnable = false;
        graphicsPipelineInfo.viewPortState.viewPorts = { viewport };
        graphicsPipelineInfo.viewPortState.scissors = { scissor };
        graphicsPipelineInfo.vertexInputState.attributeDescriptions = Vertex::GetAttributeDescription();
        graphicsPipelineInfo.vertexInputState.bindingDescriptions = Vertex::GetBindingDescriptions();
        graphicsPipelineInfo.shaderPipelineState.stages = shaderStages;
        graphicsPipelineInfo.renderPass = renderPass;
        graphicsPipelineInfo.subPass = 0;
        //Everything else is left by default for now
        graphicsPipeline = Pipeline::Graphics::Create(device, graphicsPipelineInfo);
    }
    void CreateRenderPass()
    {
        RenderPass::SubPassDescription subPassDescription{};
        RenderPass::AttachmentReference attachmentRef{};
        attachmentRef.location = 0;
        subPassDescription.colorAttachments = { attachmentRef };
        RenderPass::Info renderPassInfo{};
        RenderPass::AttachmentDescription colorAttachment;
        colorAttachment.loadOp = RenderPass::LoadOperation::Clear; //clear the attachment 0 on begin
        colorAttachment.storeOp = RenderPass::StoreOperation::Store; //write the result to attachment 0
        renderPassInfo.colorAttachments = { colorAttachment };
        renderPassInfo.subPasses = { subPassDescription };
        renderPass = RenderPass::Create(device, renderPassInfo);
    }
    void RecordMainCommandBuffer(float a_Delta)
    {
        drawCommandDelta += a_Delta;
        if (drawCommandDelta >= 0.032) {
            RecordDrawCommandBuffer(drawCommandDelta);
            drawCommandDelta = 0;
        }
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
        Command::Buffer::Reset(mainCommandBuffer);
        Command::Buffer::Begin(mainCommandBuffer, bufferBeginInfo);
        //clear background
        {
            ColorValue clearColor{ color.r, color.g, color.b, 1.f };
            Image::Subresource::Range range{};
            range.levelCount = 1;
            Command::ClearColorImage(mainCommandBuffer, swapChainImage, Image::Layout::Unknown, clearColor, { range });
        }
        //fill texture
        {
            Command::BufferImageCopy copy;
            copy.bufferOffset = 0;
            copy.imageExtent.width = texture.GetWidth();
            copy.imageExtent.height = texture.GetHeight();
            copy.imageExtent.depth = 1;
            Command::CopyBufferToImage(mainCommandBuffer, textureTransferBuffer, texture.GetImage(), { copy });
        }
        //draw quad
        {
            Command::ExecuteCommandBuffer(mainCommandBuffer, drawCommandBuffer);
        }
        Command::Buffer::End(mainCommandBuffer);
    }
    void RecordDrawCommandBuffer(const double a_Delta)
    {
        Command::Buffer::BeginInfo bufferBeginInfo{};
        bufferBeginInfo.flags = Command::Buffer::UsageFlagBits::None;
        Command::Buffer::Reset(drawCommandBuffer);
        Command::RenderPassBeginInfo renderPassBeginInfo{};
        renderPassBeginInfo.renderPass = renderPass;
        renderPassBeginInfo.framebuffer = frameBuffer;
        renderPassBeginInfo.renderArea.offset = { 0, 0 };
        renderPassBeginInfo.renderArea.extent = window.GetExtent();
        renderPassBeginInfo.colorClearValues.push_back(ColorValue(0.9529f, 0.6235f, 0.0941f, 1.f));
        Command::Buffer::Begin(drawCommandBuffer, bufferBeginInfo);
        Command::BeginRenderPass(drawCommandBuffer, renderPassBeginInfo, Command::SubPassContents::Inline);
        {
            Command::BindPipeline(drawCommandBuffer, Pipeline::BindingPoint::Graphics, graphicsPipeline);
            Command::BindDescriptorSets(drawCommandBuffer, Pipeline::BindingPoint::Graphics, graphicsPipelineInfo.layout, 0, { texture.GetDescriptorSet() }, {});
            Command::BindVertexBuffers(drawCommandBuffer, 0, { vertexBuffer }, { 0 });
            Command::Draw(drawCommandBuffer, vertices.size(), 1, 0, 0);
        }
        Command::EndRenderPass(drawCommandBuffer);
        Command::Buffer::End(drawCommandBuffer);
    }
    double                  drawCommandDelta{ 0 };
    bool                    render{ false };
    PhysicalDevice::Handle  physicalDevice;
    Device::Handle          device;
    Image::Handle           swapChainImage;
    Window                  window;    

    Queue::Handle           queue;
    Queue::Fence::Handle    imageAcquisitionFence;

    Command::Pool::Handle   commandPool;
    Command::Buffer::Handle mainCommandBuffer;
    Command::Buffer::Handle drawCommandBuffer;

    FrameBuffer::Handle      frameBuffer;
    Image::Handle            frameBufferImage;

    RenderPass::Handle       renderPass;
    Pipeline::Graphics::Info graphicsPipelineInfo;
    Pipeline::Handle         graphicsPipeline;
    std::vector<Shader::Stage::Handle>   shaderStages;

    Texture2D<0>            texture{ device, Image::Format::Uint8_Normalized_RGBA, 16, 16, 1 };
    Image::Sampler::Handle  textureSampler;
    Memory::Handle          textureTransferMemory;
    Buffer::Handle          textureTransferBuffer;

    const std::vector<Vertex> vertices = {
        {{-1.0f,  1.0f}, {1.0f, 0.0f}},
        {{-1.0f, -1.0f}, {0.0f, 1.0f}},
        {{ 1.0f,  1.0f}, {0.0f, 0.0f}},
        {{ 1.0f, -1.0f}, {0.0f, 0.0f}}
    };
    Buffer::Handle          vertexBuffer;
    Memory::Handle          vertexBufferMemory;
};

int main()
{
    int ret = 0;
    TexturesTestApp testApp;
    testApp.window.Show();
    testApp.Loop();
	return ret;
}
