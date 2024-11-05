#pragma once

#include "camera.hpp"
#include "gameobject.hpp"

#include <vulkan/vulkan.h>

#define MAX_LIGHTS 10

struct VulkanFrameInfo {
    int frameIndex;
    float deltaTime;

    VkCommandBuffer commandBuffer;
    VulkanCamera& camera;
    VkDescriptorSet globalDescriptorSet;
    GameObject::map& gameObjects;
};

struct PointLight {
	glm::vec4 position{};
	glm::vec4 color{};
};

struct UniformBufferObject {
    glm::mat4 projection{1.0f};
    glm::mat4 view{1.0f};
    glm::mat4 inverseView{1.0f}; // for get camera position

    glm::vec4 ambientLightColor{1.0f, 1.0f, 1.0f, 0.02f}; // w is intensity
    PointLight pointLights[MAX_LIGHTS];
    int numLights;
};
