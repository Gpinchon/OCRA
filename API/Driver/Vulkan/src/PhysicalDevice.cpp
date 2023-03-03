#include <VK/PhysicalDevice.hpp>
#include <VK/Instance.hpp>

#include <OCRA/Structs.hpp>

namespace OCRA::PhysicalDevice
{
static inline auto GetLimits(const VkPhysicalDeviceLimits& a_vkLimits)
{
    PhysicalDeviceLimits limits{};
    memset(&limits, 0, sizeof(limits));
    limits.maxImageDimension1D    = a_vkLimits.maxImageDimension1D;
    limits.maxImageDimension2D    = a_vkLimits.maxImageDimension2D;
    limits.maxImageDimension3D    = a_vkLimits.maxImageDimension3D;
    limits.maxImageDimensionCube  = a_vkLimits.maxImageDimensionCube;
    limits.maxImageArrayLayers    = a_vkLimits.maxImageArrayLayers;
    limits.maxTexelBufferElements = a_vkLimits.maxTexelBufferElements;
    limits.maxUniformBufferRange  = a_vkLimits.maxUniformBufferRange;
    limits.maxStorageBufferRange  = a_vkLimits.maxStorageBufferRange;

    limits.maxPushConstantsSize      = a_vkLimits.maxPushConstantsSize;
    limits.maxMemoryAllocationCount  = a_vkLimits.maxMemoryAllocationCount;
    limits.maxSamplerAllocationCount = a_vkLimits.maxSamplerAllocationCount;

    limits.bufferImageGranularity = a_vkLimits.bufferImageGranularity;
    limits.sparseAddressSpaceSize = a_vkLimits.sparseAddressSpaceSize;

    limits.maxBoundDescriptorSets                = a_vkLimits.maxBoundDescriptorSets;
    limits.maxPerStageDescriptorSamplers         = a_vkLimits.maxPerStageDescriptorSamplers;
    limits.maxPerStageDescriptorUniformBuffers   = a_vkLimits.maxPerStageDescriptorUniformBuffers;
    limits.maxPerStageDescriptorStorageBuffers   = a_vkLimits.maxPerStageDescriptorStorageBuffers;
    limits.maxPerStageDescriptorSampledImages    = a_vkLimits.maxPerStageDescriptorSampledImages;
    limits.maxPerStageDescriptorStorageImages    = a_vkLimits.maxPerStageDescriptorStorageImages;
    limits.maxPerStageDescriptorInputAttachments = a_vkLimits.maxPerStageDescriptorInputAttachments;
    limits.maxPerStageResources                  = a_vkLimits.maxPerStageResources;

    limits.maxDescriptorSetSamplers              = a_vkLimits.maxDescriptorSetSamplers;
    limits.maxDescriptorSetUniformBuffers        = a_vkLimits.maxDescriptorSetUniformBuffers;
    limits.maxDescriptorSetUniformBuffersDynamic = a_vkLimits.maxDescriptorSetUniformBuffersDynamic;
    limits.maxDescriptorSetStorageBuffers        = a_vkLimits.maxDescriptorSetStorageBuffers;
    limits.maxDescriptorSetStorageBuffersDynamic = a_vkLimits.maxDescriptorSetStorageBuffersDynamic;
    limits.maxDescriptorSetSampledImages         = a_vkLimits.maxDescriptorSetSampledImages;
    limits.maxDescriptorSetStorageImages         = a_vkLimits.maxDescriptorSetStorageImages;
    limits.maxDescriptorSetInputAttachments      = a_vkLimits.maxDescriptorSetInputAttachments;

    limits.maxVertexInputAttributes      = a_vkLimits.maxVertexInputAttributes;
    limits.maxVertexInputBindings        = a_vkLimits.maxVertexInputBindings;
    limits.maxVertexInputAttributeOffset = a_vkLimits.maxVertexInputAttributeOffset;
    limits.maxVertexInputBindingStride   = a_vkLimits.maxVertexInputBindingStride;
    limits.maxVertexOutputComponents     = a_vkLimits.maxVertexOutputComponents;

    limits.maxTessellationGenerationLevel                  = a_vkLimits.maxTessellationGenerationLevel;
    limits.maxTessellationPatchSize                        = a_vkLimits.maxTessellationPatchSize;
    limits.maxTessellationControlPerVertexInputComponents  = a_vkLimits.maxTessellationControlPerVertexInputComponents;
    limits.maxTessellationControlPerVertexOutputComponents = a_vkLimits.maxTessellationControlPerVertexOutputComponents;
    limits.maxTessellationControlPerPatchOutputComponents  = a_vkLimits.maxTessellationControlPerPatchOutputComponents;
    limits.maxTessellationControlTotalOutputComponents     = a_vkLimits.maxTessellationControlTotalOutputComponents;
    limits.maxTessellationEvaluationInputComponents        = a_vkLimits.maxTessellationEvaluationInputComponents;
    limits.maxTessellationEvaluationOutputComponents       = a_vkLimits.maxTessellationEvaluationOutputComponents;

    limits.maxGeometryShaderInvocations     = a_vkLimits.maxGeometryShaderInvocations;
    limits.maxGeometryInputComponents       = a_vkLimits.maxGeometryInputComponents;
    limits.maxGeometryOutputComponents      = a_vkLimits.maxGeometryOutputComponents;
    limits.maxGeometryOutputVertices        = a_vkLimits.maxGeometryOutputVertices;
    limits.maxGeometryTotalOutputComponents = a_vkLimits.maxGeometryTotalOutputComponents;

    limits.maxFragmentInputComponents         = a_vkLimits.maxFragmentInputComponents;
    limits.maxFragmentOutputAttachments       = a_vkLimits.maxFragmentOutputAttachments;
    limits.maxFragmentDualSrcAttachments      = a_vkLimits.maxFragmentDualSrcAttachments;
    limits.maxFragmentCombinedOutputResources = a_vkLimits.maxFragmentCombinedOutputResources;

    limits.maxComputeSharedMemorySize     = a_vkLimits.maxComputeSharedMemorySize;
    limits.maxComputeWorkGroupCount[0]    = a_vkLimits.maxComputeWorkGroupCount[0];
    limits.maxComputeWorkGroupCount[1]    = a_vkLimits.maxComputeWorkGroupCount[1];
    limits.maxComputeWorkGroupCount[2]    = a_vkLimits.maxComputeWorkGroupCount[2];
    limits.maxComputeWorkGroupInvocations = a_vkLimits.maxComputeWorkGroupInvocations;
    limits.maxComputeWorkGroupSize[0]     = a_vkLimits.maxComputeWorkGroupSize[0];
    limits.maxComputeWorkGroupSize[1]     = a_vkLimits.maxComputeWorkGroupSize[1];
    limits.maxComputeWorkGroupSize[2]     = a_vkLimits.maxComputeWorkGroupSize[2];

    limits.subPixelPrecisionBits    = a_vkLimits.subPixelPrecisionBits;
    limits.subTexelPrecisionBits    = a_vkLimits.subTexelPrecisionBits;
    limits.mipmapPrecisionBits      = a_vkLimits.mipmapPrecisionBits;
    limits.maxDrawIndexedIndexValue = a_vkLimits.maxDrawIndexedIndexValue;
    limits.maxDrawIndirectCount     = a_vkLimits.maxDrawIndirectCount;
    limits.maxSamplerLodBias        = a_vkLimits.maxSamplerLodBias;
    limits.maxSamplerAnisotropy     = a_vkLimits.maxSamplerAnisotropy;

    limits.maxViewports             = a_vkLimits.maxViewports;
    limits.maxViewportDimensions[0] = a_vkLimits.maxViewportDimensions[0];
    limits.maxViewportDimensions[1] = a_vkLimits.maxViewportDimensions[1];
    limits.viewportBoundsRange[0]   = a_vkLimits.viewportBoundsRange[0];
    limits.viewportBoundsRange[1]   = a_vkLimits.viewportBoundsRange[1];
    limits.viewportSubPixelBits     = a_vkLimits.viewportSubPixelBits;

    limits.minMemoryMapAlignment           = a_vkLimits.minMemoryMapAlignment;
    limits.minTexelBufferOffsetAlignment   = a_vkLimits.minTexelBufferOffsetAlignment;
    limits.minUniformBufferOffsetAlignment = a_vkLimits.minUniformBufferOffsetAlignment;
    limits.minStorageBufferOffsetAlignment = a_vkLimits.minStorageBufferOffsetAlignment;
    limits.minTexelOffset                  = a_vkLimits.minTexelOffset;
    limits.maxTexelOffset                  = a_vkLimits.maxTexelOffset;
    limits.minTexelGatherOffset            = a_vkLimits.minTexelGatherOffset;
    limits.maxTexelGatherOffset            = a_vkLimits.maxTexelGatherOffset;
    limits.minInterpolationOffset          = a_vkLimits.minInterpolationOffset;
    limits.maxInterpolationOffset          = a_vkLimits.maxInterpolationOffset;
    limits.subPixelInterpolationOffsetBits = a_vkLimits.subPixelInterpolationOffsetBits;
    
    limits.maxFramebufferWidth  = a_vkLimits.maxFramebufferWidth;
    limits.maxFramebufferHeight = a_vkLimits.maxFramebufferHeight;
    limits.maxFramebufferLayers = a_vkLimits.maxFramebufferLayers;
    limits.maxColorAttachments = a_vkLimits.maxColorAttachments;
    limits.framebufferColorSampleCounts   = OCRA::SampleCount(a_vkLimits.framebufferColorSampleCounts + 1);
    limits.framebufferDepthSampleCounts   = OCRA::SampleCount(a_vkLimits.framebufferDepthSampleCounts + 1);
    limits.framebufferStencilSampleCounts = OCRA::SampleCount(a_vkLimits.framebufferStencilSampleCounts + 1);
    limits.framebufferNoAttachmentsSampleCounts = OCRA::SampleCount(a_vkLimits.framebufferNoAttachmentsSampleCounts + 1);

    limits.sampledImageColorSampleCounts   = OCRA::SampleCount(a_vkLimits.sampledImageColorSampleCounts + 1);
    limits.sampledImageIntegerSampleCounts = OCRA::SampleCount(a_vkLimits.sampledImageIntegerSampleCounts + 1);
    limits.sampledImageDepthSampleCounts   = OCRA::SampleCount(a_vkLimits.sampledImageDepthSampleCounts + 1);
    limits.sampledImageStencilSampleCounts = OCRA::SampleCount(a_vkLimits.sampledImageStencilSampleCounts + 1);
    limits.storageImageSampleCounts        = OCRA::SampleCount(a_vkLimits.storageImageSampleCounts + 1);
    limits.maxSampleMaskWords = a_vkLimits.maxSampleMaskWords;

    limits.timestampComputeAndGraphics = a_vkLimits.timestampComputeAndGraphics;
    limits.timestampPeriod = a_vkLimits.timestampPeriod;

    limits.maxClipDistances = a_vkLimits.maxClipDistances;
    limits.maxCullDistances = a_vkLimits.maxCullDistances;
    limits.maxCombinedClipAndCullDistances = a_vkLimits.maxCombinedClipAndCullDistances;

    limits.discreteQueuePriorities = a_vkLimits.discreteQueuePriorities;
    limits.pointSizeRange[0] = a_vkLimits.pointSizeRange[0];
    limits.pointSizeRange[1] = a_vkLimits.pointSizeRange[1];
    limits.lineWidthRange[0] = a_vkLimits.lineWidthRange[0];
    limits.lineWidthRange[1] = a_vkLimits.lineWidthRange[1];
    limits.pointSizeGranularity = a_vkLimits.pointSizeGranularity;
    limits.lineWidthGranularity = a_vkLimits.lineWidthGranularity;
    limits.strictLines = a_vkLimits.strictLines;

    limits.standardSampleLocations = a_vkLimits.standardSampleLocations;
    limits.optimalBufferCopyOffsetAlignment = a_vkLimits.optimalBufferCopyOffsetAlignment;
    limits.optimalBufferCopyRowPitchAlignment = a_vkLimits.optimalBufferCopyRowPitchAlignment;
    limits.nonCoherentAtomSize = a_vkLimits.nonCoherentAtomSize;
    return limits;
}

static inline auto GetSparseProperties(const VkPhysicalDeviceSparseProperties& a_SparseProperties) {
    PhysicalDeviceSparseProperties sparseProperties{};
    sparseProperties.residencyAlignedMipSize = a_SparseProperties.residencyAlignedMipSize;
    sparseProperties.residencyNonResidentStrict = a_SparseProperties.residencyNonResidentStrict;
    sparseProperties.residencyStandard2DBlockShape = a_SparseProperties.residencyStandard2DBlockShape;
    sparseProperties.residencyStandard2DMultisampleBlockShape = a_SparseProperties.residencyStandard2DMultisampleBlockShape;
    sparseProperties.residencyStandard3DBlockShape = a_SparseProperties.residencyStandard3DBlockShape;
    return sparseProperties;
}

const MemoryProperties GetMemoryProperties(const Handle& a_PhysicalDevice)
{
    VkPhysicalDeviceMemoryProperties p;
    vkGetPhysicalDeviceMemoryProperties(*a_PhysicalDevice, &p);
    MemoryProperties memoryProperties;
    memoryProperties.memoryHeaps.resize(p.memoryHeapCount);
    for (auto i = 0u; i < p.memoryHeapCount; ++i) {
        const auto& mh = p.memoryHeaps[i];
        auto& omh = memoryProperties.memoryHeaps.at(i);
        omh.size = mh.size;
        omh.flags |= mh.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT ? MemoryHeapFlagBits::DeviceLocal : MemoryHeapFlagBits::None;
        omh.flags |= mh.flags & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT ? MemoryHeapFlagBits::MultiInstance : MemoryHeapFlagBits::None;
    }
    memoryProperties.memoryTypes.resize(p.memoryTypeCount);
    for (auto i = 0u; i < p.memoryTypeCount; ++i) {
        const auto& mt = p.memoryTypes[i];
        auto& omt = memoryProperties.memoryTypes.at(i);
        omt.heapIndex = mt.heapIndex;
        omt.propertyFlags |= mt.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT  ? MemoryPropertyFlagBits::DeviceLocal     : MemoryPropertyFlagBits::None;
        omt.propertyFlags |= mt.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT  ? MemoryPropertyFlagBits::HostVisible     : MemoryPropertyFlagBits::None;
        omt.propertyFlags |= mt.propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ? MemoryPropertyFlagBits::HostCoherent    : MemoryPropertyFlagBits::None;
        omt.propertyFlags |= mt.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT   ? MemoryPropertyFlagBits::LazilyAllocated : MemoryPropertyFlagBits::None;
        omt.propertyFlags |= mt.propertyFlags & VK_MEMORY_PROPERTY_PROTECTED_BIT     ? MemoryPropertyFlagBits::Protected       : MemoryPropertyFlagBits::None;
    }
    return memoryProperties;
}

const PhysicalDeviceProperties GetProperties(const Handle& a_PhysicalDevice)
{
    PhysicalDeviceProperties properties;
    VkPhysicalDeviceProperties vkProperties{};
    vkGetPhysicalDeviceProperties(*a_PhysicalDevice, &vkProperties);
    switch (vkProperties.deviceType)
    {
    case VK_PHYSICAL_DEVICE_TYPE_OTHER:
        properties.deviceType = PhysicalDeviceType::Other;
        break;
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
        properties.deviceType = PhysicalDeviceType::IntegratedGpu;
        break;
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
        properties.deviceType = PhysicalDeviceType::DiscreteGpu;
        break;
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
        properties.deviceType = PhysicalDeviceType::VirtualGpu;
        break;
    case VK_PHYSICAL_DEVICE_TYPE_CPU:
        properties.deviceType = PhysicalDeviceType::Cpu;
        break;
    }
    switch (vkProperties.vendorID)
    {
    case 0x1002:
        properties.vendorName = "AMD";
        break;
    case 0x1010:
        properties.vendorName = "ImgTec";
        break;
    case 0x10DE:
        properties.vendorName = "NVIDIA";
        break;
    case 0x13B5:
        properties.vendorName = "ARM";
        break;
    case 0x5143:
        properties.vendorName = "Qualcomm";
        break;
    case 0x8086:
        properties.vendorName = "INTEL";
        break;
    default:
        break;
    }
    std::copy(vkProperties.pipelineCacheUUID, vkProperties.pipelineCacheUUID + 16, properties.pipelineCacheUUID.data());
    properties.deviceID = vkProperties.deviceID;
    properties.vendorID = vkProperties.vendorID;
    properties.apiVersion = vkProperties.apiVersion;
    properties.driverVersion = vkProperties.driverVersion;
    properties.deviceName = vkProperties.deviceName;
    properties.sparseProperties = GetSparseProperties(vkProperties.sparseProperties);
    properties.limits = GetLimits(vkProperties.limits);
    return properties;
}

const PhysicalDeviceFeatures GetFeatures(const Handle& a_PhysicalDevice)
{
    VkPhysicalDeviceFeatures vkFeatures{};
    vkGetPhysicalDeviceFeatures(*a_PhysicalDevice, &vkFeatures);
    PhysicalDeviceFeatures features;
    features.robustBufferAccess = vkFeatures.robustBufferAccess;
    features.fullDrawIndexUint32 = vkFeatures.fullDrawIndexUint32;
    features.imageCubeArray = vkFeatures.imageCubeArray;
    features.independentBlend = vkFeatures.independentBlend;
    features.geometryShader = vkFeatures.geometryShader;
    features.tessellationShader = vkFeatures.tessellationShader;
    features.sampleRateShading = vkFeatures.sampleRateShading;
    features.dualSrcBlend = vkFeatures.dualSrcBlend;
    features.logicOp = vkFeatures.logicOp;
    features.multiDrawIndirect = vkFeatures.multiDrawIndirect;
    features.drawIndirectFirstInstance = vkFeatures.drawIndirectFirstInstance;
    features.depthClamp = vkFeatures.depthClamp;
    features.depthBiasClamp = vkFeatures.depthBiasClamp;
    features.fillModeNonSolid = vkFeatures.fillModeNonSolid;
    features.depthBounds = vkFeatures.depthBounds;
    features.wideLines = vkFeatures.wideLines;
    features.largePoints = vkFeatures.largePoints;
    features.alphaToOne = vkFeatures.alphaToOne;
    features.multiViewport = vkFeatures.multiViewport;
    features.samplerAnisotropy = vkFeatures.samplerAnisotropy;
    features.textureCompressionETC2 = vkFeatures.textureCompressionETC2;
    features.textureCompressionASTC_LDR = vkFeatures.textureCompressionASTC_LDR;
    features.textureCompressionBC = vkFeatures.textureCompressionBC;
    features.occlusionQueryPrecise = vkFeatures.occlusionQueryPrecise;
    features.pipelineStatisticsQuery = vkFeatures.pipelineStatisticsQuery;
    features.vertexPipelineStoresAndAtomics = vkFeatures.vertexPipelineStoresAndAtomics;
    features.fragmentStoresAndAtomics = vkFeatures.fragmentStoresAndAtomics;
    features.shaderTessellationAndGeometryPointSize = vkFeatures.shaderTessellationAndGeometryPointSize;
    features.shaderImageGatherExtended = vkFeatures.shaderImageGatherExtended;
    features.shaderStorageImageExtendedFormats = vkFeatures.shaderStorageImageExtendedFormats;
    features.shaderStorageImageMultisample = vkFeatures.shaderStorageImageMultisample;
    features.shaderStorageImageReadWithoutFormat = vkFeatures.shaderStorageImageReadWithoutFormat;
    features.shaderStorageImageWriteWithoutFormat = vkFeatures.shaderStorageImageWriteWithoutFormat;
    features.shaderUniformBufferArrayDynamicIndexing = vkFeatures.shaderUniformBufferArrayDynamicIndexing;
    features.shaderSampledImageArrayDynamicIndexing = vkFeatures.shaderSampledImageArrayDynamicIndexing;
    features.shaderStorageBufferArrayDynamicIndexing = vkFeatures.shaderStorageBufferArrayDynamicIndexing;
    features.shaderStorageImageArrayDynamicIndexing = vkFeatures.shaderStorageImageArrayDynamicIndexing;
    features.shaderClipDistance = vkFeatures.shaderClipDistance;
    features.shaderCullDistance = vkFeatures.shaderCullDistance;
    features.shaderFloat64 = vkFeatures.shaderFloat64;
    features.shaderInt64 = vkFeatures.shaderInt64;
    features.shaderInt16 = vkFeatures.shaderInt16;
    features.shaderResourceResidency = vkFeatures.shaderResourceResidency;
    features.shaderResourceMinLod = vkFeatures.shaderResourceMinLod;
    features.sparseBinding = vkFeatures.sparseBinding;
    features.sparseResidencyBuffer = vkFeatures.sparseResidencyBuffer;
    features.sparseResidencyImage2D = vkFeatures.sparseResidencyImage2D;
    features.sparseResidencyImage3D = vkFeatures.sparseResidencyImage3D;
    features.sparseResidency2Samples = vkFeatures.sparseResidency2Samples;
    features.sparseResidency4Samples = vkFeatures.sparseResidency4Samples;
    features.sparseResidency8Samples = vkFeatures.sparseResidency8Samples;
    features.sparseResidency16Samples = vkFeatures.sparseResidency16Samples;
    features.sparseResidencyAliased = vkFeatures.sparseResidencyAliased;
    features.variableMultisampleRate = vkFeatures.variableMultisampleRate;
    features.inheritedQueries = vkFeatures.inheritedQueries;
    return features;
}

const std::vector<QueueFamilyProperties> GetQueueFamilyProperties(const Handle& a_PhysicalDevice)
{
    uint32_t queueCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(*a_PhysicalDevice, &queueCount, nullptr);
    std::vector<VkQueueFamilyProperties> vkQueueProperties{ queueCount };
    std::vector<QueueFamilyProperties>   queueProperties{ queueCount };
    vkGetPhysicalDeviceQueueFamilyProperties(*a_PhysicalDevice, &queueCount, vkQueueProperties.data());
    for (auto i = 0u; i < queueCount; ++i) {
        const auto& qp = vkQueueProperties.at(i);
        auto& oqp = queueProperties.at(i);
        oqp.queueCount = qp.queueCount;
        oqp.timestampValidBits = qp.timestampValidBits;
        oqp.minImageTransferGranularity = OCRA::Extent3D(
            qp.minImageTransferGranularity.width,
            qp.minImageTransferGranularity.height,
            qp.minImageTransferGranularity.depth);
        oqp.queueFlags |= qp.queueFlags & VK_QUEUE_GRAPHICS_BIT       ? QueueFlagsBits::Graphics      : QueueFlagsBits::None;
        oqp.queueFlags |= qp.queueFlags & VK_QUEUE_COMPUTE_BIT        ? QueueFlagsBits::Compute       : QueueFlagsBits::None;
        oqp.queueFlags |= qp.queueFlags & VK_QUEUE_TRANSFER_BIT       ? QueueFlagsBits::Transfer      : QueueFlagsBits::None;
        oqp.queueFlags |= qp.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT ? QueueFlagsBits::SparseBinding : QueueFlagsBits::None;
        oqp.queueFlags |= qp.queueFlags & VK_QUEUE_PROTECTED_BIT      ? QueueFlagsBits::Protected     : QueueFlagsBits::None;
    }
    return queueProperties;
}

//const auto GetImageFormatProperties(const Handle& a_PhysicalDevice)
//{
//    vkGetPhysicalDeviceImageFormatProperties()
//}

uint32_t FindQueueFamily(const PhysicalDevice::Handle& a_PhysicalDevice, const QueueFlags& a_QueueFlags)
{
    auto& queueProperties = GetQueueFamilyProperties(a_PhysicalDevice);
    for (auto familyIndex = 0u; familyIndex < queueProperties.size(); ++familyIndex) {
        //check if a_QueueFlags is a subset of queueFlags
        if ((queueProperties.at(familyIndex).queueFlags & a_QueueFlags) == a_QueueFlags)
            return familyIndex;
    }
    return std::numeric_limits<uint32_t>::infinity();
}

uint32_t FindMemoryType(const PhysicalDevice::Handle& a_PhysicalDevice, const MemoryPropertyFlags& a_MemoryProperties)
{
    auto& memoryProperties = PhysicalDevice::GetMemoryProperties(a_PhysicalDevice);
    for (auto memoryTypeIndex = 0u; memoryTypeIndex < memoryProperties.memoryTypes.size(); ++memoryTypeIndex) {
        if (memoryProperties.memoryTypes.at(memoryTypeIndex).propertyFlags == a_MemoryProperties)
            return memoryTypeIndex;
    }
    //Couldn't find optimal memory type, take any fitting type
    for (auto memoryTypeIndex = 0u; memoryTypeIndex < memoryProperties.memoryTypes.size(); ++memoryTypeIndex) {
        if ((memoryProperties.memoryTypes.at(memoryTypeIndex).propertyFlags & a_MemoryProperties) != 0)
            return memoryTypeIndex;
    }
    return std::numeric_limits<uint32_t>::infinity();
}
}
