#include <UniformTexture.hpp>

namespace OCRA {
bool UniformTexture::Update()
{
    if (!dirty)
        return false;
    DescriptorSetImageInfo setImageInfo;
    DescriptorSetWrite writeOp;
    setImageInfo.imageView   = texture.GetImageView();
    setImageInfo.imageLayout = ImageLayout::General;
    setImageInfo.sampler     = texture.GetSampler();
    writeOp.type             = DescriptorType::ImageSampler;
    writeOp.imageInfo        = setImageInfo;
    writeOp.dstBinding       = GetDescriptorSetLayoutBindings().front().binding;
    writeOp.dstCount         = 1;
    SetWriteOperations({ writeOp });
    dirty = false;
    return true;
}

DescriptorSetBinding UniformTexture::CreateSetBinding(const uint32_t a_Binding)
{
    DescriptorSetBinding binding;
    binding.binding    = a_Binding;
    binding.count      = 1;
    binding.type       = DescriptorType::ImageSampler;
    binding.stageFlags = ShaderStageFlagBits::All;
    return binding;
}
}
