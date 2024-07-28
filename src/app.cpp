#include "app.h"

#include <tuple>

VulkanApp::VulkanApp(VulkanAppSettings& settings)
{
	window = std::make_unique<VulkanWindow>(settings.screenWidth, settings.screenHeight, settings.name);
	device = std::make_unique<VulkanDevice>(*window);
	renderer = std::make_unique<VulkanRenderer>(*window, *device);

	fragShaderPath = settings.fragShaderPath;
	vertShaderPath = settings.vertShaderPath;

	loadGameObjects();
	createPipelineLayout();
	createPipeline();
}
VulkanApp::~VulkanApp()
{
	vkDestroyPipelineLayout(device->device(), pipelineLayout, nullptr);
}

void VulkanApp::run()
{
	while (!window->shouldClose()) {
		glfwPollEvents();
		// On Linux, resizing can be occurs wrong rendering
		// On Linux, you need another frame update method
		if (auto commandBuffer = renderer->beginFrame()) {
			renderer->beginSwapChainRenderPass(commandBuffer);
			renderGameObjects(commandBuffer);
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

void VulkanApp::createPipelineLayout()
{
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(PushConstantData);

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	if (vkCreatePipelineLayout(device->device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		throw std::runtime_error("failed to create pipeline layout");
}

void VulkanApp::createPipeline()
{
	assert(pipelineLayout != nullptr && "Can't create pipeline before pipeline layout");

	PipelineConfigInfo pipelineInfo{};
	VulkanPipeline::defaultConfigInfo(pipelineInfo);
	pipelineInfo.renderPass = renderer->getSwapChainRenderPass();
	pipelineInfo.pipelineLayout = pipelineLayout;

	pipeline = std::make_unique<VulkanPipeline>(*device, pipelineInfo, vertShaderPath, fragShaderPath);
}

void VulkanApp::renderGameObjects(VkCommandBuffer commandBuffer)
{
	// Test rotation 2
	float i = 0;
	for (auto& obj : gameObjects) {
		i += 0.01f;
		obj.transform2D.rotation = glm::mod(obj.transform2D.rotation + 0.001f * i, 2.0f * glm::two_pi<float>());
	}

	pipeline->bind(commandBuffer);

	for (auto& obj : gameObjects) {
		// Test rotation anim
		//obj.transform2D.rotation = glm::mod(obj.transform2D.rotation + 0.01f, glm::two_pi<float>());

		PushConstantData push{};
		// Temp movement
		push.offset = obj.transform2D.position;
		push.color = obj.color;
		push.transform = obj.transform2D.mat2();

		vkCmdPushConstants(commandBuffer, pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0, sizeof(PushConstantData), &push);
		obj.model->bind(commandBuffer);
		obj.model->draw(commandBuffer);
	}
}
