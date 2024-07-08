#pragma once

#include "render/window.h"
#include "render/pipeline.h"

struct VulkanAppSettings {
public:
	int Width = 800;
	int Height = 600;
	std::string Name = "vulkan tech demo";

	std::string VertShaderPath;
	std::string FragShaderPath;
};

class VulkanApp {
public:
	VulkanApp(const VulkanAppSettings& settings);
	~VulkanApp();

	void run();

private:
	VulkanWindow* window;
	VulkanPipeline* pipeline;
};
