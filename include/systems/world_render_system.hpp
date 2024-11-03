#pragma once

#include "render/device.hpp"
#include "render/pipeline.hpp"
#include "gameobject.hpp"
#include "render/camera.hpp"
#include "render/frameInfo.hpp"

// Libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <iostream>
#include <memory>
#include <vector>

class WorldRenderSystem {
public:
	WorldRenderSystem(VulkanDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout,
		const std::string& vertFilePath, const std::string& fragFilePath);
	~WorldRenderSystem();

	WorldRenderSystem(const WorldRenderSystem&) = delete;
	WorldRenderSystem& operator=(const WorldRenderSystem&) = delete;

	void render(VulkanFrameInfo& frameInfo);

private:
	void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
	void createPipeline(VkRenderPass renderPass, 
		const std::string& vertFilePath, const std::string& fragFilePath);
	
	VulkanDevice& device;

	std::unique_ptr<VulkanPipeline> pipeline;
	std::string vertShaderPath, fragShaderPath;
	VkPipelineLayout pipelineLayout;
};
