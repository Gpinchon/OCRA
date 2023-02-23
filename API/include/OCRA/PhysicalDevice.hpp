#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Common/Extent3D.hpp>
#include <OCRA/Common/SampleCount.hpp>

#include <string>
#include <array>
#include <bitset>
#include <vector>

OCRA_DECLARE_HANDLE(OCRA::PhysicalDevice);

namespace OCRA::PhysicalDevice
{
struct Limits {
    uint32_t              maxImageDimension1D;
    uint32_t              maxImageDimension2D;
    uint32_t              maxImageDimension3D;
    uint32_t              maxImageDimensionCube;
    uint32_t              maxImageArrayLayers;
    uint32_t              maxTexelBufferElements;
    uint32_t              maxUniformBufferRange;
    uint32_t              maxStorageBufferRange;
    uint32_t              maxPushConstantsSize;
    uint32_t              maxMemoryAllocationCount;
    uint32_t              maxSamplerAllocationCount;
    uint64_t              bufferImageGranularity;
    uint64_t              sparseAddressSpaceSize;
    uint32_t              maxBoundDescriptorSets;
    uint32_t              maxPerStageDescriptorSamplers;
    uint32_t              maxPerStageDescriptorUniformBuffers;
    uint32_t              maxPerStageDescriptorStorageBuffers;
    uint32_t              maxPerStageDescriptorSampledImages;
    uint32_t              maxPerStageDescriptorStorageImages;
    uint32_t              maxPerStageDescriptorInputAttachments;
    uint32_t              maxPerStageResources;
    uint32_t              maxDescriptorSetSamplers;
    uint32_t              maxDescriptorSetUniformBuffers;
    uint32_t              maxDescriptorSetUniformBuffersDynamic;
    uint32_t              maxDescriptorSetStorageBuffers;
    uint32_t              maxDescriptorSetStorageBuffersDynamic;
    uint32_t              maxDescriptorSetSampledImages;
    uint32_t              maxDescriptorSetStorageImages;
    uint32_t              maxDescriptorSetInputAttachments;
    uint32_t              maxVertexInputAttributes;
    uint32_t              maxVertexInputBindings;
    uint32_t              maxVertexInputAttributeOffset;
    uint32_t              maxVertexInputBindingStride;
    uint32_t              maxVertexOutputComponents;
    uint32_t              maxTessellationGenerationLevel;
    uint32_t              maxTessellationPatchSize;
    uint32_t              maxTessellationControlPerVertexInputComponents;
    uint32_t              maxTessellationControlPerVertexOutputComponents;
    uint32_t              maxTessellationControlPerPatchOutputComponents;
    uint32_t              maxTessellationControlTotalOutputComponents;
    uint32_t              maxTessellationEvaluationInputComponents;
    uint32_t              maxTessellationEvaluationOutputComponents;
    uint32_t              maxGeometryShaderInvocations;
    uint32_t              maxGeometryInputComponents;
    uint32_t              maxGeometryOutputComponents;
    uint32_t              maxGeometryOutputVertices;
    uint32_t              maxGeometryTotalOutputComponents;
    uint32_t              maxFragmentInputComponents;
    uint32_t              maxFragmentOutputAttachments;
    uint32_t              maxFragmentDualSrcAttachments;
    uint32_t              maxFragmentCombinedOutputResources;
    uint32_t              maxComputeSharedMemorySize;
    uint32_t              maxComputeWorkGroupCount[3];
    uint32_t              maxComputeWorkGroupInvocations;
    uint32_t              maxComputeWorkGroupSize[3];
    uint32_t              subPixelPrecisionBits;
    uint32_t              subTexelPrecisionBits;
    uint32_t              mipmapPrecisionBits;
    uint32_t              maxDrawIndexedIndexValue;
    uint32_t              maxDrawIndirectCount;
    float                 maxSamplerLodBias;
    float                 maxSamplerAnisotropy;
    uint32_t              maxViewports;
    uint32_t              maxViewportDimensions[2];
    float                 viewportBoundsRange[2];
    uint32_t              viewportSubPixelBits;
    size_t                minMemoryMapAlignment;
    uint64_t              minTexelBufferOffsetAlignment;
    uint64_t              minUniformBufferOffsetAlignment;
    uint64_t              minStorageBufferOffsetAlignment;
    int32_t               minTexelOffset;
    uint32_t              maxTexelOffset;
    int32_t               minTexelGatherOffset;
    uint32_t              maxTexelGatherOffset;
    float                 minInterpolationOffset;
    float                 maxInterpolationOffset;
    uint32_t              subPixelInterpolationOffsetBits;
    uint32_t              maxFramebufferWidth;
    uint32_t              maxFramebufferHeight;
    uint32_t              maxFramebufferLayers;
    SampleCount           framebufferColorSampleCounts;
    SampleCount           framebufferDepthSampleCounts;
    SampleCount           framebufferStencilSampleCounts;
    SampleCount           framebufferNoAttachmentsSampleCounts;
    uint32_t              maxColorAttachments;
    SampleCount           sampledImageColorSampleCounts;
    SampleCount           sampledImageIntegerSampleCounts;
    SampleCount           sampledImageDepthSampleCounts;
    SampleCount           sampledImageStencilSampleCounts;
    SampleCount           storageImageSampleCounts;
    uint32_t              maxSampleMaskWords;
    bool                  timestampComputeAndGraphics;
    float                 timestampPeriod;
    uint32_t              maxClipDistances;
    uint32_t              maxCullDistances;
    uint32_t              maxCombinedClipAndCullDistances;
    uint32_t              discreteQueuePriorities;
    float                 pointSizeRange[2];
    float                 lineWidthRange[2];
    float                 pointSizeGranularity;
    float                 lineWidthGranularity;
    bool                  strictLines;
    bool                  standardSampleLocations;
    uint64_t              optimalBufferCopyOffsetAlignment;
    uint64_t              optimalBufferCopyRowPitchAlignment;
    uint64_t              nonCoherentAtomSize;
};
struct SparseProperties {
    bool    residencyStandard2DBlockShape{ false };
    bool    residencyStandard2DMultisampleBlockShape{ false };
    bool    residencyStandard3DBlockShape{ false };
    bool    residencyAlignedMipSize{ false };
    bool    residencyNonResidentStrict{ false };
};
enum class Type {
    Other = 0,
    IntegratedGpu = 1,
    DiscreteGpu = 2,
    VirtualGpu = 3,
    Cpu = 4,
    MaxValue
};
struct Properties {
    uint32_t                apiVersion{ 0 };
    uint64_t                driverVersion{ 0 };
    uint32_t                vendorID{ 0 };
    uint32_t                deviceID{ 0 };
    Type                    deviceType;
    std::string             vendorName;
    std::string             deviceName;
    std::array<uint8_t, 16> pipelineCacheUUID;
    Limits                  limits;
    SparseProperties        sparseProperties;
};
struct Features {
    bool    robustBufferAccess{ false };
    bool    fullDrawIndexUint32{ false };
    bool    imageCubeArray{ false };
    bool    independentBlend{ false };
    bool    geometryShader{ false };
    bool    tessellationShader{ false };
    bool    sampleRateShading{ false };
    bool    dualSrcBlend{ false };
    bool    logicOp{ false };
    bool    multiDrawIndirect{ false };
    bool    drawIndirectFirstInstance{ false };
    bool    depthClamp{ false };
    bool    depthBiasClamp{ false };
    bool    fillModeNonSolid{ false };
    bool    depthBounds{ false };
    bool    wideLines{ false };
    bool    largePoints{ false };
    bool    alphaToOne{ false };
    bool    multiViewport{ false };
    bool    samplerAnisotropy{ false };
    bool    textureCompressionETC2{ false };
    bool    textureCompressionASTC_LDR{ false };
    bool    textureCompressionBC{ false };
    bool    occlusionQueryPrecise{ false };
    bool    pipelineStatisticsQuery{ false };
    bool    vertexPipelineStoresAndAtomics{ false };
    bool    fragmentStoresAndAtomics{ false };
    bool    shaderTessellationAndGeometryPointSize{ false };
    bool    shaderImageGatherExtended{ false };
    bool    shaderStorageImageExtendedFormats{ false };
    bool    shaderStorageImageMultisample{ false };
    bool    shaderStorageImageReadWithoutFormat{ false };
    bool    shaderStorageImageWriteWithoutFormat{ false };
    bool    shaderUniformBufferArrayDynamicIndexing{ false };
    bool    shaderSampledImageArrayDynamicIndexing{ false };
    bool    shaderStorageBufferArrayDynamicIndexing{ false };
    bool    shaderStorageImageArrayDynamicIndexing{ false };
    bool    shaderClipDistance{ false };
    bool    shaderCullDistance{ false };
    bool    shaderFloat64{ false };
    bool    shaderInt64{ false };
    bool    shaderInt16{ false };
    bool    shaderResourceResidency{ false };
    bool    shaderResourceMinLod{ false };
    bool    sparseBinding{ false };
    bool    sparseResidencyBuffer{ false };
    bool    sparseResidencyImage2D{ false };
    bool    sparseResidencyImage3D{ false };
    bool    sparseResidency2Samples{ false };
    bool    sparseResidency4Samples{ false };
    bool    sparseResidency8Samples{ false };
    bool    sparseResidency16Samples{ false };
    bool    sparseResidencyAliased{ false };
    bool    variableMultisampleRate{ false };
    bool    inheritedQueries{ false };
};
using QueueFlags = std::bitset<5>;
namespace QueueFlagsBits {
static QueueFlags None          = 0b00000;
static QueueFlags Graphics      = 0b10000;
static QueueFlags Compute       = 0b01000;
static QueueFlags Transfer      = 0b00100;
static QueueFlags SparseBinding = 0b00010;
static QueueFlags Protected     = 0b00001;// VK_VERSION_1_1
}
struct QueueFamilyProperties {
    QueueFlags queueFlags{ QueueFlagsBits::None };
    uint32_t   queueCount{ 0 };
    uint32_t   timestampValidBits{ 0 };
    Extent3D   minImageTransferGranularity{};
};
const std::vector<QueueFamilyProperties> GetQueueFamilyProperties(const Handle& a_PhysicalDevice);

using MemoryPropertyFlags = std::bitset<6>;
namespace MemoryPropertyFlagBits {
static MemoryPropertyFlags None             = 0b000000;
static MemoryPropertyFlags DeviceLocal      = 0b100000;
static MemoryPropertyFlags HostVisible      = 0b010000; //allows for memory mapping
static MemoryPropertyFlags HostCoherent     = 0b001000; //flush and invalidate are not necesssary for sync with device
static MemoryPropertyFlags HostCached       = 0b000100; //is backed by host's buffer, faster access
static MemoryPropertyFlags LazilyAllocated  = 0b000010; //only allows access from device, do not use with HostVisible
static MemoryPropertyFlags Protected        = 0b000001; //only allows access from device, allows protected queue operations, do not use with HostVisible, HostCoherent or HostCached
};
struct MemoryType {
    MemoryPropertyFlags propertyFlags{ MemoryPropertyFlagBits::None };
    uint32_t            heapIndex{ 0 };
};
using MemoryHeapFlags = std::bitset<2>;
namespace MemoryHeapFlagBits {
static MemoryHeapFlags None             = 0b00;
static MemoryHeapFlags DeviceLocal      = 0b10;
static MemoryHeapFlags MultiInstance    = 0b01; //allows for memory mapping
};
struct MemoryHeap {
    size_t          size{ 0 };
    MemoryHeapFlags flags{ MemoryHeapFlagBits::None };
};
struct MemoryProperties {
    std::vector<MemoryType> memoryTypes;
    std::vector<MemoryHeap> memoryHeaps;
};

const MemoryProperties GetMemoryProperties(const Handle& a_PhysicalDevice);
const Properties GetProperties(const Handle& a_PhysicalDevice);
const Features GetFeatures(const Handle& a_PhysicalDevice);
}