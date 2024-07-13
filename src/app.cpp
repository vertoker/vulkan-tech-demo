#include "app.h"

#include <tuple>

VulkanApp::VulkanApp(VulkanAppSettings& settings)
{
	window = std::make_unique<VulkanWindow>(settings.screenWidth, settings.screenHeight, settings.name);
	device = std::make_unique<VulkanDevice>(*window);
	swapChain = std::make_unique<VulkanSwapChain>(*device, window->getExtent());

	loadModels();
	createPipelineLayout();
	createPipeline(settings);
	createCommandBuffers();
}
VulkanApp::~VulkanApp()
{
	vkDestroyPipelineLayout(device->device(), pipelineLayout, nullptr);
}

void VulkanApp::run()
{
	while (!window->shouldClose()) {
		glfwPollEvents();
		drawFrame();
	}
	// Fix errors while dispose Vulkan
	vkDeviceWaitIdle(device->device());
}

void VulkanApp::loadModels()
{
	std::vector<VulkanModel::Vertex> vertices {
		{ { 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
		{ { 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f } },
		{ { -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } }
	};
	testModel = std::make_unique<VulkanModel>(*device, vertices);
}

void VulkanApp::createPipelineLayout()
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(device->device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		throw std::runtime_error("failed to create pipeline layout");
}

void VulkanApp::createPipeline(VulkanAppSettings& settings)
{
	auto pipelineInfo = VulkanPipeline::defaultConfigInfo(settings.screenWidth, settings.screenHeight);
	pipelineInfo.renderPass = swapChain->getRenderPass();
	pipelineInfo.pipelineLayout = pipelineLayout;

	pipeline = std::make_unique<VulkanPipeline>(*device, pipelineInfo, settings.vertShaderPath, settings.fragShaderPath);
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

	for (int i = 0; i < commandBuffers.size(); i++)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
			throw std::runtime_error("failed to begin recording command buffer!");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = swapChain->getRenderPass();
		renderPassInfo.framebuffer = swapChain->getFrameBuffer(i);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

		// can't declare std::array
		std::vector<VkClearValue> clearValues{2};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		pipeline->bind(commandBuffers[i]);

		testModel->bind(commandBuffers[i]);
		testModel->draw(commandBuffers[i]);

		vkCmdEndRenderPass(commandBuffers[i]);
		
		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to record command buffer!");
	}
}

void VulkanApp::drawFrame()
{
	uint32_t imageIndex;
	auto result = swapChain->acquireNextImage(&imageIndex);
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		throw std::runtime_error("failed to present swap chain image!");

	result = swapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
	if (result != VK_SUCCESS)
		throw std::runtime_error("failed to present swap chain image!");

}
