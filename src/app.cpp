#include "app.h"

#include <tuple>

VulkanApp::VulkanApp(VulkanAppSettings& settings)
{
	window = std::make_unique<VulkanWindow>(settings.screenWidth, settings.screenHeight, settings.name);
	device = std::make_unique<VulkanDevice>(*window);
	renderer = std::make_unique<VulkanRenderer>(*window, *device);
	renderSystem = std::make_unique<SimpleRenderSystem>(*device, 
		renderer->getSwapChainRenderPass(), 
		settings.vertShaderPath, settings.fragShaderPath);

	loadGameObjects();
}
VulkanApp::~VulkanApp()
{

}

void VulkanApp::run()
{
	while (!window->shouldClose()) {
		glfwPollEvents();
		// On Linux, resizing can be occurs wrong rendering
		// On Linux, you need another frame update method
		if (auto commandBuffer = renderer->beginFrame()) {
			renderer->beginSwapChainRenderPass(commandBuffer);
			renderSystem->renderGameObjects(commandBuffer, gameObjects);
			renderer->endSwapChainRenderPass(commandBuffer);
			renderer->endFrame();
		}
	}
	// Fix errors while dispose Vulkan
	vkDeviceWaitIdle(device->device());
}

void VulkanApp::loadGameObjects()
{
	std::vector<VulkanModel::Vertex> vertices{
		{ { 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
		{ { 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f } },
		{ { -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } }
	};
	auto testModel = std::make_shared<VulkanModel>(*device, vertices);

	for (size_t i = 0; i < 150; i++)
	{
		auto triangle = GameObject::createGameObject();
		triangle.model = testModel;
		triangle.color = { 0.8f, 0.1f, 0.1f + i * 0.003f };
		triangle.transform2D.position.x = 0.0f;
		triangle.transform2D.scale = { 0.3f + i * 0.008f, 1.0f };
		triangle.transform2D.rotation = 0.0f * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));
	}
}

