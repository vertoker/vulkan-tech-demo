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

	loadGameObjects();
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


std::unique_ptr<VulkanModel> createCubeModel(VulkanDevice& device, glm::vec3 offset) {
    glm::vec3 white{ 0.9f, 0.9f, 0.9f };
    glm::vec3 yellow{ 0.8f, 0.8f, 0.1f };
    glm::vec3 orange{ 0.8f, 0.4f, 0.1f };
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

    VulkanModel::Builder builder{};

    builder.vertices = {
        // left face (white)
        {v8, white}, {v2, white}, {v4, white},
        {v8, white}, {v6, white}, {v2, white},
        // right face (yellow) // invert
        {v7, yellow}, {v3, yellow}, {v1, yellow},
        {v7, yellow}, {v1, yellow}, {v5, yellow},

        // top face (orange) // invert
        {v8, orange}, {v4, orange}, {v3, orange},
        {v8, orange}, {v3, orange}, {v7, orange},
        // bottom face (red)
        {v6, red}, {v1, red}, {v2, red},
        {v6, red}, {v5, red}, {v1, red},

        // forward face (blue) // invert
        {v4, blue}, {v2, blue}, {v1, blue},
        {v4, blue}, {v1, blue}, {v3, blue},
        // backward face (green)
        {v8, green}, {v5, green}, {v6, green},
        {v8, green}, {v7, green}, {v5, green},
    };

    for (auto& v : builder.vertices)
        v.position += offset;

    return std::make_unique<VulkanModel>(device, builder);
}

void VulkanApp::loadGameObjects()
{
    std::shared_ptr<VulkanModel> testModel = createCubeModel(*device, { 0.0f, 0.0f, 0.0f });

    auto cube = GameObject::createGameObject();
    cube.model = testModel;
    cube.transform.position = { 0.0f, 0.0f, 1.0f };
    cube.transform.rotation = { 0.0f, 0.0f, 0.0f };
    //cube.transform.rotation = { 1.0f, 1.0f, 0.5f, 0.0f };
    cube.transform.scale = { 0.5f, 0.5f, 0.5f };

    gameObjects.push_back(std::move(cube));
}