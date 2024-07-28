#pragma once

#include "gameobject.h"
#include "render/window.h"
#include "render/renderer.h"
#include "systems/simple_render_system.h"

// Libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

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

struct PushConstantData {
	glm::mat2 transform{ 1.f }; // Identity matrix
	glm::vec2 offset;
	// alignas is for memory specification for shader declaration
	alignas(16) glm::vec3 color;
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
	void loadGameObjects();

	std::unique_ptr<VulkanWindow> window;
	std::unique_ptr<VulkanDevice> device;
	std::unique_ptr<VulkanRenderer> renderer;
	std::unique_ptr<SimpleRenderSystem> renderSystem;

	std::vector<GameObject> gameObjects;
};
