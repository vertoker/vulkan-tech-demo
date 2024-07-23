#include "app.h"

#include <tuple>

VulkanApp::VulkanApp(VulkanAppSettings& settings)
{
	window = std::make_unique<VulkanWindow>(settings.screenWidth, settings.screenHeight, settings.name);
	device = std::make_unique<VulkanDevice>(*window);

	fragShaderPath = settings.fragShaderPath;
	vertShaderPath = settings.vertShaderPath;

	loadGameObjects();
	createPipelineLayout();
	recreateSwapChain();
	createCommandBuffers();
}
VulkanApp::~VulkanApp()
{
	vkDestroyPipelineLayout(device->device(), pipelineLayout, nullptr);
	//delete swapChain;
}

void VulkanApp::run()
{
	while (!window->shouldClose()) {
		glfwPollEvents();
		// On Linux, resizing can be occurs wrong rendering
		// On Linux, you need another frame update method
		drawFrame();
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
	PipelineConfigInfo pipelineInfo{};
	VulkanPipeline::defaultConfigInfo(pipelineInfo);
	pipelineInfo.renderPass = swapChain->getRenderPass();
	pipelineInfo.pipelineLayout = pipelineLayout;

	pipeline = std::make_unique<VulkanPipeline>(*device, pipelineInfo, vertShaderPath, fragShaderPath);
}

void VulkanApp::createCommandBuffers()
{
	commandBuffers.resize(swapChain->imageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = device->getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(device->device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate command buffers!");
}
void VulkanApp::freeCommandBuffers()
{
	vkFreeCommandBuffers(
		device->device(),
		device->getCommandPool(),
		static_cast<uint32_t>(commandBuffers.size()),
		commandBuffers.data()
	);
	commandBuffers.clear();
}

void VulkanApp::drawFrame()
{
	uint32_t imageIndex;
	auto result = swapChain->acquireNextImage(&imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return;
	}

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		throw std::runtime_error("failed to present swap chain image!");

	recordCommandBuffer(imageIndex);
	result = swapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window->wasWindowResized()) {
		window->resetWindowResizedFlag();
		recreateSwapChain();
		return;
	}

	if (result != VK_SUCCESS)
		throw std::runtime_error("failed to present swap chain image!");

}

void VulkanApp::recreateSwapChain()
{
	auto extent = window->getExtent();
	while (extent.width == 0 || extent.height == 0) {
		extent = window->getExtent();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(device->device());

	if (swapChain == nullptr) {
		swapChain = std::make_unique<VulkanSwapChain>(*device, extent);
	} else {
		swapChain = std::make_unique<VulkanSwapChain>(*device, extent, std::move(swapChain));
		if (swapChain->imageCount() != commandBuffers.size()) {
			freeCommandBuffers();
			createCommandBuffers();
		}
	}

	// Need new pipeline because of new swapchain
	createPipeline();
	// TODO: If render pass compatible do nothing else
}

void VulkanApp::recordCommandBuffer(int imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
		throw std::runtime_error("failed to begin recording command buffer!");

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = swapChain->getRenderPass();
	renderPassInfo.framebuffer = swapChain->getFrameBuffer(imageIndex);

	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

	// can't declare std::array
	std::vector<VkClearValue> clearValues{ 2 };
	clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	// Render pass - list of commands for GPU
	vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	// Dynamic viewport
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	auto extent = swapChain->getSwapChainExtent();
	viewport.width = static_cast<float>(extent.width);
	viewport.height = static_cast<float>(extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = extent;

	vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
	vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

	// Render gameobjects
	renderGameObjects(commandBuffers[imageIndex]);

	vkCmdEndRenderPass(commandBuffers[imageIndex]);

	if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
		throw std::runtime_error("failed to record command buffer!");
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
