#include "app.hpp"

#include <tuple>
#include <numeric>

VulkanApp::VulkanApp(VulkanAppSettings& settings)
{
    // Base, on first creation
	window = std::make_unique<VulkanWindow>(settings.screenWidth, settings.screenHeight, settings.name);
	device = std::make_unique<VulkanDevice>(*window);

    // Rendering core
	renderer = std::make_unique<VulkanRenderer>(*window, *device);
    camera = std::make_unique<VulkanCamera>();
    createDescriptors();

    // More app-like data
	renderSystem = std::make_unique<SimpleRenderSystem>(*device, 
		renderer->getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout(),
		settings.vertShaderPath, settings.fragShaderPath);
    keyboardInput = std::make_unique<InputKeyboardController>();

	loadGameObjects(settings.modelPath);
}
VulkanApp::~VulkanApp()
{

}

struct UniformBufferObject {
	// alignas is same as PushConstants
    alignas(16) glm::mat4 projectionView{1.0f};
    alignas(16) glm::vec3 lightDirection = glm::normalize(glm::vec3 {1.0f, -3.0f, -1.0f});
};

const float MAX_FRAME_TIME = 0.25f;

void VulkanApp::run()
{
     // for swapchain rendering
    std::vector<std::unique_ptr<VulkanBuffer>> uboBuffers(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);

    // why not in single buffer? because of NonCoherentAtomSize

    for (size_t i = 0; i < uboBuffers.size(); i++)
    {
        uboBuffers[i] = std::make_unique<VulkanBuffer>(
            *device,
            sizeof(UniformBufferObject),
            1, // buffer only for single ubo
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, // this is uniform buffer
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT // host (cpu) can rw data here
        );
        uboBuffers[i]->map();
    }

    std::vector<VkDescriptorSet> globalDescriptorSets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < globalDescriptorSets.size(); i++)
    {
        auto bufferInfo = uboBuffers[i]->descriptorInfo();
        VulkanDescriptorWriter(*globalSetLayout, *globalPool)
            .writeBuffer(0, &bufferInfo)
            .build(globalDescriptorSets[i]);
    }
    
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto cameraObject = GameObject::createGameObject();

	while (!window->shouldClose()) {
		glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        deltaTime = glm::min(deltaTime, MAX_FRAME_TIME);

        keyboardInput->move(window->getPtr(), deltaTime, cameraObject);

        //std::cout << "position=" << cameraObject.transform.position_str() << std::endl;
        //std::cout << "rotation=" << cameraObject.transform.rotation_str() << std::endl;

        //camera->setViewDirection(glm::vec3(0.0f), glm::vec3(0.5f, 0.0f, 1.0f));
        //camera->setViewTarget(glm::vec3(-1.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.5f));
        camera->setViewYXZ(cameraObject.transform.position, cameraObject.transform.rotation);

        float aspect = renderer->getAspectRatio();
        // height is constant, width is flexible
        // right - left = aspect * (bottom - top) (y is inverted)

        //camera->setOrthographicProjection(-aspect * 0.6f, aspect * 0.6f, -0.6f, 0.6f, -5, 5);
        camera->setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);

		// On Linux, resizing can be occurs wrong rendering
		// On Linux, you need another frame update method
		if (auto commandBuffer = renderer->beginFrame()) {
            int frameIndex = renderer->getFrameIndex();
            VulkanFrameInfo frameInfo {
                frameIndex, deltaTime, 
                commandBuffer,
                *camera,
                globalDescriptorSets[frameIndex]
            };

            // update
            UniformBufferObject ubo{};
            ubo.projectionView = camera->getProjection() * camera->getView();
            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();

            // rendering
			renderer->beginSwapChainRenderPass(commandBuffer);
			renderSystem->renderGameObjects(frameInfo, gameObjects);
			renderer->endSwapChainRenderPass(commandBuffer);
			renderer->endFrame();
		}
	}
	// Fix errors while dispose Vulkan
	vkDeviceWaitIdle(device->device());
}

void VulkanApp::createDescriptors()
{
    globalPool = VulkanDescriptorPool::Builder(*device)
        .setMaxSets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VulkanSwapChain::MAX_FRAMES_IN_FLIGHT)
        //.addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VulkanSwapChain::MAX_FRAMES_IN_FLIGHT)
        .build();
    globalSetLayout = VulkanDescriptorSetLayout::Builder(*device)
        .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
        .build();
}

void VulkanApp::loadGameObjects(const std::string &modelPath)
{
    std::shared_ptr<VulkanModel> testModel = VulkanModel::createModelFromFile(*device, modelPath);

    auto gameObj = GameObject::createGameObject();
    gameObj.model = testModel;
    gameObj.transform.position = { 0.0f, 0.0f, 1.0f };
    gameObj.transform.rotation = { 0.0f, 0.0f, 0.0f };
    //gameObj.transform.rotation = { 1.0f, 1.0f, 0.5f, 0.0f };
    
    //gameObj.transform.scale = { 1.5f, 1.5f, 1.5f };
    gameObj.transform.scale = { 0.5f, 0.5f, 0.5f };

    gameObjects.push_back(std::move(gameObj));
}