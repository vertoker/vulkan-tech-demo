#include "render/descriptor.hpp"

// std
#include <cassert>
#include <stdexcept>

// *************** Descriptor Set Layout Builder *********************

VulkanDescriptorSetLayout::Builder& VulkanDescriptorSetLayout::Builder::addBinding(
    uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count)
{
    assert(vkBindings.count(binding) == 0 && "Binding already in use");
    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = binding;
    layoutBinding.descriptorType = descriptorType;
    layoutBinding.descriptorCount = count;
    layoutBinding.stageFlags = stageFlags;
    vkBindings[binding] = layoutBinding;
    return *this;
}
std::unique_ptr<VulkanDescriptorSetLayout> VulkanDescriptorSetLayout::Builder::build() const
{
    return std::make_unique<VulkanDescriptorSetLayout>(device, vkBindings);
}

// *************** Descriptor Set Layout *********************

VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(
    VulkanDevice &device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> vkBindings)
    : device{device}, vkBindings{vkBindings}
{
    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
    setLayoutBindings.reserve(vkBindings.bucket_count());
    for (auto pair : vkBindings)
    {
        setLayoutBindings.emplace_back(pair.second);
    }
    
    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
    descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
    descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();
    
    if (vkCreateDescriptorSetLayout(device.device(), &descriptorSetLayoutInfo, nullptr, &vkDescriptorSetLayout) != VK_SUCCESS)
        throw std::runtime_error("failed to create descriptor set layout!");
}
VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(device.device(), vkDescriptorSetLayout, nullptr);
}

// *************** Descriptor Pool Builder *********************

VulkanDescriptorPool::Builder &VulkanDescriptorPool::Builder::addPoolSize(VkDescriptorType descriptorType, uint32_t count)
{
    vkPoolSizes.push_back({descriptorType, count});
    return *this;
}
VulkanDescriptorPool::Builder &VulkanDescriptorPool::Builder::setPoolFlags(VkDescriptorPoolCreateFlags flags)
{
    vkPoolFlags = flags;
    return *this;
}
VulkanDescriptorPool::Builder &VulkanDescriptorPool::Builder::setMaxSets(uint32_t count)
{
    maxSets = count;
    return *this;
}
std::unique_ptr<VulkanDescriptorPool> VulkanDescriptorPool::Builder::build() const
{
    return std::make_unique<VulkanDescriptorPool>(device, maxSets, vkPoolFlags, vkPoolSizes);
}

// *************** Descriptor Pool *********************

VulkanDescriptorPool::VulkanDescriptorPool(VulkanDevice &device, uint32_t maxSets, 
    VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize> &vkPoolSizes)
    : device{device}
{
    VkDescriptorPoolCreateInfo descriptorPoolInfo{};
    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(vkPoolSizes.size());
    descriptorPoolInfo.pPoolSizes = vkPoolSizes.data();
    descriptorPoolInfo.maxSets = maxSets;
    descriptorPoolInfo.flags = poolFlags;
    
    if (vkCreateDescriptorPool(device.device(), &descriptorPoolInfo, nullptr, &vkDescriptorPool) != VK_SUCCESS)
        throw std::runtime_error("failed to create descriptor pool!");
}
VulkanDescriptorPool::~VulkanDescriptorPool()
{
    vkDestroyDescriptorPool(device.device(), vkDescriptorPool, nullptr);
}

bool VulkanDescriptorPool::tryAllocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const
{
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = vkDescriptorPool;
    allocInfo.pSetLayouts = &descriptorSetLayout;
    allocInfo.descriptorSetCount = 1;

    // Might want to create a "DescriptorPoolManager" class that handles this case, and builds
    // a new pool whenever an old pool fills up. But this is beyond our current scope
    // TODO fix to the more efficient method
    return vkAllocateDescriptorSets(device.device(), &allocInfo, &descriptor) == VK_SUCCESS;
}
void VulkanDescriptorPool::freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const
{
    vkFreeDescriptorSets(device.device(), vkDescriptorPool, static_cast<uint32_t>(descriptors.size()), descriptors.data());
}
void VulkanDescriptorPool::resetPool()
{
    vkResetDescriptorPool(device.device(), vkDescriptorPool, 0);
}

// *************** Descriptor Writer *********************

VulkanDescriptorWriter::VulkanDescriptorWriter(
    VulkanDescriptorSetLayout &setLayout, VulkanDescriptorPool &pool)
    : setLayout{setLayout}, pool{pool} { }

VulkanDescriptorWriter &VulkanDescriptorWriter::writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo)
{
    assert(setLayout.vkBindings.count(binding) == 1 && "Layout does not contain specified binding");
    
    auto &bindingDescription = setLayout.vkBindings[binding];
    assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info, but binding expects multiple");
    
    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pBufferInfo = bufferInfo;
    write.descriptorCount = 1;

    vkWrites.push_back(write);
    return *this;
}
VulkanDescriptorWriter &VulkanDescriptorWriter::writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo)
{
    assert(setLayout.vkBindings.count(binding) == 1 && "Layout does not contain specified binding");
    
    auto &bindingDescription = setLayout.vkBindings[binding];
    assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info, but binding expects multiple");
    
    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pImageInfo = imageInfo;
    write.descriptorCount = 1;

    vkWrites.push_back(write);
    return *this;
}

bool VulkanDescriptorWriter::build(VkDescriptorSet &set)
{
    bool success = pool.tryAllocateDescriptor(setLayout.getDescriptorSetLayout(), set);

    if (!success)
        return false;

    overwrite(set);
    return true;
}
void VulkanDescriptorWriter::overwrite(VkDescriptorSet &set)
{
    for (auto &write : vkWrites) {
        write.dstSet = set;
    }
    vkUpdateDescriptorSets(pool.device.device(), vkWrites.size(), vkWrites.data(), 0, nullptr);
}
