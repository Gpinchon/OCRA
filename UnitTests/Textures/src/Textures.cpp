#include <Common.hpp>
#include <Window.hpp>
#include <UniformTexture.hpp>

#include <OCRA/Command/Buffer.hpp>
#include <OCRA/Command/Pool.hpp>
#include <OCRA/Command/Draw.hpp>
#include <OCRA/Common/Vec_Boolean.hpp>
#include <OCRA/Common/Vec_Interpolation.hpp>
#include <OCRA/Common/Vec_Math.hpp>
#include <OCRA/Common/Vec2.hpp>
#include <OCRA/Common/Vec3.hpp>
#include <OCRA/Common/ViewPort.hpp>
#include <OCRA/Device.hpp>
#include <OCRA/FrameBuffer.hpp>
#include <OCRA/Image/Image.hpp>
#include <OCRA/Image/View.hpp>
#include <OCRA/Instance.hpp>
#include <OCRA/Memory.hpp>
#include <OCRA/Pipeline/Graphics.hpp>
#include <OCRA/Pipeline/Layout.hpp>
#include <OCRA/Pipeline/VertexInputState.hpp>
#include <OCRA/Queue/Fence.hpp>
#include <OCRA/Shader/Module.hpp>
#include <OCRA/Shader/Stage.hpp>
#include <OCRA/Surface.hpp>

#include <OCRA/ShaderCompiler/Compiler.hpp>
#include <OCRA/ShaderCompiler/Shader.hpp>

using namespace OCRA;

#define SWAPCHAIN_IMAGE_NBR 2

Vec3 hue2rgb(float h)
{
    h = fract(saturate(h)) * 6.0 - 2.0;
    return saturate(Vec3(abs(h - 1.0) - 1.0, 2.0 - abs(h), 2.0 - abs(h - 2.0)));
}

Texture2D CreateTexture(const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device)
{
    auto& device = a_Device;
    auto& physicalDevice = a_PhysicalDevice;
    Texture2D texture(a_Device, Image::Format::Uint8_Normalized_RGBA, 640, 480, 1);
    const auto textureSize = texture.GetWidth() * texture.GetHeight() * Image::GetPixelSize(texture.GetImageInfo().format);
    Buffer::Info bufferInfo;
    bufferInfo.size = textureSize;
    bufferInfo.usage = Buffer::UsageFlagBits::TransferSrc;
    auto textureTransferBuffer = Buffer::Create(device, bufferInfo);
    auto textureTransferMemory = AllocateMemory(
        physicalDevice, device,
        bufferInfo.size,
        PhysicalDevice::MemoryPropertyFlagBits::HostVisible);
    Buffer::BindMemory(device, textureTransferBuffer, textureTransferMemory, 0);
    Memory::MappedRange range;
    range.memory = textureTransferMemory;
    range.length = textureSize;
    range.offset = 0;
    auto bufferPtr = (Vec<4, uint8_t>*)Memory::Map(device, range);
    iVec2 iResolution = { texture.GetWidth(), texture.GetHeight() };
    for (uint32_t x = 0; x < texture.GetWidth(); ++x) {
        for (uint32_t y = 0; y < texture.GetHeight(); ++y) {
            float yf = 1 - (y / float(iResolution.y));
            Vec2 fragCoord = { x, yf * iResolution.y };
            float scale = 27.0 / iResolution.y;                             // grid scale
            Vec2 area = Vec2(floor(13.0 / iResolution.y * iResolution.x), 13.0); // size of inner area

            Vec2 p0 = fragCoord - iResolution / 2.0; // position (pixel)
            Vec2 p1 = p0 * scale;                       // position (grid)

            // gray background with crosshair
            float c1 = 1.0 - step(2.f, std::min(abs(p0.x), abs(p0.y))) * 0.5;

            // grid lines
            Vec2 grid = step(scale, abs(Vec2(0.5) - fract(p1 * 0.5)));
            c1 = std::clamp(c1 + 2.0 - grid.x - grid.y, 0.0, 1.0);

            // outer area checker
            Vec2 checker = step(0.49999f, fract(floor(p1 * 0.5 + 0.5) * 0.5));
            if (any(greaterThan(abs(p1), area))) c1 = abs(checker.x - checker.y);

            float corner = sqrt(8.0) - length(abs(p1) - area + 4.0); // corner circles
            float circle = 12.0 - length(p1);                      // big center circle
            float mask = saturate(circle / scale);               // center circls mask

            // grayscale bars
            float bar1 = saturate(p1.y < 5.0 ? floor(p1.x / 4.0 + 3.0) / 5.0 : p1.x / 16.0 + 0.5);
            c1 = mix(c1, bar1, mask * saturate(ceil(4.0 - abs(5.0 - p1.y))));

            // basic color bars
            Vec3 bar2 = hue2rgb((p1.y > -5.0 ? floor(p1.x / 4.0) / 6.0 : p1.x / 16.0) + 0.5);
            Vec3 c2 = mix(Vec3(c1), bar2, mask * saturate(ceil(4.0 - abs(-5.0 - p1.y))));

            // big circle line
            c2 = mix(c2, Vec3(1.0), saturate(2.0 - abs(std::max(circle, corner)) / scale));
            const size_t index = x + y * texture.GetWidth();
            bufferPtr[index].r = c2.x * 255;
            bufferPtr[index].g = c2.y * 255;
            bufferPtr[index].b = c2.z * 255;
            bufferPtr[index].a = 255;
        }
    }
    Memory::Unmap(device, textureTransferMemory);
    Image::BufferCopy bufferCopy;
    bufferCopy.imageExtent = texture.GetImageInfo().extent;
    Image::CopyBufferToImage(device, textureTransferBuffer, texture.GetImage(), { bufferCopy });
    return texture;
}

