#include "app.h"

#include <tuple>

VulkanApp::VulkanApp(VulkanAppSettings& settings)
{
	window = std::make_unique<VulkanWindow>(settings.screenWidth, settings.screenHeight, settings.name);
	device = std::make_unique<VulkanDevice>(*window);
	renderer = std::make_unique<VulkanRenderer>(*window, *device);
    camera = std::make_unique<VulkanCamera>();
	renderSystem = std::make_unique<SimpleRenderSystem>(*device, 
		renderer->getSwapChainRenderPass(), 
		settings.vertShaderPath, settings.fragShaderPath);

	loadGameObjects();
    camera->setOrthographicProjection(-1, 1, -1, 1, -5, 5);
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
			renderSystem->renderGameObjects(commandBuffer, gameObjects, *camera);
			renderer->endSwapChainRenderPass(commandBuffer);
			renderer->endFrame();
		}
	}
	// Fix errors while dispose Vulkan
	vkDeviceWaitIdle(device->device());
}


std::unique_ptr<VulkanModel> createCubeModel(VulkanDevice& device, glm::vec3 offset) {
    glm::vec3 white{ 0.9f, 0.9f, 0.9f };
    glm::vec3 yellow{ 0.8f, 0.8f, 0.1f };
    glm::vec3 orange{ 0.9f, 0.2f, 0.1f };
    glm::vec3 red{ 0.8f, 0.1f, 0.1f };
    glm::vec3 blue{ 0.1f, 0.1f, 0.8f };
    glm::vec3 green{ 0.1f, 0.8f, 0.1f };

    glm::vec3 v1{  0.5f,  0.5f,  0.5f };
    glm::vec3 v2{ -0.5f,  0.5f,  0.5f };
    glm::vec3 v3{  0.5f, -0.5f,  0.5f };
    glm::vec3 v4{ -0.5f, -0.5f,  0.5f };
    glm::vec3 v5{  0.5f,  0.5f, -0.5f };
    glm::vec3 v6{ -0.5f,  0.5f, -0.5f };
    glm::vec3 v7{  0.5f, -0.5f, -0.5f };
    glm::vec3 v8{ -0.5f, -0.5f, -0.5f };

    std::vector<VulkanModel::Vertex> vertices {
        // left face (white)
        {v8, white}, {v2, white}, {v4, white},
        {v8, white}, {v6, white}, {v2, white},
        // right face (yellow)
        {v7, yellow}, {v3, yellow}, {v1, yellow},
        {v7, yellow}, {v1, yellow}, {v5, yellow},
        // top face (orange)
        {v8, orange}, {v4, orange}, {v3, orange},
        {v8, orange}, {v3, orange}, {v7, orange},
        // bottom face (red)
        {v6, red}, {v1, red}, {v2, red},
        {v6, red}, {v5, red}, {v1, red},
        // nose face (blue)
        {v4, blue}, {v2, blue}, {v1, blue},
        {v4, blue}, {v1, blue}, {v3, blue},
        // tail face (green)
        {v8, green}, {v5, green}, {v6, green},
        {v8, green}, {v7, green}, {v5, green},
    };

    for (auto& v : vertices)
        v.position += offset;

    return std::make_unique<VulkanModel>(device, vertices);
}

void VulkanApp::loadGameObjects()
{
    std::shared_ptr<VulkanModel> testModel = createCubeModel(*device, { 0.0f, 0.0f, 0.0f });

    auto cube = GameObject::createGameObject();
    cube.model = testModel;
    cube.transform.position = { 0.0f, 0.0f, 0.5f };
    cube.transform.scale = { 0.5f, 0.5f, 0.5f };
    cube.transform.rotation = { 0.0f, 0.0f, 0.0f };
    //cube.transform.rotation = { 1.0f, 1.0f, 0.5f, 0.0f };

    gameObjects.push_back(std::move(cube));
}