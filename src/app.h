#pragma once

#include "gameobject.h"
#include "render/window.h"
#include "render/renderer.h"
#include "systems/simple_render_system.h"
#include "input.h"

// Libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <iostream>
#include <chrono>
#include <memory>
#include <vector>

struct VulkanAppSettings {
public:
	int screenWidth = 600;
	int screenHeight = 600;
	std::string name = "vulkan tech demo";

	std::string vertShaderPath;
	std::string fragShaderPath;
	std::string modelPath;
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
	void loadGameObjects(const std::string& modelPath);

	std::unique_ptr<VulkanWindow> window;
	std::unique_ptr<VulkanDevice> device;
	std::unique_ptr<VulkanRenderer> renderer;
	std::unique_ptr<VulkanCamera> camera;
	std::unique_ptr<SimpleRenderSystem> renderSystem;
	std::unique_ptr<InputKeyboardController> keyboardInput;

	std::vector<GameObject> gameObjects;
};
