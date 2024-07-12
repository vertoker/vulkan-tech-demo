#pragma once

#include "render/window.h"
#include "render/pipeline.h"
#include "render/swapchain.h"
#include "render/model.h"

// std
#include <iostream>
#include <memory>
#include <vector>

struct VulkanAppSettings {
public:
	int screenWidth = 800;
	int screenHeight = 600;
	std::string name = "vulkan tech demo";

	std::string vertShaderPath;
	std::string fragShaderPath;
};

class VulkanApp {
public:
	VulkanApp(VulkanAppSettings& settings);
	~VulkanApp();

	VulkanApp(const VulkanApp&) = delete;
	VulkanApp& operator=(const VulkanApp&) = delete;

	void run();

private:
	void loadModels();
	void createPipelineLayout();
	void createPipeline(VulkanAppSettings& settings);
	void createCommandBuffers();
	void drawFrame();

	std::unique_ptr<VulkanWindow> window;
	std::unique_ptr<VulkanDevice> device;
	std::unique_ptr<VulkanSwapChain> swapChain;
	std::unique_ptr<VulkanPipeline> pipeline;

	VkPipelineLayout pipelineLayout;
	std::vector<VkCommandBuffer> commandBuffers;

	std::unique_ptr<VulkanModel> testModel;
};
