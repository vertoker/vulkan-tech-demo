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

	VulkanApp() = default;
	VulkanApp(const VulkanApp&) = delete;
	VulkanApp& operator=(const VulkanApp&) = delete;

	void run();

private:
	void loadModels();
	void createPipelineLayout();
	void createPipeline();
	void createCommandBuffers();
	void drawFrame();

	void recreateSwapChain();
	void recordCommandBuffer(int imageIndex);

	std::unique_ptr<VulkanWindow> window;
	VulkanSwapChain* swapChain = nullptr;

	std::unique_ptr<VulkanDevice> device;

	std::unique_ptr<VulkanPipeline> pipeline;
	std::string vertShaderPath, fragShaderPath;
	VkPipelineLayout pipelineLayout;
	std::vector<VkCommandBuffer> commandBuffers;

	std::unique_ptr<VulkanModel> testModel;
};
