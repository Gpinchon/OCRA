#include <Common.hpp>

#include <Instance.hpp>
#include <Device.hpp>
#include <FrameBuffer.hpp>
#include <Memory.hpp>
#include <RenderPass.hpp>
#include <Surface.hpp>
#include <SwapChain.hpp>
#include <Image/Image.hpp>
#include <Image/View.hpp>
#include <Command/Pool.hpp>
#include <Command/Buffer.hpp>
#include <Command/Draw.hpp>
#include <Command/Scissor.hpp>
#include <Command/ViewPort.hpp>
#include <Queue/Fence.hpp>
#include <Common/Vec3.hpp>

#include <ShaderCompiler/Compiler.hpp>
#include <ShaderCompiler/Shader.hpp>

#include <Windows.h>

using namespace OCRA;

#include <Pipeline/Graphics.hpp>

struct Vertex {
    Vec2 pos;
    Vec3 color;
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
        attribs.at(0).format.size = 2;
        attribs.at(0).format.normalized = false;
        attribs.at(0).format.type = VertexType::Float32;
        attribs.at(0).offset = offsetof(Vertex, pos);
        attribs.at(1).binding = 0;
        attribs.at(1).location = 1;
        attribs.at(1).format.size = 3;
        attribs.at(1).format.normalized = true;
        attribs.at(1).format.type = VertexType::Float32;
        attribs.at(1).offset = offsetof(Vertex, color);
        return attribs;
    }
};

