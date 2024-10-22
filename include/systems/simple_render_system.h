#pragma once

#include "render/device.h"
#include "render/pipeline.h"
#include "gameobject.h"
#include "render/camera.h"

// Libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <iostream>
#include <memory>
#include <vector>

class SimpleRenderSystem {
public:
	SimpleRenderSystem(VulkanDevice& device, VkRenderPass renderPass, 
		const std::string& vertFilePath,
		const std::string& fragFilePath);
	~SimpleRenderSystem();

	SimpleRenderSystem(const SimpleRenderSystem&) = delete;
	SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

	void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects, const VulkanCamera& camera);

private:
	void createPipelineLayout();
	void createPipeline(VkRenderPass renderPass, 
		const std::string& vertFilePath,
		const std::string& fragFilePath);

	VulkanDevice& device;

	std::unique_ptr<VulkanPipeline> pipeline;
	std::string vertShaderPath, fragShaderPath;
	VkPipelineLayout pipelineLayout;
};
