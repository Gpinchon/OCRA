#include <OCRA/PhysicalDevice.hpp>

#include <GL/PhysicalDevice.hpp>
#include <GL/Instance.hpp>
#include <GL/glew.h>

#include <stdexcept>
#include <iostream>
#include <sstream>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Instance);

namespace OCRA::PhysicalDevice
{
using Command = std::function<void()>;

static inline auto GetInteger(const GLenum& state)
{
    GLint val;
    glGetIntegerv(state, &val);
    return val;
}

static inline auto GetInteger(const GLenum& a_State, const GLuint& a_Index)
{
    GLint val;
    glGetIntegeri_v(a_State, a_Index, &val);
    return val;
}

static inline auto GetPhysicalDeviceLimitsGL()
{
    Limits limits{};
    memset(&limits, 0, sizeof(Limits));
    limits.maxImageDimension1D = GetInteger(GL_MAX_TEXTURE_SIZE);
    limits.maxImageDimension2D = GetInteger(GL_MAX_RECTANGLE_TEXTURE_SIZE);
    limits.maxImageDimension3D = GetInteger(GL_MAX_3D_TEXTURE_SIZE);
    limits.maxImageDimensionCube = GetInteger(GL_MAX_CUBE_MAP_TEXTURE_SIZE);
    limits.maxImageArrayLayers = GetInteger(GL_MAX_ARRAY_TEXTURE_LAYERS);
    limits.maxTexelBufferElements = GetInteger(GL_MAX_TEXTURE_BUFFER_SIZE);
    limits.maxUniformBufferRange = GetInteger(GL_MAX_UNIFORM_BLOCK_SIZE);
    limits.maxStorageBufferRange = GetInteger(GL_MAX_SHADER_STORAGE_BLOCK_SIZE);

    limits.maxPushConstantsSize = 256; //typical in Vulkan
    limits.maxMemoryAllocationCount = 4096; //typical in Vulkan
    limits.maxSamplerAllocationCount = 4000; //typical in Vulkan

    limits.bufferImageGranularity = 0; //no granularity imposed by OGL
    limits.sparseAddressSpaceSize = (std::numeric_limits<uint64_t>::max)();

    limits.maxBoundDescriptorSets = OCRA_GL_MAX_BOUND_DESCRIPTOR_SETS;
    limits.maxPerStageDescriptorSamplers = 4000;
    limits.maxPerStageDescriptorUniformBuffers = GetInteger(GL_MAX_UNIFORM_BUFFER_BINDINGS) / 6; //divided by the number of shader stages
    limits.maxPerStageDescriptorStorageBuffers = GetInteger(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS) / 6; //divided by the number of shader stages
    limits.maxPerStageDescriptorSampledImages = GetInteger(GL_MAX_IMAGE_SAMPLES);
    limits.maxPerStageDescriptorStorageImages = GetInteger(GL_MAX_IMAGE_UNITS);
    limits.maxPerStageDescriptorInputAttachments = (std::numeric_limits<uint32_t>::max)();
    limits.maxPerStageResources = (std::numeric_limits<uint32_t>::max)();

    limits.maxDescriptorSetSamplers = (std::numeric_limits<uint32_t>::max)();
    limits.maxDescriptorSetUniformBuffers = (std::numeric_limits<uint32_t>::max)();
    limits.maxDescriptorSetUniformBuffersDynamic = (std::numeric_limits<uint32_t>::max)();
    limits.maxDescriptorSetStorageBuffers = (std::numeric_limits<uint32_t>::max)();
    limits.maxDescriptorSetStorageBuffersDynamic = (std::numeric_limits<uint32_t>::max)();
    limits.maxDescriptorSetSampledImages = (std::numeric_limits<uint32_t>::max)();
    limits.maxDescriptorSetStorageImages = (std::numeric_limits<uint32_t>::max)();
    limits.maxDescriptorSetInputAttachments = (std::numeric_limits<uint32_t>::max)();

    limits.maxVertexInputAttributes = GetInteger(GL_MAX_VERTEX_ATTRIBS);
    limits.maxVertexInputBindings = GetInteger(GL_MAX_VERTEX_ATTRIB_BINDINGS);
    limits.maxVertexInputAttributeOffset = GetInteger(GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET);
    limits.maxVertexInputBindingStride = GetInteger(GL_MAX_VERTEX_ATTRIB_STRIDE);
    limits.maxVertexOutputComponents = GetInteger(GL_MAX_VERTEX_OUTPUT_COMPONENTS);

    limits.maxTessellationGenerationLevel = GetInteger(GL_MAX_TESS_GEN_LEVEL);
    limits.maxTessellationPatchSize = GetInteger(GL_MAX_PATCH_VERTICES);
    limits.maxTessellationControlPerVertexInputComponents = GetInteger(GL_MAX_TESS_CONTROL_INPUT_COMPONENTS);
    limits.maxTessellationControlPerVertexOutputComponents = GetInteger(GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS);
    limits.maxTessellationControlPerPatchOutputComponents = GetInteger(GL_MAX_TESS_PATCH_COMPONENTS);
    limits.maxTessellationControlTotalOutputComponents = GetInteger(GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS);
    limits.maxTessellationEvaluationInputComponents = GetInteger(GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS);
    limits.maxTessellationEvaluationOutputComponents = GetInteger(GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS);

    limits.maxGeometryShaderInvocations = GetInteger(GL_MAX_GEOMETRY_SHADER_INVOCATIONS);
    limits.maxGeometryInputComponents = GetInteger(GL_MAX_GEOMETRY_INPUT_COMPONENTS);
    limits.maxGeometryOutputComponents = GetInteger(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS);
    limits.maxGeometryOutputVertices = GetInteger(GL_MAX_GEOMETRY_OUTPUT_VERTICES);
    limits.maxGeometryTotalOutputComponents = GetInteger(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS);

    limits.maxFragmentInputComponents = GetInteger(GL_MAX_FRAGMENT_INPUT_COMPONENTS);
    limits.maxFragmentOutputAttachments = GetInteger(GL_MAX_DRAW_BUFFERS);
    limits.maxFragmentDualSrcAttachments = GetInteger(GL_MAX_DUAL_SOURCE_DRAW_BUFFERS);
    limits.maxFragmentCombinedOutputResources = GetInteger(GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES);

    limits.maxComputeSharedMemorySize = GetInteger(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE);
    limits.maxComputeWorkGroupCount[0] = GetInteger(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0);
    limits.maxComputeWorkGroupCount[1] = GetInteger(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1);
    limits.maxComputeWorkGroupCount[2] = GetInteger(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2);
    limits.maxComputeWorkGroupInvocations = GetInteger(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS);
    limits.maxComputeWorkGroupSize[0] = GetInteger(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0);
    limits.maxComputeWorkGroupSize[1] = GetInteger(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1);
    limits.maxComputeWorkGroupSize[2] = GetInteger(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2);
    limits.subPixelPrecisionBits = GetInteger(GL_SUBPIXEL_BITS);
    //limits.subTexelPrecisionBits = GetInteger(    );
    //limits.mipmapPrecisionBits = GetInteger(    );
    limits.maxDrawIndexedIndexValue = GetInteger(GL_MAX_ELEMENT_INDEX);
    //limits.maxDrawIndirectCount = GetInteger(    );
    limits.maxSamplerLodBias = GetInteger(GL_MAX_TEXTURE_LOD_BIAS);
    limits.maxSamplerAnisotropy = GetInteger(GL_MAX_TEXTURE_MAX_ANISOTROPY);

    limits.maxViewports = GetInteger(GL_MAX_VIEWPORTS);
    {
        GLint viewportDims[2];
        glGetIntegerv(GL_MAX_VIEWPORT_DIMS, viewportDims);
        limits.maxViewportDimensions[0] = viewportDims[0];
        limits.maxViewportDimensions[1] = viewportDims[1];
    }
    {
        GLint viewportBoundsRange[2];
        glGetIntegerv(GL_VIEWPORT_BOUNDS_RANGE, viewportBoundsRange);
        limits.viewportBoundsRange[0] = viewportBoundsRange[0];
        limits.viewportBoundsRange[1] = viewportBoundsRange[1];
    }
    limits.viewportSubPixelBits = GetInteger(GL_VIEWPORT_SUBPIXEL_BITS);

    limits.minMemoryMapAlignment = GetInteger(GL_MIN_MAP_BUFFER_ALIGNMENT);
    //limits.minTexelBufferOffsetAlignment = GetInteger(    );
    //limits.minUniformBufferOffsetAlignment = GetInteger(    );
    //limits.minStorageBufferOffsetAlignment = GetInteger(    );
    limits.minTexelOffset = GetInteger(GL_MIN_PROGRAM_TEXEL_OFFSET);
    limits.maxTexelOffset = GetInteger(GL_MAX_PROGRAM_TEXEL_OFFSET);
    limits.minTexelGatherOffset = GetInteger(GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET);
    limits.maxTexelGatherOffset = GetInteger(GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET);
    limits.minInterpolationOffset = GetInteger(GL_MIN_FRAGMENT_INTERPOLATION_OFFSET);
    limits.maxInterpolationOffset = GetInteger(GL_MAX_FRAGMENT_INTERPOLATION_OFFSET);
    //limits.subPixelInterpolationOffsetBits = GetInteger(    );
    
    limits.maxFramebufferWidth = GetInteger(GL_MAX_FRAMEBUFFER_WIDTH);
    limits.maxFramebufferHeight = GetInteger(GL_MAX_FRAMEBUFFER_HEIGHT);
    limits.maxFramebufferLayers = GetInteger(GL_MAX_FRAMEBUFFER_LAYERS);
    limits.framebufferColorSampleCounts = OCRA::SampleCount(GetInteger(GL_COLOR_SAMPLES_NV));
    limits.framebufferDepthSampleCounts = OCRA::SampleCount(GetInteger(GL_DEPTH_SAMPLES_NV));
    limits.framebufferStencilSampleCounts = OCRA::SampleCount(GetInteger(GL_STENCIL_SAMPLES_NV));
    limits.framebufferNoAttachmentsSampleCounts = OCRA::SampleCount(GetInteger(GL_MAX_FRAMEBUFFER_SAMPLES));
    limits.maxColorAttachments = GetInteger(GL_MAX_COLOR_ATTACHMENTS);

    limits.sampledImageColorSampleCounts = OCRA::SampleCount(GetInteger(GL_MAX_COLOR_TEXTURE_SAMPLES));
    limits.sampledImageIntegerSampleCounts = OCRA::SampleCount(GetInteger(GL_MAX_INTEGER_SAMPLES));
    limits.sampledImageDepthSampleCounts = OCRA::SampleCount(GetInteger(GL_MAX_DEPTH_TEXTURE_SAMPLES));
    //limits.sampledImageStencilSampleCounts = GetInteger(    );
    //limits.storageImageSampleCounts = GetInteger(    );
    limits.maxSampleMaskWords = GetInteger(GL_MAX_SAMPLE_MASK_WORDS);

    //limits.timestampComputeAndGraphics = GetInteger(    );
    //limits.timestampPeriod = GetInteger(    );

    limits.maxClipDistances = GetInteger(GL_MAX_CLIP_DISTANCES);
    limits.maxCullDistances = GetInteger(GL_MAX_CULL_DISTANCES);
    limits.maxCombinedClipAndCullDistances = GetInteger(GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES);

    //limits.discreteQueuePriorities = GetInteger(    );
    {
        GLfloat pointSizeRange[2];
        glGetFloatv(GL_VIEWPORT_BOUNDS_RANGE, pointSizeRange);
        limits.pointSizeRange[0] = pointSizeRange[0];
        limits.pointSizeRange[1] = pointSizeRange[1];
    }
    {
        GLfloat lineWidthRange[2];
        glGetFloatv(GL_VIEWPORT_BOUNDS_RANGE, lineWidthRange);
        limits.lineWidthRange[0] = lineWidthRange[0];
        limits.lineWidthRange[1] = lineWidthRange[1];
    }
    limits.pointSizeGranularity = GetInteger(GL_POINT_SIZE_GRANULARITY);
    limits.lineWidthGranularity = GetInteger(GL_LINE_WIDTH_GRANULARITY);
    //limits.strictLines = GetInteger(    );

    //limits.standardSampleLocations = GetInteger(    );
    //limits.optimalBufferCopyOffsetAlignment = GetInteger(    );
    //limits.optimalBufferCopyRowPitchAlignment = GetInteger(    );
    //limits.nonCoherentAtomSize = GetInteger(    );
    return limits;
}

static inline auto GetPhysicalDevicePropertiesGL()
{
    Properties properties;
    properties.apiVersion = GetInteger(GL_MAJOR_VERSION) * 100 + GetInteger(GL_MINOR_VERSION) * 10;
    //properties.driverVersion = (char*)glGetString(GL_VERSION);
    properties.vendorName = (char*)glGetString(GL_VENDOR);
    properties.deviceName = (char*)glGetString(GL_RENDERER);
    properties.deviceType = Type::Other;
    //properties.pipelineCacheUUID = 0;
    properties.limits = GetPhysicalDeviceLimitsGL();
    //properties.sparseProperties = 0;
    return properties;
}

static inline auto GetPhysicalDeviceFeaturesGL(const Properties& a_Properties)
{
    Features features;
    features.robustBufferAccess = GLEW_ARB_robust_buffer_access_behavior;
    features.fullDrawIndexUint32 = a_Properties.limits.maxDrawIndexedIndexValue == (std::numeric_limits<uint32_t>::max)();
    features.imageCubeArray = GLEW_ARB_texture_cube_map_array;
    features.independentBlend = GLEW_ARB_blend_func_extended;
    features.geometryShader = GLEW_ARB_geometry_shader4;
    features.tessellationShader = GLEW_ARB_tessellation_shader;
    features.sampleRateShading = GLEW_ARB_sample_shading;
    features.dualSrcBlend = GLEW_ARB_draw_buffers_blend;
    features.logicOp = true; //supported by default
    features.multiDrawIndirect = GLEW_ARB_multi_draw_indirect;
    features.drawIndirectFirstInstance = GLEW_ARB_base_instance;
    features.depthClamp = GLEW_ARB_depth_clamp;
    features.depthBiasClamp = GLEW_ARB_polygon_offset_clamp;
    features.fillModeNonSolid = true; //supported by default
    features.depthBounds = GLEW_EXT_depth_bounds_test;
    features.wideLines = a_Properties.limits.lineWidthRange[1] > 1;
    features.largePoints = a_Properties.limits.pointSizeRange[1] > 1;
    features.alphaToOne = GLEW_ARB_multisample;
    features.multiViewport = GLEW_ARB_viewport_array;
    features.samplerAnisotropy = GLEW_ARB_texture_filter_anisotropic;
    features.textureCompressionETC2 = GLEW_ARB_ES3_compatibility;
    features.textureCompressionASTC_LDR = GLEW_KHR_texture_compression_astc_ldr;
    features.textureCompressionBC = GLEW_EXT_texture_compression_s3tc;
    features.occlusionQueryPrecise = GLEW_ARB_occlusion_query;
    features.pipelineStatisticsQuery = GLEW_ARB_pipeline_statistics_query;
    features.vertexPipelineStoresAndAtomics = GLEW_ARB_shader_atomic_counters;
    features.fragmentStoresAndAtomics = GLEW_ARB_shader_atomic_counters;
    features.shaderTessellationAndGeometryPointSize = true; //supported by default
    features.shaderImageGatherExtended = GLEW_ARB_texture_gather;
    features.shaderStorageImageExtendedFormats = false; //not supported by OpenGL
    features.shaderStorageImageMultisample = GLEW_ARB_multisample;
    features.shaderStorageImageReadWithoutFormat = false; //not supported by OpenGL
    features.shaderStorageImageWriteWithoutFormat = false; //not supported by OpenGL
    features.shaderUniformBufferArrayDynamicIndexing = false; //not supported by OpenGL
    features.shaderSampledImageArrayDynamicIndexing = false; //not supported by OpenGL
    features.shaderStorageBufferArrayDynamicIndexing = false; //not supported by OpenGL
    features.shaderStorageImageArrayDynamicIndexing = false; //not supported by OpenGL
    features.shaderClipDistance = true; //supported by default
    features.shaderCullDistance = GLEW_ARB_cull_distance;
    features.shaderFloat64 = GLEW_ARB_gpu_shader_fp64;
    features.shaderInt64 = GLEW_ARB_gpu_shader_int64;
    features.shaderInt16 = GLEW_AMD_gpu_shader_int16;
    features.shaderResourceResidency = GLEW_ARB_bindless_texture;
    features.shaderResourceMinLod = GLEW_ARB_shader_texture_lod;
    features.sparseBinding = GLEW_ARB_sparse_texture && GLEW_ARB_sparse_buffer;
    features.sparseResidencyBuffer = GLEW_ARB_sparse_buffer;
    features.sparseResidencyImage2D = GLEW_ARB_sparse_texture;
    features.sparseResidencyImage3D = GLEW_ARB_sparse_texture;
    features.sparseResidency2Samples = GLEW_ARB_sparse_texture2;
    features.sparseResidency4Samples = GLEW_ARB_sparse_texture2;
    features.sparseResidency8Samples = GLEW_ARB_sparse_texture2;
    features.sparseResidency16Samples = GLEW_ARB_sparse_texture2;
    features.sparseResidencyAliased = false; //not supported by OpenGL
    features.variableMultisampleRate = true; //TODO make sure it's supported
    features.inheritedQueries = false; //not supported by OpenGL
    return features;
}

static inline auto GetMemoryPropertiesGL()
{
    MemoryProperties memoryProperties;
    memoryProperties.memoryHeaps.resize(1);
    memoryProperties.memoryHeaps.at(0).size = static_cast<size_t>(GetInteger(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX)) * 1000;
    memoryProperties.memoryHeaps.at(0).flags = MemoryHeapFlagBits::DeviceLocal;

    memoryProperties.memoryTypes.resize(11);
    memoryProperties.memoryTypes.at(0).heapIndex = 0;
    memoryProperties.memoryTypes.at(0).propertyFlags = MemoryPropertyFlagBits::None;
    memoryProperties.memoryTypes.at(1).heapIndex = 0;
    memoryProperties.memoryTypes.at(1).propertyFlags = MemoryPropertyFlagBits::HostVisible | MemoryPropertyFlagBits::HostCoherent;
    memoryProperties.memoryTypes.at(2).heapIndex = 0;
    memoryProperties.memoryTypes.at(2).propertyFlags = MemoryPropertyFlagBits::HostVisible | MemoryPropertyFlagBits::HostCached;
    memoryProperties.memoryTypes.at(3).heapIndex = 0;
    memoryProperties.memoryTypes.at(3).propertyFlags = MemoryPropertyFlagBits::HostVisible | MemoryPropertyFlagBits::HostCached | MemoryPropertyFlagBits::HostCoherent;
    memoryProperties.memoryTypes.at(4).heapIndex = 0;
    memoryProperties.memoryTypes.at(4).propertyFlags = MemoryPropertyFlagBits::DeviceLocal;
    memoryProperties.memoryTypes.at(5).heapIndex = 0;
    memoryProperties.memoryTypes.at(5).propertyFlags = MemoryPropertyFlagBits::DeviceLocal | MemoryPropertyFlagBits::HostVisible | MemoryPropertyFlagBits::HostCoherent;
    memoryProperties.memoryTypes.at(6).heapIndex = 0;
    memoryProperties.memoryTypes.at(6).propertyFlags = MemoryPropertyFlagBits::DeviceLocal | MemoryPropertyFlagBits::HostVisible | MemoryPropertyFlagBits::HostCached;
    memoryProperties.memoryTypes.at(7).heapIndex = 0;
    memoryProperties.memoryTypes.at(7).propertyFlags = MemoryPropertyFlagBits::DeviceLocal | MemoryPropertyFlagBits::HostVisible | MemoryPropertyFlagBits::HostCached | MemoryPropertyFlagBits::HostCoherent;
    memoryProperties.memoryTypes.at(8).heapIndex = 0;
    memoryProperties.memoryTypes.at(8).propertyFlags = MemoryPropertyFlagBits::DeviceLocal | MemoryPropertyFlagBits::LazilyAllocated;
    memoryProperties.memoryTypes.at(9).heapIndex = 0;
    memoryProperties.memoryTypes.at(9).propertyFlags = MemoryPropertyFlagBits::Protected;
    memoryProperties.memoryTypes.at(10).heapIndex = 0;
    memoryProperties.memoryTypes.at(10).propertyFlags = MemoryPropertyFlagBits::Protected | MemoryPropertyFlagBits::DeviceLocal;
    return memoryProperties;
}

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
        std::stringstream ss;
        ss << "GL CALLBACK : **GL ERROR * *\n" <<
            " type     = " << type << "\n" <<
            " severity = " << severity << "\n" <<
            " message  = " << message;
        std::cerr << ss.str() << std::endl;
        throw std::runtime_error(ss.str());
    }
}

Impl::Impl(const Instance::Handle& a_Instance)
    : instance(a_Instance)
{
    if (!GLEW_EXT_direct_state_access) throw std::runtime_error("Direct state access extension required !");
}

void Impl::GetProperties()
{
    PushCommand([this] {
#ifdef _DEBUG
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(MessageCallback, 0);
#endif _DEBUG
        properties = GetPhysicalDevicePropertiesGL();
        features = GetPhysicalDeviceFeaturesGL(properties);
        memoryProperties = GetMemoryPropertiesGL();
    }, true);
}

const MemoryProperties& GetMemoryProperties(const Handle& a_PhysicalDevice)
{
    return a_PhysicalDevice->memoryProperties;
}
const Properties& GetProperties(const Handle& a_PhysicalDevice)
{
    return a_PhysicalDevice->properties;
}
const Features& GetFeatures(const Handle& a_PhysicalDevice)
{
    return a_PhysicalDevice->features;
}
const std::vector<QueueFamilyProperties> GetQueueFamilyProperties(const Handle& a_PhysicalDevice)
{
    return { a_PhysicalDevice->queue.properties };
}
}
