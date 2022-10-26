#pragma once

#include <Descriptor/Pool.hpp>
#include <Descriptor/SetLayout.hpp>

#include <Handle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Descriptor::Pool);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::Set);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::SetLayout);

namespace OCRA {
class Uniform {
public:
    //We already know how our uniforms are formated
    Uniform(const std::vector<Descriptor::SetLayout::Binding>& a_Bindings, const Device::Handle& a_Device, const Descriptor::Pool::Handle& a_DescriptorPool)
        : device(a_Device)
        , descriptorPool(a_DescriptorPool)
        , descriptorSetLayoutBindings(a_Bindings)
    {
        CreateDescriptorSetLayout();
        CreateDescriptorSets();
    }
    ~Uniform()
    {}
    //We don't know our bindings yet and will call CreateDescriptorSetLayout & CreateDescriptorSets later
    Uniform(const Device::Handle& a_Device, const Descriptor::Pool::Handle& a_DescriptorPool)
        : device(a_Device)
        , descriptorPool(a_DescriptorPool)
    {}
    auto& GetDevice() const { return device; };
    auto& GetDescriptorPool() const { return descriptorPool; }
    auto& GetDescriptorSetLayoutBindings() const { return descriptorSetLayoutBindings; }
    auto& GetDescriptorSetLayout() const { return descriptorSetLayout; }
    auto& GetDescriptorSets() const { return descriptorSets; }

protected:
    //To be called when the data needs updating
    virtual void Update() = 0;
    void SetDescriptorSetLayoutBindings(const std::vector<Descriptor::SetLayout::Binding>& a_Bindings) {
        descriptorSetLayoutBindings = a_Bindings;
    }
    void CreateDescriptorSetLayout() {
        Descriptor::SetLayout::Info info;
        info.bindings = GetDescriptorSetLayoutBindings();
        descriptorSetLayout = Descriptor::SetLayout::Create(device, info);
    };
    void CreateDescriptorSets() {
        Descriptor::Pool::AllocateInfo allocateInfo{};
        allocateInfo.pool = GetDescriptorPool();
        allocateInfo.layouts.push_back(GetDescriptorSetLayout());
        descriptorSets = Descriptor::Pool::AllocateSet(device, allocateInfo);
    };

private:
    const Device::Handle                          device;
    const Descriptor::Pool::Handle                descriptorPool;
    std::vector<Descriptor::SetLayout::Binding>   descriptorSetLayoutBindings;
    Descriptor::SetLayout::Handle                 descriptorSetLayout;
    std::vector<Descriptor::Set::Handle>          descriptorSets;


};
}