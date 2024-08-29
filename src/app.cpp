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
    keyboardInput = std::make_unique<InputKeyboardController>();

	loadGameObjects(settings.modelPath);
}
VulkanApp::~VulkanApp()
{

}

const float MAX_FRAME_TIME = 0.25f;

void VulkanApp::run()
{
    auto currentTime = std::chrono::high_resolution_clock::now();

    auto viewer = GameObject::createGameObject();

	while (!window->shouldClose()) {
		glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        deltaTime = glm::min(deltaTime, MAX_FRAME_TIME);

        keyboardInput->move(window->getPtr(), deltaTime, viewer);

        //std::cout << "position=" << viewer.transform.position_str() << std::endl;
        //std::cout << "rotation=" << viewer.transform.rotation_str() << std::endl;

        //camera->setViewDirection(glm::vec3(0.0f), glm::vec3(0.5f, 0.0f, 1.0f));
        //camera->setViewTarget(glm::vec3(-1.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.5f));
        camera->setViewYXZ(viewer.transform.position, viewer.transform.rotation);

        float aspect = renderer->getAspectRatio();
        // height is constant, width is flexible
        // right - left = aspect * (bottom - top) (y is inverted)

        //camera->setOrthographicProjection(-aspect * 0.6f, aspect * 0.6f, -0.6f, 0.6f, -5, 5);
        camera->setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);

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

void VulkanApp::loadGameObjects(const std::string& modelPath)
{
    std::shared_ptr<VulkanModel> testModel = VulkanModel::createModelFromFile(*device, modelPath);

    auto gameObj = GameObject::createGameObject();
    gameObj.model = testModel;
    gameObj.transform.position = { 0.0f, 0.0f, 1.0f };
    gameObj.transform.rotation = { 0.0f, 0.0f, 0.0f };
    //gameObj.transform.rotation = { 1.0f, 1.0f, 0.5f, 0.0f };
    gameObj.transform.scale = { 0.1f, 0.1f, 0.1f };

    gameObjects.push_back(std::move(gameObj));
}