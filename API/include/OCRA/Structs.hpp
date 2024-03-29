#pragma once

#include <OCRA/Allocator.hpp>
#include <OCRA/Enums.hpp>
#include <OCRA/Extent.hpp>
#include <OCRA/Flags.hpp>
#include <OCRA/Handles.hpp>
#include <OCRA/Vec.hpp>

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <list>
#include <optional>
#include <string>
#include <vector>

// Damn you Windows !
#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

namespace OCRA {
// Use this to ignore timeout and wait indefinitely
static constexpr auto IgnoreTimeout     = (std::chrono::nanoseconds::max)();
static constexpr auto IgnoreQueueFamily = (std::numeric_limits<uint32_t>::max)();
struct DescriptorPoolSize {
    DescriptorType type { DescriptorType::Unknown };
    uint32_t count { 0 }; // nbr of allocated types, byte size for InlineUniformBlock
};

struct DescriptorSetBinding {
    uint32_t binding { 0 }; // number of this entry corresponding to a resource inside shader
    DescriptorType type { DescriptorType::Unknown }; // the type of resource descriptor to use
    uint32_t count { 0 }; // the nbr of descriptor, when InlineUniformBlock used, is byte size
    ShaderStageFlags stageFlags { ShaderStageFlagBits::None }; // the shader stages that can access this resource, might not be used
};
struct DescriptorSetBufferInfo {
    Buffer::Handle buffer;
    size_t offset { 0 };
    size_t range { 0 };
};
struct DescriptorSetImageInfo {
    Image::Sampler::Handle sampler;
    Image::View::Handle imageView;
    ImageLayout imageLayout { ImageLayout::Undefined };
};
struct DescriptorSetWrite {
    Descriptor::Set::Handle dstSet; // ignored when using PushDescriptor
    uint32_t dstBinding { 0 };
    uint32_t dstArrayElement { 0 };
    uint32_t dstCount { 0 };
    DescriptorType type { DescriptorType::Unknown };
    std::optional<std::vector<uint8_t>> inlineUniformBlock;
    std::optional<DescriptorSetBufferInfo> bufferInfo;
    std::optional<DescriptorSetImageInfo> imageInfo;
    std::optional<Buffer::Handle> texelBuffer;
};
struct DescriptorSetCopy {
    Descriptor::Set::Handle srcSet;
    Descriptor::Set::Handle dstSet;
    uint32_t srcBinding { 0 };
    uint32_t dstBinding { 0 };
    uint32_t srcArrayElement { 0 };
    uint32_t dstArrayElement { 0 };
    uint32_t count { 0 };
};

union ColorValue {
    constexpr ColorValue() noexcept = default;
    constexpr ColorValue(float a_R, float a_G, float a_B, float a_A) noexcept
    {
        float32[0] = a_R;
        float32[1] = a_G;
        float32[2] = a_B;
        float32[3] = a_A;
    }
    constexpr ColorValue(int32_t a_R, int32_t a_G, int32_t a_B, int32_t a_A) noexcept
    {
        int32[0] = a_R;
        int32[1] = a_G;
        int32[2] = a_B;
        int32[3] = a_A;
    }
    constexpr ColorValue(uint32_t a_R, uint32_t a_G, uint32_t a_B, uint32_t a_A) noexcept
    {
        uint32[0] = a_R;
        uint32[1] = a_G;
        uint32[2] = a_B;
        uint32[3] = a_A;
    }
    int32_t int32[4] { 0, 0, 0, 0 };
    uint32_t uint32[4];
    float float32[4];
};
constexpr auto TransparentBlack = ColorValue { 0, 0, 0, 0 };
constexpr auto OpaqueBlack      = ColorValue { 0, 0, 0, 1 };
constexpr auto OpaqueWhite      = ColorValue { 1, 1, 1, 1 };

struct DepthStencilValue {
    float depth { 0 };
    uint32_t stencil { 0 };
};
union ClearValue {
    ClearValue() {};
    ClearValue(const ColorValue& a_Value)
        : color(a_Value)
    {
    }
    ClearValue(const DepthStencilValue& a_Value)
        : depthStencil(a_Value)
    {
    }
    ColorValue color { TransparentBlack };
    DepthStencilValue depthStencil;
};
struct ComponentMapping {
    Swizzle r { Swizzle::R };
    Swizzle g { Swizzle::G };
    Swizzle b { Swizzle::B };
    Swizzle a { Swizzle::A };
};
template <typename T = double>
struct DepthBounds {
    DepthBounds(const T& a_Min = T(0), const T& a_Max = T(0))
    {
        min = a_Min;
        max = a_Max;
    }
    T min { 0 }, max { 1000 };
};
template <typename T = double>
struct DepthRange {
    DepthRange(const T& a_Near, const T& a_Far)
        : near(a_Near)
        , far(a_Far)
    {
    }
    T near { 0 }, far { 1 };
};

using Offset2D = iVec2;
using Offset3D = iVec3;

struct Rect2D {
    Rect2D(int a_X = 0, int a_Y = 0, unsigned a_Width = 0, unsigned a_Height = 0)
        : offset(a_X, a_Y)
        , extent(a_Width, a_Height)
    {
    }
    Offset2D offset { 0, 0 };
    uExtent2D extent { 0, 0 };
};
struct Rect3D {
    iVec3 offset;
    uExtent3D extent;
};

struct ViewPort {
    Rect2D rect { 0, 0, 0, 0 };
    DepthRange<double> depthRange { 0, 1 };
};
struct ScissorTest {
    float x, y;
    unsigned width, height;
};

static constexpr auto RemainingMipLevels   = ~0U;
static constexpr auto RemainingArrayLayers = ~0U;
struct ImageSubresourceLayers {
    ImageAspectFlags aspects = ImageAspectFlagBits::None;
    uint32_t mipLevel        = 0;
    uint32_t baseArrayLayer  = 0;
    uint32_t layerCount      = RemainingArrayLayers;
};
struct ImageSubresourceRange {
    ImageAspectFlags aspects = ImageAspectFlagBits::None;
    uint32_t baseMipLevel    = 0;
    uint32_t levelCount      = RemainingMipLevels;
    uint32_t baseArrayLayer  = 0;
    uint32_t layerCount      = RemainingArrayLayers;
};
struct ImageBufferCopy {
    uint64_t bufferOffset { 0 };
    uint32_t bufferRowLength { 0 };
    uint32_t bufferImageHeight { 0 };
    ImageSubresourceLayers imageSubresource;
    Offset3D imageOffset;
    Extent3D imageExtent;
};

struct ImageBlit {
    ImageSubresourceLayers srcSubresource;
    std::array<Offset3D, 2> srcOffsets;
    ImageSubresourceLayers dstSubresource;
    std::array<Offset3D, 2> dstOffsets;
};

struct ImageCopy {
    ImageSubresourceLayers srcSubresource;
    Offset3D srcOffset;
    ImageSubresourceLayers dstSubresource;
    Offset3D dstOffset;
    Extent3D extent {};
};

struct PipelineRenderingInfo {
    std::vector<Format> colorAttachmentFormats; // The formats of the color attachments
    Format depthAttachmentFormat   = Format::Unknown; // The format of the depth attachments
    Format stencilAttachmentFormat = Format::Unknown; // The format of the stencil attachments
};
struct PipelineStencilOpState {
    StencilOp failOp { StencilOp::Keep }; // the operation to be realized when stencil test FAILS
    StencilOp passOp { StencilOp::Keep }; // the operation to be realized when stencil test PASSES
    StencilOp depthFailOp { StencilOp::Keep }; // the operation to be realized when stencil test PASSES but depth test FAILS
    CompareOp compareOp { CompareOp::Always };
    uint32_t compareMask { 1 }; // a mask that is ANDed with ref and the buffer's content
    uint32_t writeMask { 1 }; // a mask that is ANDed with the stencil value about to be written to the buffer
    uint32_t reference { 0 }; // the reference value used in comparison.
};
struct PipelineColorBlendAttachmentState {
    bool enable { false }; // is blending enabled ?
    BlendFactor srcColorBlendFactor { BlendFactor::One };
    BlendFactor dstColorBlendFactor { BlendFactor::Zero };
    BlendOp colorBlendOperation { BlendOp::Add };
    BlendFactor srcAlphaBlendFactor { BlendFactor::One };
    BlendFactor dstAlphaBlendFactor { BlendFactor::Zero };
    BlendOp alphaBlendOperation { BlendOp::Add };
    ColorComponentFlags colorMask { ColorComponentFlagBits::RGBA }; // color mask used for writing to this attachment
};
struct PipelineColorBlendState {
    bool logicOpEnable { false };
    LogicOp logicOp { LogicOp::Copy }; // see glLogicOp
    std::vector<PipelineColorBlendAttachmentState> attachments;
    Vec4 blendConstants;
};
struct PipelineDepthStencilState {
    bool depthTestEnable { true };
    bool depthWriteEnable { true };
    CompareOp depthCompareOp { CompareOp::Less };
    bool depthBoundsTestEnable { false };
    bool stencilTestEnable { false };
    PipelineStencilOpState frontStencilOpState;
    PipelineStencilOpState backStencilOpState;
    DepthBounds<double> depthBounds { 0, 1 };
};
struct PipelineDynamicState {
    std::list<DynamicState> dynamicStates;
    inline bool Contains(const DynamicState a_DynamicState) const
    {
        return std::find(dynamicStates.begin(), dynamicStates.end(), a_DynamicState) != dynamicStates.end();
    }
};
struct PipelineInputAssemblyState {
    PrimitiveTopology topology { PrimitiveTopology::Unknown };
    bool primitiveRestartEnable { false };
};
struct PipelineMultisampleState {
    SampleCount rasterizationSamples { SampleCount::Count1 };
    bool sampleShadingEnable { false };
    float minSampleShading { 1 };
    std::vector<uint32_t> sampleMasks;
    bool alphaToCoverageEnable { false };
    bool alphaToOneEnable { false };
};
struct PipelineRasterizationState {
    bool rasterizerDiscardEnable { false };
    bool depthClampEnable { true };
    bool depthBiasEnable { false };
    float depthBiasConstantFactor { 0 };
    float depthBiasSlopeFactor { 0 };
    float depthBiasClamp { 0 };
    float lineWidth { 1 };
    PolygonMode polygonMode { PolygonMode::Fill };
    CullMode cullMode { CullMode::Back };
    FrontFace frontFace { FrontFace::CounterClockwise };
};
struct ShaderSpecializationMapEntry {
    uint32_t constantID;
    uint32_t offset;
    size_t size;
};
struct ShaderSpecializationInfo {
    std::vector<ShaderSpecializationMapEntry> mapEntries;
    std::vector<uint8_t> data;
};
struct PipelineShaderStage {
    ShaderStageFlags stage { ShaderStageFlagBits::None }; // must be a single stage
    ShaderModule::Handle module; // the spir-v module
    std::string entryPoint { "" }; // entry point name
    ShaderSpecializationInfo specializationInfo;
};
struct PipelineShaderPipelineState {
    std::vector<PipelineShaderStage> stages;
};

struct PipelineTessellationState {
    uint32_t patchControlPoints { 0 };
};
struct VertexAttributeFormat {
    uint8_t size { 0 }; // Number of components per vertex
    VertexType type { VertexType::None }; // Type of data of each components
    bool normalized { false };
};
struct VertexAttributeDescription {
    VertexAttributeFormat format;
    uint32_t offset { 0 }; //(Relative offset) the distance between elements in the buffer
    uint8_t binding { 0 }; // The binding number this attribute takes its data from
    uint32_t location { 0 }; // Location in the shader for this attribute
};
struct VertexBindingDescription {
    uint32_t binding { 0 }; // index inside the BindVertexBuffers Command
    uint32_t stride { 0 }; // byte stride
    VertexInputRate inputRate { VertexInputRate::Vertex }; // is the data divided by vertex or by instance ?
};
struct PipelineVertexInputState {
    uint32_t primitiveRestartIndex { 0 };
    std::vector<VertexAttributeDescription> attributeDescriptions;
    std::vector<VertexBindingDescription> bindingDescriptions;
};
struct PipelineViewPortState {
    std::vector<ViewPort> viewPorts {};
    std::vector<Rect2D> scissors {};
};

struct BufferCopyRegion {
    uint64_t readOffset { 0 }, writeOffset { 0 }, size { 0 };
};

struct MemoryHeap {
    size_t size { 0 };
    MemoryHeapFlags flags { MemoryHeapFlagBits::None };
};
struct MemoryMappedRange {
    Memory::Handle memory; // the memory to map
    uint64_t offset { 0 }; // the beginning index inside the memory
    uint64_t length { 0 }; // the length of memory to map
};
struct MemoryType {
    MemoryPropertyFlags propertyFlags { MemoryPropertyFlagBits::None };
    uint32_t heapIndex { 0 };
};
struct MemoryProperties {
    std::vector<MemoryType> memoryTypes;
    std::vector<MemoryHeap> memoryHeaps;
};

struct PushConstantRange {
    ShaderStageFlags stage { ShaderStageFlagBits::None };
    uint32_t offset { 0 };
    uint32_t size { 0 };
};

struct QueueFamilyProperties {
    QueueFlags queueFlags { QueueFlagBits::None };
    uint32_t queueCount { 0 };
    uint32_t timestampValidBits { 0 };
    Extent3D minImageTransferGranularity {};
};
struct QueueInfo {
    uint32_t queueFamilyIndex { 0 };
    uint32_t queueCount { 0 };
    std::vector<float> queuePriorities;
};
struct TimelineSemaphoreSubmitInfo {
    std::vector<uint64_t> waitSemaphoreValues; // The list of semaphores to wait
    std::vector<uint64_t> signalSemaphoreValues; // The list of semaphores to signal
};
struct QueueSubmitSemaphore {
    Semaphore::Handle semaphore;
    uint64_t timelineValue { 0 }; // the value to wait after if semaphore is of type Timeline
};
struct QueueSubmitWaitInfo : QueueSubmitSemaphore {
    PipelineStageFlags dstStages { PipelineStageFlagBits::None };
};
using QueueSubmitSignalInfo = QueueSubmitSemaphore;
struct QueueSubmitInfo {
    std::vector<OCRA::Command::Buffer::Handle> commandBuffers;
    std::vector<QueueSubmitWaitInfo> waitSemaphores;
    std::vector<QueueSubmitSignalInfo> signalSemaphores;
};

struct CreateQueryPoolInfo {
    QueryType type { QueryType::Unknown };
    uint32_t count { 0 };
    QueryPipelineStatisticFlags pipelineStatistics;
};

struct MemoryBarrier {
    AccessFlags srcAccessMask { AccessFlagBits::None };
    AccessFlags dstAccessMask { AccessFlagBits::None };
};
struct BufferMemoryBarrier : MemoryBarrier {
    uint32_t srcQueueFamilyIndex { IgnoreQueueFamily };
    uint32_t dstQueueFamilyIndex { IgnoreQueueFamily };
    OCRA::Buffer::Handle buffer;
    uint64_t offset { 0 };
    uint64_t size { 0 };
};
struct ImageMemoryBarrier : MemoryBarrier {
    uint32_t srcQueueFamilyIndex { IgnoreQueueFamily };
    uint32_t dstQueueFamilyIndex { IgnoreQueueFamily };
    Image::Handle image;
    ImageSubresourceRange subRange;
    ImageLayout oldLayout;
    ImageLayout newLayout;
};

struct ImageLayoutTransitionInfo {
    Image::Handle image;
    ImageSubresourceRange subRange;
    ImageLayout oldLayout { ImageLayout::Undefined };
    ImageLayout newLayout { ImageLayout::Undefined };
    uint32_t srcQueueFamilyIndex { IgnoreQueueFamily };
    uint32_t dstQueueFamilyIndex { IgnoreQueueFamily };
};

/**
 * imageViewResolve : if RenderingInfo::resolveMode is not None & this is not nullptr, this attachment will be resolved
 * imageLayout        : the layout imageView is in or will be transitioned to
 * imageLayoutResolve : the layout imageViewResolve is in or will be transitioned to
 * loadOp : if this is Clear, this attachment will be cleared using clearValue
 * storeOp : if this is Store, this attachment will be used for rendering
 */
struct RenderingAttachmentInfo {
    Image::View::Handle imageView;
    Image::View::Handle imageViewResolve;
    ImageLayout imageLayout { ImageLayout::Undefined };
    ImageLayout imageLayoutResolve { ImageLayout::Undefined };
    LoadOp loadOp { LoadOp::DontCare };
    StoreOp storeOp { StoreOp::DontCare };
    ClearValue clearValue;
};

struct RenderingInfo {
    RenderingFlags flags;
    Rect2D area;
    uint32_t layerCount { 0 };
    ResolveMode resolveMode { ResolveMode::None };
    std::vector<RenderingAttachmentInfo> colorAttachments;
    RenderingAttachmentInfo depthAttachment;
    RenderingAttachmentInfo stencilAttachment;
};

struct PhysicalDeviceLimits {
    uint32_t maxImageDimension1D;
    uint32_t maxImageDimension2D;
    uint32_t maxImageDimension3D;
    uint32_t maxImageDimensionCube;
    uint32_t maxImageArrayLayers;
    uint32_t maxTexelBufferElements;
    uint32_t maxUniformBufferRange;
    uint32_t maxStorageBufferRange;
    uint32_t maxPushConstantsSize;
    uint32_t maxMemoryAllocationCount;
    uint32_t maxSamplerAllocationCount;
    uint64_t bufferImageGranularity;
    uint64_t sparseAddressSpaceSize;
    uint32_t maxBoundDescriptorSets;
    uint32_t maxPerStageDescriptorSamplers;
    uint32_t maxPerStageDescriptorUniformBuffers;
    uint32_t maxPerStageDescriptorStorageBuffers;
    uint32_t maxPerStageDescriptorSampledImages;
    uint32_t maxPerStageDescriptorStorageImages;
    uint32_t maxPerStageDescriptorInputAttachments;
    uint32_t maxPerStageResources;
    uint32_t maxDescriptorSetSamplers;
    uint32_t maxDescriptorSetUniformBuffers;
    uint32_t maxDescriptorSetUniformBuffersDynamic;
    uint32_t maxDescriptorSetStorageBuffers;
    uint32_t maxDescriptorSetStorageBuffersDynamic;
    uint32_t maxDescriptorSetSampledImages;
    uint32_t maxDescriptorSetStorageImages;
    uint32_t maxDescriptorSetInputAttachments;
    uint32_t maxVertexInputAttributes;
    uint32_t maxVertexInputBindings;
    uint32_t maxVertexInputAttributeOffset;
    uint32_t maxVertexInputBindingStride;
    uint32_t maxVertexOutputComponents;
    uint32_t maxTessellationGenerationLevel;
    uint32_t maxTessellationPatchSize;
    uint32_t maxTessellationControlPerVertexInputComponents;
    uint32_t maxTessellationControlPerVertexOutputComponents;
    uint32_t maxTessellationControlPerPatchOutputComponents;
    uint32_t maxTessellationControlTotalOutputComponents;
    uint32_t maxTessellationEvaluationInputComponents;
    uint32_t maxTessellationEvaluationOutputComponents;
    uint32_t maxGeometryShaderInvocations;
    uint32_t maxGeometryInputComponents;
    uint32_t maxGeometryOutputComponents;
    uint32_t maxGeometryOutputVertices;
    uint32_t maxGeometryTotalOutputComponents;
    uint32_t maxFragmentInputComponents;
    uint32_t maxFragmentOutputAttachments;
    uint32_t maxFragmentDualSrcAttachments;
    uint32_t maxFragmentCombinedOutputResources;
    uint32_t maxComputeSharedMemorySize;
    uint32_t maxComputeWorkGroupCount[3];
    uint32_t maxComputeWorkGroupInvocations;
    uint32_t maxComputeWorkGroupSize[3];
    uint32_t subPixelPrecisionBits;
    uint32_t subTexelPrecisionBits;
    uint32_t mipmapPrecisionBits;
    uint32_t maxDrawIndexedIndexValue;
    uint32_t maxDrawIndirectCount;
    float maxSamplerLodBias;
    float maxSamplerAnisotropy;
    uint32_t maxViewports;
    uint32_t maxViewportDimensions[2];
    float viewportBoundsRange[2];
    uint32_t viewportSubPixelBits;
    size_t minMemoryMapAlignment;
    uint64_t minTexelBufferOffsetAlignment;
    uint64_t minUniformBufferOffsetAlignment;
    uint64_t minStorageBufferOffsetAlignment;
    int32_t minTexelOffset;
    uint32_t maxTexelOffset;
    int32_t minTexelGatherOffset;
    uint32_t maxTexelGatherOffset;
    float minInterpolationOffset;
    float maxInterpolationOffset;
    uint32_t subPixelInterpolationOffsetBits;
    uint32_t maxFramebufferWidth;
    uint32_t maxFramebufferHeight;
    uint32_t maxFramebufferLayers;
    SampleCount framebufferColorSampleCounts;
    SampleCount framebufferDepthSampleCounts;
    SampleCount framebufferStencilSampleCounts;
    SampleCount framebufferNoAttachmentsSampleCounts;
    uint32_t maxColorAttachments;
    SampleCount sampledImageColorSampleCounts;
    SampleCount sampledImageIntegerSampleCounts;
    SampleCount sampledImageDepthSampleCounts;
    SampleCount sampledImageStencilSampleCounts;
    SampleCount storageImageSampleCounts;
    uint32_t maxSampleMaskWords;
    bool timestampComputeAndGraphics;
    float timestampPeriod;
    uint32_t maxClipDistances;
    uint32_t maxCullDistances;
    uint32_t maxCombinedClipAndCullDistances;
    uint32_t discreteQueuePriorities;
    float pointSizeRange[2];
    float lineWidthRange[2];
    float pointSizeGranularity;
    float lineWidthGranularity;
    bool strictLines;
    bool standardSampleLocations;
    uint64_t optimalBufferCopyOffsetAlignment;
    uint64_t optimalBufferCopyRowPitchAlignment;
    uint64_t nonCoherentAtomSize;
};
struct PhysicalDeviceSparseProperties {
    bool residencyStandard2DBlockShape { false };
    bool residencyStandard2DMultisampleBlockShape { false };
    bool residencyStandard3DBlockShape { false };
    bool residencyAlignedMipSize { false };
    bool residencyNonResidentStrict { false };
};
struct PhysicalDeviceProperties {
    uint32_t apiVersion { 0 };
    uint64_t driverVersion { 0 };
    uint32_t vendorID { 0 };
    uint32_t deviceID { 0 };
    PhysicalDeviceType deviceType;
    std::string vendorName;
    std::string deviceName;
    std::array<uint8_t, 16> pipelineCacheUUID;
    PhysicalDeviceLimits limits;
    PhysicalDeviceSparseProperties sparseProperties;
};
struct PhysicalDeviceFeatures {
    bool robustBufferAccess { false };
    bool fullDrawIndexUint32 { false };
    bool imageCubeArray { false };
    bool independentBlend { false };
    bool geometryShader { false };
    bool tessellationShader { false };
    bool sampleRateShading { false };
    bool dualSrcBlend { false };
    bool logicOp { false };
    bool multiDrawIndirect { false };
    bool drawIndirectFirstInstance { false };
    bool depthClamp { false };
    bool depthBiasClamp { false };
    bool fillModeNonSolid { false };
    bool depthBounds { false };
    bool wideLines { false };
    bool largePoints { false };
    bool alphaToOne { false };
    bool multiViewport { false };
    bool samplerAnisotropy { false };
    bool textureCompressionETC2 { false };
    bool textureCompressionASTC_LDR { false };
    bool textureCompressionBC { false };
    bool occlusionQueryPrecise { false };
    bool pipelineStatisticsQuery { false };
    bool vertexPipelineStoresAndAtomics { false };
    bool fragmentStoresAndAtomics { false };
    bool shaderTessellationAndGeometryPointSize { false };
    bool shaderImageGatherExtended { false };
    bool shaderStorageImageExtendedFormats { false };
    bool shaderStorageImageMultisample { false };
    bool shaderStorageImageReadWithoutFormat { false };
    bool shaderStorageImageWriteWithoutFormat { false };
    bool shaderUniformBufferArrayDynamicIndexing { false };
    bool shaderSampledImageArrayDynamicIndexing { false };
    bool shaderStorageBufferArrayDynamicIndexing { false };
    bool shaderStorageImageArrayDynamicIndexing { false };
    bool shaderClipDistance { false };
    bool shaderCullDistance { false };
    bool shaderFloat64 { false };
    bool shaderInt64 { false };
    bool shaderInt16 { false };
    bool shaderResourceResidency { false };
    bool shaderResourceMinLod { false };
    bool sparseBinding { false };
    bool sparseResidencyBuffer { false };
    bool sparseResidencyImage2D { false };
    bool sparseResidencyImage3D { false };
    bool sparseResidency2Samples { false };
    bool sparseResidency4Samples { false };
    bool sparseResidency8Samples { false };
    bool sparseResidency16Samples { false };
    bool sparseResidencyAliased { false };
    bool variableMultisampleRate { false };
    bool inheritedQueries { false };
};

/**
 * @brief if the command buffer is secondary, this specifies what is inherited from it
 */
struct CommandBufferInheritanceInfo {
    uint32_t viewMask = 0;
    std::vector<Format> colorAttachmentFormats;
    Format depthAttachmentFormat   = Format::Unknown;
    Format stencilAttachmentFormat = Format::Unknown;
    SampleCount samples            = SampleCount::Count1;
};

struct CommandBufferBeginInfo {
    CommandBufferUsageFlags flags { CommandBufferUsageFlagBits::None };
    std::optional<CommandBufferInheritanceInfo> inheritanceInfo;
};

struct CreateInstanceInfo {
    std::string name;
    uint32_t applicationVersion { 0 };
    std::string engineName;
    uint32_t engineVersion { 0 };
};
struct CreateBufferInfo {
    CreateBufferFlags flags { CreateBufferFlagBits::None };
    uint64_t size { 0 };
    BufferUsageFlags usage { BufferUsageFlagBits::None };
    SharingMode sharingMode { SharingMode::Exclusive };
    std::vector<uint32_t> queueFamilyIndices;
};
struct CreateCommandPoolInfo {
    CreateCommandPoolFlags flags { CreateCommandPoolFlagBits::None };
    uint32_t queueFamilyIndex { 0 };
};
struct CreateDescriptorPoolInfo {
    uint32_t maxSets = 0;
    std::vector<DescriptorPoolSize> sizes; // the types the allocated descriptors can contain
};
struct CreateDeviceInfo {
    std::vector<QueueInfo> queueInfos;
};
struct CreateImageInfo {
    ImageType type { ImageType::Unknown };
    ImageUsageFlags usage { ImageUsageFlagBits::None }; // Specifies how the image will be used
    SampleCount samples { SampleCount::Count1 };
    Format format { Format::Unknown };
    Extent3D extent {};
    uint16_t mipLevels { 0 }, arrayLayers { 0 };
    bool fixedSampleLocations { false };
};
struct CreateImageViewInfo {
    ImageViewType type { ImageViewType::Unknown };
    Image::Handle image { 0 };
    Format format { Format::Unknown };
    ComponentMapping components {};
    ImageSubresourceRange subRange {};
};
struct CreatePipelineGraphicsInfo { // describes a graphics pipeline with each stage
    DescriptorUpdate descriptorUpdate { DescriptorUpdate::Unknown }; //how will the descriptor set be updated for this pipeline ?
    PipelineColorBlendState colorBlendState {};
    PipelineDepthStencilState depthStencilState {};
    PipelineInputAssemblyState inputAssemblyState {};
    PipelineMultisampleState multisampleState {};
    PipelineRasterizationState rasterizationState {};
    PipelineShaderPipelineState shaderPipelineState {};
    PipelineTessellationState tessellationState {};
    PipelineVertexInputState vertexInputState {};
    PipelineViewPortState viewPortState {};
    PipelineDynamicState dynamicState {};
    PipelineRenderingInfo renderingInfo {};
    std::vector<DescriptorSetBinding> bindings; // the bindings of this pipeline, must be compatible with Pool::Info::sizes
    std::vector<PushConstantRange> pushConstants;
};
struct CreateImageSamplerInfo {
    Filter minFilter { Filter::Linear };
    Filter magFilter { Filter::Linear };
    Filter mipmapMode { Filter::Linear }; // must be Linear or Nearest
    SamplerAddressMode addressModeU { SamplerAddressMode::Repeat };
    SamplerAddressMode addressModeV { SamplerAddressMode::Repeat };
    SamplerAddressMode addressModeW { SamplerAddressMode::Repeat };
    float mipLodBias { 0 };
    bool anisotropyEnable { false };
    float maxAnisotropy { 0 };
    bool compareEnable { false };
    CompareOp compareOp { CompareOp::Less };
    float minLod { 0 };
    float maxLod { 1000 };
    Vec4 borderColor;
};
struct CreateSemaphoreInfo {
    SemaphoreType type { SemaphoreType::Unknown };
    uint64_t initialValue { 0 };
};
struct CreateShaderModuleInfo {
    std::vector<uint32_t> code;
};

struct AllocateCommandBufferInfo {
    CommandBufferLevel level { CommandBufferLevel::Unknown };
    uint32_t count { 0 };
};
struct AllocateDescriptorSetInfo {
    std::vector<DescriptorSetBinding> bindings; // the bindings of this descriptor set, must be compatible with Pool::Info::sizes
};
struct AllocateMemoryInfo {
    size_t size { 0 };
    uint32_t memoryTypeIndex { 0 };
};
struct AllocateBufferInfo {
    MemoryPropertyFlags memoryFlags { MemoryPropertyFlagBits::None };
    CreateBufferFlags bufferFlags { CreateBufferFlagBits::None };
    BufferUsageFlags bufferUsage { BufferUsageFlagBits::None };
    uint64_t size { 0 };
    SharingMode sharingMode { SharingMode::Exclusive };
    std::vector<uint32_t> queueFamilyIndices;
};
}
