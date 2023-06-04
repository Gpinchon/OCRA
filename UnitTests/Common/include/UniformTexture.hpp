#pragma once

#include <Texture.hpp>
#include <Uniform.hpp>

#include <OCRA/OCRA.hpp>

namespace OCRA {
class UniformTexture : public Uniform {
public:
    UniformTexture()                      = default;
    UniformTexture(const UniformTexture&) = default;
    UniformTexture(const Device::Handle& a_Device, const uint32_t& a_Binding, const Texture& a_Texture)
        : Uniform(a_Device, { CreateSetLayoutBinding(a_Binding) })
        , texture(a_Texture)
    {
    }
    void SetTexture(const Texture& a_Texture)
    {
        texture = a_Texture;
        dirty   = true;
    }
    auto& GetTexture() const { return texture; }
    virtual bool Update() override;

private:
    static DescriptorSetLayoutBinding CreateSetLayoutBinding(const uint32_t a_Binding);
    bool dirty { true };
    Texture texture {};
};
}
