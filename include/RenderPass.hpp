/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Framebuffer.hpp>
#include <Handle.hpp>
#include <Image/Format.hpp>
#include <Pipeline/BindingPoint.hpp>
#include <Rect2D.hpp>
#include <Sample.hpp>
#include <Scalar.hpp>

#include <array>

namespace OCRA::Image {
enum class Layout {
	Unknown = -1,
	MaxValue
};
}

namespace OCRA::RenderPass
{
struct AttachmentDescription {
	enum class LoadOperation {
		DontCare, Load, Clear,
		MaxValue
	};
	enum class StoreOperation {
		DontCare, Store,
		MaxValue
	};
	Image::Format format{ Image::Format::Unknown };
	Sample::Count samples{ Sample::Count::Count1 };
	//load/store for color/depth buffer
	LoadOperation loadOp{ LoadOperation::DontCare }; //determines what to do with the buffer before rendering
	StoreOperation storeOp{ StoreOperation::DontCare }; //determines what to do with the buffer after rendering
	//load/store for stencil buffer
	LoadOperation stencilLoadOp{ LoadOperation::DontCare };
	StoreOperation stencilStoreOp{ StoreOperation::DontCare };
	Image::Layout initialLayout{ Image::Layout::Unknown };
	Image::Layout finalLayout{ Image::Layout::Unknown };
};

struct AttachmentReference {
	Int8 attachment{ -1 }; //-1 means no attachment
	Image::Layout layout{ Image::Layout::Unknown };
};

struct SubPassDescription {
	constexpr static auto MaxAttachments = 32;
	Pipeline::BindingPoint pipelineBindPoint{ Pipeline::BindingPoint::Unknown };
	Uint8 inputAttachmentCount{ 0 };
	std::array<AttachmentReference, MaxAttachments> inputAttachments;
	Uint8 colorAttachmentCount{ 0 };
	std::array<AttachmentReference, MaxAttachments> colorAttachments;
	AttachmentReference resolveAttachment;
	AttachmentReference depthStencilAttachment;
	//describes attachments not used by this subpass but whose content must pe preserved
	Uint8 preserveAttachmentCount{ 0 };
	std::array<AttachmentReference, MaxAttachments> preserveAttachments;
};

struct SubPassDependency {
	Int8 srcSubPass{ -1 };
	Int8 dstSubPass{ -1 };
};

struct Info {
	constexpr static auto MaxAttachments = 32;
	constexpr static auto MaxDependencies = 64;
	Uint8 attachmentCount{ 0 };
	std::array<AttachmentDescription, MaxAttachments> attachments;
	Uint8 subpassCount{ 0 };
	std::array<SubPassDescription, MaxAttachments> subPasses;
	Uint8 dependencyCount{ 0 };
	std::array<SubPassDependency, MaxDependencies> dependencies;
};
Uint8 GetAttachmentCount(const Handle& a_RenderPass);
const AttachmentDescription& GetAttachment(const Handle& a_RenderPass, Uint8 a_AttachmentIndex);
Uint8 GetSubpassCount(const Handle& a_RenderPass);
const SubPassDescription& GetSubpass(const Handle& a_RenderPass, Uint8 a_SubPassIndex);
Uint8 GetDependencyCount(const Handle& a_RenderPass);
const SubPassDependency& GetDependency(const Handle& a_RenderPass, Uint8 a_DependencyIndex);
}