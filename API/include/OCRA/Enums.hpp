#pragma once

namespace OCRA
{
enum class BlendOp {
    Add,
    Subtract,
    ReverseSubtract,
    Min,
    Max,
    MaxValue
};
enum class BlendFactor {
    Zero,
    One,
    SrcColor,
    OneMinusSrcColor,
    DstColor,
    OneMinusDstColor,
    SrcAlpha,
    OneMinusSrcAlpha,
    DstAlpha,
    OneMinusDstAlpha,
    ConstantColor,
    OneMinusConstantColor,
    ConstantAlpha,
    OneMinusConstantAlpha,
    SrcAlphaSaturate,
    Src1Color,
    OneMinusSrc1Color,
    Src1Alpha,
    OneMinusSrc1Alpha,
    MaxValue
};
enum class ColorSpace {
    Unknown = -1,
    Linear,
    sRGB,
    HDR10,
    MaxValue
};
enum class CommandBufferLevel {
    Unknown = -1,
    Primary,
    Secondary,
    MaxValue
};
enum class CompareOp {
    Unknown = -1,
    Never,
    Less,
    Equal,
    LessOrEqual,
    Greater,
    NotEqual,
    GreaterOrEqual,
    Always,
    MaxValue
};
enum class CullMode {
    None,
    Front,
    Back,
    FrontAndBack,
    MaxValue
};
enum class DescriptorType {
    Unknown = -1,
    SampledImage,
    StorageImage,
    UniformTexelBuffer,
    StorageTexelBuffer,
    UniformBuffer,
    StorageBuffer,
    UniformBufferDynamic,
    StorageBufferDynamic,
    InputAttachment,
    InlineUniformBlock,
    AccelerationStructure, //for RayTracing
    MutableValve,
    MaxValue
};
enum class FenceStatus {
    Signaled,
    Unsignaled
};
enum class Filter {
    Nearest,
    Linear,
    Cubic,
    MaxValue
};
enum class Format {
    Unknown = -1,
    /// <summary>
    /// Normalized Uint8 pixel types
    /// </summary>
    Uint8_Normalized_R,
    Uint8_Normalized_RG,
    Uint8_Normalized_RGB,
    Uint8_Normalized_RGBA,
    Uint8_Normalized_BGR,
    Uint8_Normalized_BGRA,
    Uint8_SBGR,
    Uint8_SBGRA,
    /// <summary>
    /// Normalized Int8 pixel types
    /// </summary>
    Int8_Normalized_R,
    Int8_Normalized_RG,
    Int8_Normalized_RGB,
    Int8_Normalized_RGBA,
    /// <summary>
    /// Unnormalized Uint8 pixel types
    /// </summary>
    Uint8_R,
    Uint8_RG,
    Uint8_RGB,
    Uint8_RGBA,
    /// <summary>
    /// Unnormalized Int8 pixel types
    /// </summary>
    Int8_R,
    Int8_RG,
    Int8_RGB,
    Int8_RGBA,
    /// <summary>
    /// Normalized Uint16 pixel types
    /// </summary>
    Uint16_Normalized_R,
    Uint16_Normalized_RG,
    Uint16_Normalized_RGB,
    Uint16_Normalized_RGBA,
    /// <summary>
    /// Normalized Int16 pixel types
    /// </summary>
    Int16_Normalized_R,
    Int16_Normalized_RG,
    Int16_Normalized_RGB,
    Int16_Normalized_RGBA,
    /// <summary>
    /// Unnormalized Uint16 pixel types
    /// </summary>
    Uint16_R,
    Uint16_RG,
    Uint16_RGB,
    Uint16_RGBA,
    /// <summary>
    /// Unnormalized Uint16 pixel types
    /// </summary>
    Int16_R,
    Int16_RG,
    Int16_RGB,
    Int16_RGBA,
    /// <summary>
    /// Uint32 pixel types
    /// </summary>
    Uint32_R,
    Uint32_RG,
    Uint32_RGB,
    Uint32_RGBA,
    /// <summary>
    /// int32_t pixel types
    /// </summary>
    Int32_R,
    Int32_RG,
    Int32_RGB,
    Int32_RGBA,
    /// <summary>
    /// Floating Point pixel types
    /// </summary>
    Float16_R,
    Float16_RG,
    Float16_RGB,
    Float16_RGBA,
    Float32_R,
    Float32_RG,
    Float32_RGB,
    Float32_RGBA,
    /// <summary>
    /// Depth pixel types
    /// </summary>
    Uint16_Normalized_Depth,
    Float32_Depth,
    /// <summary>
    /// Depth/Stencil pixel types
    /// </summary>
    Uint24_Normalized_Depth_Uint8_Stencil,
    Float32_Normalized_Depth_Uint8_Stencil,
    Uint8_Stencil,
    /// <summary>
    /// Compressed pixel types
    /// </summary>
    S3TC_DXT5_RGBA,
    S3TC_DXT5_SRGBA,
    /// <summary>
    /// Special pixel types
    /// </summary>
    Normalized_BGR10_A2,
    BGR10_A2,
    MaxValue
};
enum class FrontFace {
    Clockwise,
    CounterClockwise,
    MaxValue
};
enum class ImageLayout {
    Undefined,
    General,
    ColorAttachmentOptimal,
    DepthStencilAttachmentOptimal,
    DepthStencilReadOnlyOptimal,
    ShaderReadOnlyOptimal,
    TransferSrcOptimal,
    TransferDstOptimal,
    Preinitialized,
    DepthReadOnlyStencilAttachmentOptimal,
    DepthAttachmentStencilReadOnlyOptimal,
    DepthAttachmentOptimal,
    DepthReadOnlyOptimal,
    StencilAttachmentOptimal,
    StencilReadOnlyOptimal,
    ReadOnlyOptimal,
    AttachmentOptimal,
    PresentSrc,
    MaxValue
};
enum class ImageType {
    Unknown = -1,
    Image1D,
    Image2D,
    Image3D,
    MaxValue
};
enum class ImageViewType {
    Unknown = -1,
    View1D,
    View2D,
    View3D,
    ViewCube,
    View1DArray,
    View2DArray,
    ViewCubeArray,
    MaxValue
};
enum class IndexType {
    None,
    Uint8,
    Uint16,
    Uint32,
    MaxValue
};
enum class LoadOp {
    DontCare, //Nothing is done
    Load,     //Values are kept
    Clear,    //Values are cleared
    MaxValue
};
enum class LogicOp {
    Clear,
    And,
    AndReverse,
    Copy,
    AndInverted,
    NoOp,
    Xor,
    Or,
    Nor,
    Equivalent,
    Invert,
    OrReverse,
    CopyInverted,
    OrInverted,
    Nand,
    Set,
    MaxValue
};
enum class PipelineBindingPoint
{
    Unknown = -1,
    Graphics,
    Compute,
    RayTracing, //might be unavailable
    MaxValue
};
enum class DynamicState {
    Viewport,
    Scissor,
    LineWidth,
    DepthBias,
    BlendConstants,
    DepthBounds,
    StencilCompareMask,
    StencilWriteMask,
    StencilReference,
    //VK 1.3
    CullMode,
    FrontFace,
    PrimitiveTopology,
    ViewportWithCount,
    ScissorWithCount,
    VertexInputBindingStride,
    DepthTestEnable,
    DepthWriteEnable,
    DepthBoundsTestEnable,
    DepthCompareOP,
    StencilTestEnable,
    StencilOP,
    RasterizerDiscardEnable,
    DepthBiasEnable,
    PrimitiveRestartEnable,
    VertexInput,
    MaxValue
};
enum class PolygonMode {
    Fill,
    Line,
    Point,
    MaxValue
};
enum class PrimitiveTopology {
    Unknown = -1,
    PointList = 0,
    LineList = 1,
    LineStrip = 2,
    TriangleList = 3,
    TriangleStrip = 4,
    TriangleFan = 5,
    LineListWithAdjacency = 6,
    LineStripWithAdjacency = 7,
    TriangleListWithAdjacency = 8,
    TriangleStripWithAdjacency = 9,
    PatchList = 10,
    MaxValue
};
enum class QueryType {
    Unknown = -1,
    Occlusion,
    PipelineStatistics,
    TimeStamp,
    MaxValue
};
enum class SampleCount {
    Count1 = 1,
    Count2 = 2,
    Count4 = 4,
    Count8 = 8,
    Count16 = 16,
    Count32 = 32,
    Count64 = 64,
    MaxValue = Count64
};
enum class SamplerAddressMode {
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder,
    MaxValue
};
enum class SemaphoreType {
    Unknown = -1,
    Binary,
    Timeline,
    MaxValue
};
enum class SharingMode {
    Exclusive,
    Concurrent,
    MaxValue
};
enum class StoreOp {
    DontCare, //Nothing is stored
    Store,    //Values from the Fragment Shader are written
    MaxValue
};
enum class SubPassContents {
    Inline,
    SecondaryCommandBuffers
};
enum class PhysicalDeviceType {
    Other = 0,
    IntegratedGpu = 1,
    DiscreteGpu = 2,
    VirtualGpu = 3,
    Cpu = 4,
    MaxValue
};
enum class StencilOp {
    Keep = 0,
    Zero = 1,
    Replace = 2,
    IncrementAndClamp = 3,
    DecrementAndClamp = 4,
    Invert = 5,
    IncrementAndWrap = 6,
    DecrementAndWrap = 7,
    MaxValue
};
enum class Swizzle {
    Identity,
    Zero, One,
    R, G, B, A,
    MaxValue
};
enum class VertexInputRate {
    Vertex,   //use vertex attribute
    Instance, //use instance index
    MaxValue
};
enum class VertexType {
    None,
    Float32,
    Float16,
    Int32,
    Uint32,
    Int16,
    Uint16,
    Int8,
    Uint8,
    MaxValue
};
}