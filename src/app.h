#pragma once

#include "render/window.h"
#include "render/pipeline.h"
#include "render/swapchain.h"

struct VulkanAppSettings {
public:
	int screenWidth = 800;
	int screenHeight = 600;
	std::string name = "vulkan tech demo";

	std::string vertShaderPath;
	std::string fragShaderPath;

	PipelineConfigInfo pipelineInfo;
};

class VulkanApp {
public:
	VulkanApp(const VulkanAppSettings& settings);
	~VulkanApp();

	void run();

private:
	VulkanWindow* window;
	VulkanDevice* device;
	VulkanSwapChain* swapChain;
	VulkanPipeline* pipeline;
};
