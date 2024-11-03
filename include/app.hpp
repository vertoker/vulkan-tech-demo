#pragma once

// app
#include "gameobject.hpp"
#include "render/window.hpp"
#include "render/renderer.hpp"
#include "input.hpp"
#include "render/descriptor.hpp"

#include "systems/world_render_system.hpp"
#include "systems/point_light_system.hpp"

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

	std::string world_vertShaderPath;
	std::string world_fragShaderPath;
	std::string pointLight_vertShaderPath;
	std::string pointLight_fragShaderPath;
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

	using render_system = std::shared_ptr<VulkanRenderSystem>;
	using render_systems = std::vector<render_system>;

private:

	void createDescriptors();
	void createRenderSystems(VulkanAppSettings& settings);
	void loadGameObjects(const std::string& modelPath);

	std::unique_ptr<VulkanWindow> window;
	std::unique_ptr<VulkanDevice> device;
	std::unique_ptr<VulkanRenderer> renderer;
	std::unique_ptr<VulkanCamera> camera;

	std::unique_ptr<VulkanDescriptorPool> globalPool;
	std::unique_ptr<VulkanDescriptorSetLayout> globalSetLayout;

	std::unique_ptr<render_systems> renderSystems;
	std::unique_ptr<InputKeyboardController> keyboardInput;

	GameObject::map gameObjects;
};
