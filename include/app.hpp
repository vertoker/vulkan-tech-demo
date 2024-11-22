#pragma once

// app
#include "render/window.hpp"
#include "render/renderer.hpp"
#include "render/descriptor.hpp"
#include "render/model.hpp"
#include "input.hpp"

#include "systems/world_render_system.hpp"
#include "systems/point_light_system.hpp"
#include "systems/imgui_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <entt/entt.hpp>

// std
#include <iostream>
#include <chrono>
#include <memory>
#include <vector>

struct VulkanAppSettings {
public:
	int screenWidth = 1000;
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

	std::unique_ptr<WorldRenderSystem> worldRenderSystem;
	std::unique_ptr<PointLightSystem> pointLightSystem;
	std::unique_ptr<ImguiSystem> imguiSystem;

	std::unique_ptr<InputKeyboardController> keyboardInput;

	entt::registry registry;
};