struct GraphicsPipelineTestApp : TestApp
{
    GraphicsPipelineTestApp()
        : TestApp("Test_GraphicsPipeline")
        , window(Window("Test_GraphicsPipeline", 1280, 720))
        , surface(CreateSurface(instance, GetModuleHandle(0), (void*)window.nativeHandle))
        , physicalDevice(Instance::EnumeratePhysicalDevices(instance).front())
        , device(CreateDevice(physicalDevice))
    {
        window.OnResize = [this](const Window&, const uint32_t a_Width, const uint32_t a_Height) {
            OnResize(a_Width, a_Height);
        };
        window.OnClose = [this](const Window&) {
            OnClose();
        };
        CreateShaderStages();
        const auto queueFamily = FindQueueFamily(physicalDevice, PhysicalDevice::QueueFlagsBits::Graphics);
        queue = Device::GetQueue(device, queueFamily, 0); //Get first available queue
        commandPool = CreateCommandPool(device, queueFamily);
        renderPass = CreateRenderPass();
        imageAcquisitionFence = Queue::Fence::Create(device);
        mainCommandBuffer = CreateCommandBuffer(device, commandPool, Command::Pool::AllocateInfo::Level::Primary);
        vertexBuffer = CreateVertexBuffer();
        vertexBufferMemory = AllocateVertexBuffer();
        Buffer::BindMemory(device, vertexBuffer, vertexBufferMemory, 0);
        FillVertexBuffer();
    }
    void Loop()
    {
        while (true) {
            window.PushEvents();
            if (close) break;
            swapChainImage = SwapChain::AcquireNextImage(device, swapChain, std::chrono::nanoseconds(15000000), nullptr, imageAcquisitionFence);
            Queue::Fence::WaitFor(device, imageAcquisitionFence, std::chrono::nanoseconds(15000000));
            Queue::Fence::Reset(device, { imageAcquisitionFence });
            RecordMainCommandBuffer();
            SubmitCommandBuffer(queue, mainCommandBuffer);
            SwapChain::Present(queue, presentInfo);
        }
    }
    Buffer::Handle CreateVertexBuffer() {
        Buffer::Info info;
        info.size  = sizeof(vertices.front()) * vertices.size();
        info.usage = Buffer::UsageFlagBits::VertexBuffer;
        return Buffer::Create(device, info);
    }
    Memory::Handle AllocateVertexBuffer() {
        return AllocateMemory(
            physicalDevice, device,
            sizeof(vertices.front()) * vertices.size(),
            PhysicalDevice::MemoryPropertyFlagBits::HostVisible);
    }
    void FillVertexBuffer() {
        Memory::MappedRange range;
        range.memory = vertexBufferMemory;
        range.length = sizeof(vertices.front()) * vertices.size();
        range.offset = 0;
        auto bufferPtr = Memory::Map(device, range);
        std::memcpy(bufferPtr, vertices.data(), sizeof(vertices.front()) * vertices.size());
        Memory::Unmap(device, vertexBufferMemory);
    }
    RenderPass::Handle CreateRenderPass()
    {
        RenderPass::Info renderPassInfo{};
        RenderPass::ColorAttachmentDescription colorAttachment;
        colorAttachment.location = 0;
        colorAttachment.loadOp = RenderPass::LoadOperation::Clear; //clear the attachment 0 on begin
        colorAttachment.storeOp = RenderPass::StoreOperation::Store;
        renderPassInfo.colorAttachments = { colorAttachment };
        return RenderPass::Create(device, renderPassInfo);
    }
    void CreateShaderStages()
    {
        const auto compiler = ShaderCompiler::Create();
        {
            ShaderCompiler::Shader::Info shaderInfo;
            shaderInfo.type = ShaderCompiler::Shader::Type::Vertex;
            shaderInfo.entryPoint = "main";
            shaderInfo.source =
                "#version 450                                 \n"
                "layout(location = 0) in vec2 inPosition;     \n"
                "layout(location = 1) in vec3 inColor;        \n"
                "layout(location = 0) out vec3 fragColor;     \n"
                "void main() {                                \n"
                "   gl_Position = vec4(inPosition, 0.0, 1.0); \n"
                "   fragColor = inColor;                      \n"
                "}                                            \n";
            const auto vertexShader = ShaderCompiler::Shader::Create(compiler, shaderInfo);
            Shader::Module::Info shaderModuleInfo;
            shaderModuleInfo.code = ShaderCompiler::Shader::Compile(vertexShader);
            const auto shaderModule = Shader::Module::Create(device, shaderModuleInfo);
            Shader::Stage::Info shaderStageInfo;
            shaderStageInfo.name = "main";
            shaderStageInfo.stage = Shader::Stage::StageFlagBits::Vertex;
            shaderStageInfo.module = shaderModule;
            shaderStages.push_back(Shader::Stage::Create(device, shaderStageInfo));
        }
        {
            ShaderCompiler::Shader::Info shaderInfo;
            shaderInfo.type = ShaderCompiler::Shader::Type::Fragment;
            shaderInfo.entryPoint = "main";
            shaderInfo.source =
                "#version 450                            \n"
                "layout(location = 0) out vec4 outColor; \n"
                "layout(location = 0) in vec3 fragColor; \n"
                "void main() {                           \n"
                "    outColor = vec4(fragColor, 1.0);    \n"
                "}                                       \n";
            const auto fragmentShader = ShaderCompiler::Shader::Create(compiler, shaderInfo);
            Shader::Module::Info shaderModuleInfo;
            shaderModuleInfo.code = ShaderCompiler::Shader::Compile(fragmentShader);
            const auto shaderModule = Shader::Module::Create(device, shaderModuleInfo);
            Shader::Stage::Info shaderStageInfo;
            shaderStageInfo.name = "main";
            shaderStageInfo.stage = Shader::Stage::StageFlagBits::Fragment;
            shaderStageInfo.module = shaderModule;
            shaderStages.push_back(Shader::Stage::Create(device, shaderStageInfo));
        }
        
    }
    Pipeline::Handle CreateGraphicsPipeline()
    {
        Pipeline::Graphics::Info graphicsPipelineInfo{};
        ViewPort viewport;
        viewport.rect.offset = { 0, 0 };
        viewport.rect.extent = extent;
        viewport.depthRange = { 0, 1 };
        Rect2D  scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = extent;
        graphicsPipelineInfo.renderPass = renderPass;
        graphicsPipelineInfo.inputAssemblyState.topology = Primitive::Topology::TriangleList;
        graphicsPipelineInfo.inputAssemblyState.primitiveRestartEnable = false;
        graphicsPipelineInfo.viewPortState.viewPorts = { viewport };
        graphicsPipelineInfo.viewPortState.scissors = { scissor };
        graphicsPipelineInfo.vertexInputState.attributeDescriptions = Vertex::GetAttributeDescription();
        graphicsPipelineInfo.vertexInputState.bindingDescriptions   = Vertex::GetBindingDescriptions();
        graphicsPipelineInfo.shaderPipelineState.stages = shaderStages;
        //Everything else is left by default for now
        return Pipeline::Graphics::Create(device, graphicsPipelineInfo);
    }
    void CreateFrameBuffer()
    {
        {
            Image::Info imageInfo;
            imageInfo.type = Image::Type::Image2D;
            imageInfo.extent.width = extent.width;
            imageInfo.extent.height = extent.height;
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
            frameBufferInfo.extent.width = extent.width;
            frameBufferInfo.extent.height = extent.height;
            frameBufferInfo.renderPass = renderPass;
            frameBuffer = FrameBuffer::Create(device, frameBufferInfo);
        }
    }
    void RecordMainCommandBuffer()
    {
        Command::Buffer::BeginInfo bufferBeginInfo{};
        bufferBeginInfo.flags = Command::Buffer::UsageFlagBits::None;
        Command::Buffer::Reset(mainCommandBuffer);
        Command::Buffer::Begin(mainCommandBuffer, bufferBeginInfo);
        {
            Command::RenderPassBeginInfo renderPassBeginInfo{};
            renderPassBeginInfo.renderPass = renderPass;
            renderPassBeginInfo.framebuffer = frameBuffer;
            renderPassBeginInfo.renderArea.offset = { 0, 0 };
            renderPassBeginInfo.renderArea.extent = extent;
            renderPassBeginInfo.colorClearValues = { {0.8118, 0.7294, 0.5137, 1} };
            Command::BeginRenderPass(mainCommandBuffer, renderPassBeginInfo, Command::SubPassContents::Inline);
            {
                Command::BindPipeline(mainCommandBuffer, Pipeline::BindingPoint::Graphics, graphicsPipeline);
                Command::BindVertexBuffers(mainCommandBuffer, 0, { vertexBuffer }, { 0 });
                Command::Draw(mainCommandBuffer, vertices.size(), 1, 0, 0);
                //draw stuff here
            }
            Command::EndRenderPass(mainCommandBuffer);
            {
                Command::ImageCopy imageCopy;
                imageCopy.extent.width  = extent.width;
                imageCopy.extent.height = extent.height;
                imageCopy.extent.depth  = 1;
                Command::CopyImage(
                    mainCommandBuffer,
                    frameBufferImage,
                    swapChainImage,
                    { imageCopy });
            }
            
        }
        Command::Buffer::End(mainCommandBuffer);
    }
    void OnResize(const uint32_t a_Width, const uint32_t a_Height)
    {
        extent = { a_Width, a_Height };
        swapChain = CreateSwapChain(device, surface, swapChain, a_Width, a_Height, 2);
        graphicsPipeline = CreateGraphicsPipeline();
        presentInfo.swapChains = { swapChain };
        CreateFrameBuffer();
    }
    void OnClose()
    {
        close = true;
    }
    bool                    close{ false };
    Window                  window;
    uExtent2D               extent{ 0, 0 };
    Surface::Handle         surface;
    PhysicalDevice::Handle  physicalDevice;
    Device::Handle          device;
    RenderPass::Handle      renderPass;
    FrameBuffer::Handle     frameBuffer;
    Image::Handle           frameBufferImage;
    Pipeline::Handle        graphicsPipeline;
    SwapChain::Handle       swapChain;
    Image::Handle           swapChainImage;
    SwapChain::PresentInfo  presentInfo;
    Command::Pool::Handle   commandPool;
    Command::Buffer::Handle mainCommandBuffer;
    Queue::Handle           queue;
    Queue::Fence::Handle    imageAcquisitionFence;
    const std::vector<Vertex> vertices = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };
    Buffer::Handle          vertexBuffer;
    Memory::Handle          vertexBufferMemory;
    std::vector<Shader::Stage::Handle>   shaderStages;
};

int GraphicsPipeline()
{
    int ret = 0;
    //basic setup as usual
    GraphicsPipelineTestApp testApp;
    testApp.window.Show();
    testApp.Loop();
    return ret;
}
