#pragma once

// app
#include "gameobject.hpp"
#include "render/window.hpp"
#include "render/renderer.hpp"
#include "input.hpp"
#include "systems/simple_render_system.hpp"
#include "render/descriptor.hpp"

// libs
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

class VulkanApp {
public:
	VulkanApp(VulkanAppSettings& settings);
	~VulkanApp();

	VulkanApp() = default;
	VulkanApp(const VulkanApp&) = delete;
	VulkanApp& operator=(const VulkanApp&) = delete;

	void run();

private:
	void createDescriptors();
	void loadGameObjects(const std::string& modelPath);

	std::unique_ptr<VulkanWindow> window;
	std::unique_ptr<VulkanDevice> device;
	std::unique_ptr<VulkanRenderer> renderer;
	std::unique_ptr<VulkanCamera> camera;

	std::unique_ptr<VulkanDescriptorPool> globalPool;
	std::unique_ptr<VulkanDescriptorSetLayout> globalSetLayout;

	std::unique_ptr<SimpleRenderSystem> renderSystem;
	std::unique_ptr<InputKeyboardController> keyboardInput;

	GameObject::Map gameObjects;
};
