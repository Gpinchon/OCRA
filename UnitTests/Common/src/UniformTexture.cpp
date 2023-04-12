#include <UniformTexture.hpp>

namespace OCRA {
void UniformTexture::Update()
{
    if (!dirty) return;
    DescriptorSetImageInfo setImageInfo;
    DescriptorSetWrite writeOp;
    setImageInfo.imageView   = texture.GetImageView();
    setImageInfo.imageLayout = ImageLayout::General;
    setImageInfo.sampler = texture.GetSampler();
    writeOp.type = DescriptorType::ImageSampler;
    writeOp.imageInfo = setImageInfo;
    writeOp.dstBinding = GetDescriptorSetLayoutBindings().front().binding;
    writeOp.dstCount = 1;
    SetWriteOperations({ writeOp });
    dirty = false;
}

DescriptorSetLayoutBinding UniformTexture::CreateSetLayoutBinding(const uint32_t a_Binding) {
    DescriptorSetLayoutBinding binding;
    binding.binding    = a_Binding;
    binding.count      = 1;
    binding.type       = DescriptorType::ImageSampler;
    binding.stageFlags = ShaderStageFlagBits::All;
    return binding;
}
}
