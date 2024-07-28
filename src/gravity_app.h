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

struct VulkanPhysicsAppSettings {
public:
	int screenWidth = 800;
	int screenHeight = 600;
	std::string name = "vulkan tech demo";

	std::string vertShaderPath;
	std::string fragShaderPath;
};

class VulkanPhysicsApp {
public:
	VulkanPhysicsApp(VulkanPhysicsAppSettings& settings);
	~VulkanPhysicsApp();

	VulkanPhysicsApp() = default;
	VulkanPhysicsApp(const VulkanPhysicsApp&) = delete;
	VulkanPhysicsApp& operator=(const VulkanPhysicsApp&) = delete;

	void run();

private:
	std::vector<GameObject> createPhysicsObjects();
	std::vector<GameObject> createVectorField();

	std::unique_ptr<VulkanWindow> window;
	std::unique_ptr<VulkanDevice> device;
	std::unique_ptr<VulkanRenderer> renderer;
	std::unique_ptr<SimpleRenderSystem> renderSystem;

	std::vector<GameObject> gameObjects;
};
