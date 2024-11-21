#include "app.hpp"

#include <ecs/components.hpp>

#include <tuple>
#include <numeric>
#include <glm/ext/matrix_transform.hpp>

VulkanApp::VulkanApp(VulkanAppSettings& settings)
{
    // Base, on first creation
	window = std::make_unique<VulkanWindow>(settings.screenWidth, settings.screenHeight, settings.name);
	device = std::make_unique<VulkanDevice>(*window);

    // Rendering core
	renderer = std::make_unique<VulkanRenderer>(*window, *device);
    camera = std::make_unique<VulkanCamera>();
    createDescriptors();
    createRenderSystems(settings);

    // More app-like data
    keyboardInput = std::make_unique<InputKeyboardController>();

	loadGameObjects(settings.modelPath);
}
VulkanApp::~VulkanApp()
{

}

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
    Transform cameraObject{};

    cameraObject.position = glm::vec3{ 0.0f, -1.5f, 4.0f };
    cameraObject.rotation = glm::vec3{ -0.4f, glm::pi<float>(), 0.0f };

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
        camera->setViewYXZ(cameraObject.position, cameraObject.rotation);

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
                globalDescriptorSets[frameIndex],
                registry
            };

            // update
            UniformBufferObject ubo{};
            ubo.projection = camera->getProjection();
            ubo.view = camera->getView();
            ubo.inverseView = camera->getInverseView();
            pointLightSystem->updateLights(frameInfo, ubo);
            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();

            // rendering
			renderer->beginSwapChainRenderPass(commandBuffer);
            worldRenderSystem->render(frameInfo);
            pointLightSystem->render(frameInfo);
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
        .addBinding(0, 
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 
            VK_SHADER_STAGE_ALL_GRAPHICS) // allow vertex/fragment lighting
        .build();
}

void VulkanApp::createRenderSystems(VulkanAppSettings& settings)
{
    worldRenderSystem = std::make_unique<WorldRenderSystem>(*device, 
		renderer->getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout(),
		settings.world_vertShaderPath, 
        settings.world_fragShaderPath);
    pointLightSystem = std::make_unique<PointLightSystem>(*device, 
		renderer->getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout(),
		settings.pointLight_vertShaderPath, 
        settings.pointLight_fragShaderPath);
}

void VulkanApp::loadGameObjects(const std::string &modelPath)
{
    std::shared_ptr<VulkanModel> testModel = VulkanModel::createModelFromFile(*device, modelPath);

    auto entity1 = registry.create();
    auto& tr1 = registry.emplace<Transform>(entity1);
    auto& model1 = registry.emplace<ModelRenderer>(entity1);

    model1.model = testModel;
    tr1.position = { -1.0f, 1.5f, 1.0f };
    tr1.rotation = { 0.0f, 0.0f, 0.0f };
    //tr1.rotation = { 1.0f, 1.0f, 0.5f, 0.0f };
    
    //tr1.scale = { 1.5f, 1.5f, 1.5f };
    tr1.scale = { 1.0f, 1.0f, 1.0f };
    //tr1.scale = { 0.5f, 0.5f, 0.5f };

    // Lights
    static const std::vector<glm::vec3> lightColors {
        {1.0f, 0.1f, 0.1f},
        {0.1f, 0.1f, 1.0f},
        {0.1f, 1.0f, 0.1f},
        {1.0f, 1.0f, 0.1f},
        {0.1f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f}
    };

    for (size_t i = 0; i < lightColors.size(); i++) {
        auto lightEntity = registry.create();
        auto& lightTr = registry.emplace<Transform>(lightEntity);
        auto& lightComponent = registry.emplace<PointLight>(lightEntity);

        lightComponent.color = lightColors[i];
        lightComponent.lightIntensity = 0.9f;
        lightTr.scale = glm::vec3{ 0.1f }; // radius

        auto rotateLight = glm::rotate(glm::mat4(1.0f), (i * glm::two_pi<float>()) / lightColors.size(), {0.0f, -1.0f, 0.0f});
        lightTr.position = glm::vec3(rotateLight * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f));
    }
}