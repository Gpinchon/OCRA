#include <Common.hpp>
#include <Window.hpp>
#include <UniformTexture.hpp>

#include <OCRA/OCRA.hpp>

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
    Texture2D texture(a_Device, Format::Uint8_Normalized_RGBA, 640, 480, 1);
    const auto textureSize = Image::GetDataSize(texture.GetImage());
    AllocateBufferInfo bufferInfo;
    bufferInfo.bufferFlags = CreateBufferFlagBits::None;
    bufferInfo.bufferUsage = BufferUsageFlagBits::TransferDst | BufferUsageFlagBits::TransferSrc;
    bufferInfo.memoryFlags = MemoryPropertyFlagBits::HostVisible;
    bufferInfo.size = Image::GetDataSize(texture.GetImage());
    auto textureTransferBuffer = Device::AllocateBuffer(device, bufferInfo);
    auto textureTransferMemory = Buffer::GetMemory(textureTransferBuffer);
    MemoryMappedRange range;
    range.memory = textureTransferMemory;
    range.length = textureSize;
    range.offset = 0;
    auto bufferPtr = (Vec<4, uint8_t>*)Memory::Map(range);
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
    Memory::Unmap(textureTransferMemory);
    ImageBufferCopy bufferCopy;
    bufferCopy.imageExtent = texture.GetImageInfo().extent;
    Image::CopyBufferToImage(textureTransferBuffer, texture.GetImage(), { bufferCopy });
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
        const auto queueFamily = FindQueueFamily(physicalDevice, QueueFlagBits::Graphics);
        queue = Device::GetQueue(device, queueFamily, 0); //Get first available queue
        imageAcquisitionFence = CreateFence(device);
        commandPool = CreateCommandPool(device, queueFamily);
        mainCommandBuffer = CreateCommandBuffer(commandPool, CommandBufferLevel::Primary);
        drawCommandBuffer = CreateCommandBuffer(commandPool, CommandBufferLevel::Secondary);
        drawSemaphore = CreateSemaphore(device, { SemaphoreType::Binary, 0 });
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

            swapChainImage = window.AcquireNextImage(std::chrono::nanoseconds(15000000), nullptr, imageAcquisitionFence);
            render = Fence::WaitFor(imageAcquisitionFence, std::chrono::nanoseconds(15000000));
            Fence::Reset({ imageAcquisitionFence });
            if (!render) continue;

            textureUniform.Update();
            RecordMainCommandBuffer();
            QueueSubmitInfo submitInfo;
            submitInfo.commandBuffers   = { mainCommandBuffer };
            submitInfo.signalSemaphores = { { drawSemaphore } };
            Queue::Submit(queue, { submitInfo });
            window.Present(queue, drawSemaphore);
            fpsCounter.EndFrame();

            const auto now = std::chrono::high_resolution_clock::now();
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
            CreateShaderModuleInfo shaderModuleInfo;
            shaderModuleInfo.code = ShaderCompiler::Shader::Compile(vertexShader);
            PipelineShaderStage shaderStage;
            shaderStage.entryPoint = "main";
            shaderStage.module = Device::CreateShaderModule(device, shaderModuleInfo);
            shaderStage.specializationInfo = {};
            shaderStage.stage = ShaderStageFlagBits::Vertex;
            shaderStages.push_back(shaderStage);
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
            CreateShaderModuleInfo shaderModuleInfo;
            shaderModuleInfo.code = ShaderCompiler::Shader::Compile(fragmentShader);
            PipelineShaderStage shaderStage;
            shaderStage.entryPoint = "main";
            shaderStage.module = Device::CreateShaderModule(device, shaderModuleInfo);
            shaderStage.specializationInfo = {};
            shaderStage.stage = ShaderStageFlagBits::Fragment;
            shaderStages.push_back(shaderStage);
        }
    }
    void CreateFrameBuffer()
    {
        {
            CreateImageInfo imageInfo;
            imageInfo.type = ImageType::Image2D;
            imageInfo.extent.width = window.GetExtent().width;
            imageInfo.extent.height = window.GetExtent().height;
            imageInfo.extent.depth = 1;
            imageInfo.format = Format::Uint8_Normalized_RGBA;
            imageInfo.mipLevels = 1;
            frameBufferImage = CreateImage(device, imageInfo);
        }
        {
            CreateImageViewInfo imageViewInfo;
            imageViewInfo.image = frameBufferImage;
            imageViewInfo.format = Format::Uint8_Normalized_RGBA;
            imageViewInfo.type = ImageViewType::View2D;
            imageViewInfo.subRange.layerCount = 1;
            frameBufferImageView = CreateImageView(device, imageViewInfo);
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
            CreatePipelineLayoutInfo layoutInfo;
            CreateDescriptorSetLayoutInfo descriptorInfo;
            descriptorInfo.bindings = textureUniform.GetDescriptorSetLayoutBindings();
            layoutInfo.setLayouts.push_back(CreateDescriptorSetLayout(device, descriptorInfo));
            graphicsPipelineLayout = CreatePipelineLayout(device, layoutInfo);
        }
        PipelineColorBlendState;
        PipelineColorBlendAttachmentState colorBlend0;
        colorBlend0.enable = true;
        CreatePipelineGraphicsInfo graphicsPipelineInfo;
        graphicsPipelineInfo.layout = graphicsPipelineLayout;
        graphicsPipelineInfo.colorBlendState.attachments.resize(1);
        graphicsPipelineInfo.colorBlendState.attachments.front() = colorBlend0;
        graphicsPipelineInfo.rasterizationState.cullMode = CullMode::None;
        graphicsPipelineInfo.inputAssemblyState.topology = PrimitiveTopology::TriangleList;
        graphicsPipelineInfo.inputAssemblyState.primitiveRestartEnable = false;
        graphicsPipelineInfo.viewPortState.viewPorts = { viewport };
        graphicsPipelineInfo.viewPortState.scissors = { scissor };
        graphicsPipelineInfo.vertexInputState.attributeDescriptions = {}; //empty attributes
        graphicsPipelineInfo.vertexInputState.bindingDescriptions = {}; //empty bindings
        graphicsPipelineInfo.shaderPipelineState.stages = shaderStages;
        //Everything else is left by default for now
        graphicsPipeline = CreatePipelineGraphics(device, graphicsPipelineInfo);
    }
    void RecordMainCommandBuffer()
    {
        CommandBufferBeginInfo bufferBeginInfo;
        bufferBeginInfo.flags = CommandBufferUsageFlagBits::None;
        Command::Buffer::Reset(mainCommandBuffer);
        Command::Buffer::Begin(mainCommandBuffer, bufferBeginInfo);
        //draw quad
        Command::ExecuteCommandBuffer(mainCommandBuffer, drawCommandBuffer);
        //copy result to swapchain
        {
            ImageCopy imageCopy;
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
        CommandBufferBeginInfo bufferBeginInfo{};
        bufferBeginInfo.flags = CommandBufferUsageFlagBits::None;
        Command::Buffer::Reset(drawCommandBuffer);

        RenderingAttachmentInfo attachmentInfo;
        attachmentInfo.clearValue = ColorValue(0.9529f, 0.6235f, 0.0941f, 1.f);
        attachmentInfo.imageView = frameBufferImageView;
        attachmentInfo.imageLayout = ImageLayout::General;
        attachmentInfo.loadOp = LoadOp::Clear;
        attachmentInfo.storeOp = StoreOp::Store;

        RenderingInfo renderingInfo;
        renderingInfo.area.offset = { 0, 0 };
        renderingInfo.area.extent = window.GetExtent();
        renderingInfo.colorAttachments.push_back(attachmentInfo);
        renderingInfo.layerCount = 1;

        Command::Buffer::Begin(drawCommandBuffer, bufferBeginInfo);
        Command::BeginRendering(drawCommandBuffer, renderingInfo);
        {
            Command::BindPipeline(drawCommandBuffer, graphicsPipeline);
            Command::PushDescriptorSet(drawCommandBuffer, PipelineBindingPoint::Graphics, graphicsPipelineLayout, textureUniform.GetWriteOperations());
            Command::Draw(drawCommandBuffer, 3, 1, 0, 0);
        }
        Command::EndRendering(drawCommandBuffer);
        Command::Buffer::End(drawCommandBuffer);
    }
    bool                    render{ false };
    PhysicalDevice::Handle  physicalDevice;
    Device::Handle          device;
    Image::Handle           swapChainImage;
    Window                  window;    

    Queue::Handle           queue;
    Semaphore::Handle drawSemaphore;
    Fence::Handle    imageAcquisitionFence;

    Image::Handle            frameBufferImage;
    Image::View::Handle      frameBufferImageView;
    
    Pipeline::Handle         graphicsPipeline;
    Pipeline::Layout::Handle graphicsPipelineLayout;

    std::vector<PipelineShaderStage>   shaderStages;

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
