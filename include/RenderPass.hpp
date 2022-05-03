/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Common/Rect2D.hpp>
#include <Handle.hpp>
#include <Image/Format.hpp>
#include <Image/Layout.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Common/SampleCount.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::RenderPass);
OCRA_DECLARE_HANDLE(OCRA::Device);

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
	SampleCount samples{ SampleCount::Count1 };
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
	int8_t attachment{ -1 }; //-1 means no attachment
	Image::Layout layout{ Image::Layout::Unknown };
};

struct SubPassDescription {
	constexpr static auto MaxAttachments = 32;
	Pipeline::BindingPoint pipelineBindPoint{ Pipeline::BindingPoint::Unknown };
	std::vector<AttachmentReference> inputAttachments;
	std::vector<AttachmentReference> colorAttachments;
	AttachmentReference resolveAttachment;
	AttachmentReference depthStencilAttachment;
	//describes attachments not used by this subpass but whose content must pe preserved
	std::vector<AttachmentReference> preserveAttachments;
};

struct SubPassDependency {
	int8_t srcSubPass{ -1 };
	int8_t dstSubPass{ -1 };
};

struct Info {
	std::vector<AttachmentDescription> attachments;
	std::vector<SubPassDescription> subPasses;
	std::vector<SubPassDependency> dependencies;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info);
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle);
uint8_t GetAttachmentCount(const Handle& a_RenderPass);
const AttachmentDescription& GetAttachment(const Handle& a_RenderPass, uint8_t a_AttachmentIndex);
uint8_t GetSubpassCount(const Handle& a_RenderPass);
const SubPassDescription& GetSubpass(const Handle& a_RenderPass, uint8_t a_SubPassIndex);
uint8_t GetDependencyCount(const Handle& a_RenderPass);
const SubPassDependency& GetDependency(const Handle& a_RenderPass, uint8_t a_DependencyIndex);
}