#pragma once

#include "camera.hpp"

#include <vulkan/vulkan.h>

struct VulkanFrameInfo {
    int frameIndex;
    float deltaTime;
    VkCommandBuffer commandBuffer;
    VulkanCamera& camera;
    VkDescriptorSet globalDescriptorSet;
    GameObject::map& gameObjects;
};