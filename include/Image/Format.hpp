/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:28:15
*/

#pragma once

namespace OCRA::Image {
enum class Format {
    Unknown = -1,
    /// <summary>
    /// Normalized Uint8 pixel types
    /// </summary>
    Uint8_Normalized_R,
    Uint8_Normalized_RG,
    Uint8_Normalized_RGB,
    Uint8_Normalized_RGBA,
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
    MaxValue
};
}