struct TexturesTestApp : TestApp
{
    TexturesTestApp()
        : TestApp("Test_Textures")
        , physicalDevice(Instance::EnumeratePhysicalDevices(instance).front())
        , device(CreateDevice(physicalDevice))
        , window(Window(instance, physicalDevice, device, name, 1280, 720))
    {
        window.OnResize = [this](const Window&, const uint32_t a_Width, const uint32_t a_Height) {
            render = a_Width > 0 && a_Height > 0;
            if (render && !window.IsClosing()) {
                CreateFrameBuffer();
                CreateGraphicsPipeline();
                RecordDrawCommandBuffer();
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
    }
    void Loop()
    {
        FPSCounter fpsCounter;
        auto printTime = std::chrono::high_resolution_clock::now();
        window.SetVSync(false);
        while (true) {
            fpsCounter.StartFrame();
            window.PushEvents();
            if (window.IsClosing()) break;
            if (!render) continue;
            const auto now = std::chrono::high_resolution_clock::now();
            swapChainImage = window.AcquireNextImage(std::chrono::nanoseconds(15000000), nullptr, imageAcquisitionFence);
            Queue::Fence::WaitFor(device, imageAcquisitionFence, std::chrono::nanoseconds(15000000));
            Queue::Fence::Reset(device, { imageAcquisitionFence });
            textureUniform.Update();
            RecordMainCommandBuffer();
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
                "#version 450                                                   \n"
                "layout(location = 0) out vec2 UV;                              \n"
                "void main() {                                                  \n"
                "   UV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);   \n"
                "   gl_Position = vec4(UV * 2.0f + -1.0f, 0.0f, 1.0f);          \n"
                "}                                                              \n"
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
                "layout(binding = 1) uniform sampler2D uTexture;      \n"
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
    void CreateFrameBuffer()
    {
        {
            Image::Info imageInfo;
            imageInfo.type = Image::Type::Image2D;
            imageInfo.extent.width = window.GetExtent().width;
            imageInfo.extent.height = window.GetExtent().height;
            imageInfo.extent.depth = 1;
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
        {
            Pipeline::Layout::Info layoutInfo;
            Descriptor::SetLayout::Info descriptorInfo;
            descriptorInfo.bindings = textureUniform.GetDescriptorSetLayoutBindings();
            layoutInfo.setLayouts.push_back(Descriptor::SetLayout::Create(device, descriptorInfo));
            graphicsPipelineLayout = Pipeline::Layout::Create(device, layoutInfo);
        }
        Pipeline::ColorBlendState::AttachmentState colorBlend0;
        colorBlend0.enable = true;
        Pipeline::Graphics::Info graphicsPipelineInfo;
        graphicsPipelineInfo.layout = graphicsPipelineLayout;
        graphicsPipelineInfo.colorBlendState.attachments.resize(1);
        graphicsPipelineInfo.colorBlendState.attachments.front() = colorBlend0;
        graphicsPipelineInfo.rasterizationState.cullMode = Pipeline::RasterizationState::CullMode::None;
        graphicsPipelineInfo.inputAssemblyState.topology = Primitive::Topology::TriangleList;
        graphicsPipelineInfo.inputAssemblyState.primitiveRestartEnable = false;
        graphicsPipelineInfo.viewPortState.viewPorts = { viewport };
        graphicsPipelineInfo.viewPortState.scissors = { scissor };
        graphicsPipelineInfo.vertexInputState.attributeDescriptions = {}; //empty attributes
        graphicsPipelineInfo.vertexInputState.bindingDescriptions = {}; //empty bindings
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
    void RecordMainCommandBuffer()
    {
        static float hue = 0;
        static auto lastTime = std::chrono::high_resolution_clock::now();
        const auto now = std::chrono::high_resolution_clock::now();
        const auto delta = std::chrono::duration<double, std::milli>(now - lastTime).count();
        lastTime = now;
        hue += 0.05 * delta;
        hue = hue > 360 ? 0 : hue;
        Command::Buffer::BeginInfo bufferBeginInfo;
        bufferBeginInfo.flags = Command::Buffer::UsageFlagBits::None;
        Command::Buffer::Reset(mainCommandBuffer);
        Command::Buffer::Begin(mainCommandBuffer, bufferBeginInfo);
        //clear background
        {
            ColorValue clearColor{ 0.f, 0.f, 0.f, 1.f };
            Image::Subresource::Range range{};
            range.levelCount = 1;
            Command::ClearColorImage(mainCommandBuffer, swapChainImage, Image::Layout::Unknown, clearColor, { range });
        }
        //draw quad
        Command::ExecuteCommandBuffer(mainCommandBuffer, drawCommandBuffer);
        //copy result to swapchain
        {
            Image::Copy imageCopy;
            imageCopy.extent.width = window.GetExtent().width;
            imageCopy.extent.height = window.GetExtent().height;
            imageCopy.extent.depth = 1;
            Command::CopyImage(
                mainCommandBuffer,
                frameBufferImage,
                swapChainImage,
                { imageCopy });
        }
        Command::Buffer::End(mainCommandBuffer);
    }
    void RecordDrawCommandBuffer()
    {
        textureUniform.Update();
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
            Command::PushDescriptorSet(drawCommandBuffer, Pipeline::BindingPoint::Graphics, graphicsPipelineLayout, textureUniform.GetWriteOperations());
            Command::Draw(drawCommandBuffer, 3, 1, 0, 0);
        }
        Command::EndRenderPass(drawCommandBuffer);
        Command::Buffer::End(drawCommandBuffer);
    }
    bool                    render{ false };
    PhysicalDevice::Handle  physicalDevice;
    Device::Handle          device;
    Image::Handle           swapChainImage;
    Window                  window;    

    Queue::Handle           queue;
    Queue::Fence::Handle    imageAcquisitionFence;

    FrameBuffer::Handle      frameBuffer;
    Image::Handle            frameBufferImage;

    RenderPass::Handle       renderPass;
    
    Pipeline::Handle         graphicsPipeline;
    Pipeline::Layout::Handle graphicsPipelineLayout;

    std::vector<Shader::Stage::Handle>   shaderStages;

    UniformTexture              textureUniform{ device, 1, CreateTexture(physicalDevice, device) };

    Command::Pool::Handle   commandPool;
    Command::Buffer::Handle mainCommandBuffer;
    Command::Buffer::Handle drawCommandBuffer;
};

int main()
{
    int ret = 0;
    TexturesTestApp testApp;
    testApp.window.Show();
    testApp.Loop();
    return ret;
}
