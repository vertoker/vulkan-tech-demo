#include "app.h"

VulkanApp::VulkanApp(VulkanAppSettings& settings)
{
	window = std::make_unique<VulkanWindow>(settings.screenWidth, settings.screenHeight, settings.name);
	device = std::make_unique<VulkanDevice>(*window);
	swapChain = std::make_unique<VulkanSwapChain>(*device, window->getExtent());

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
	}
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

}

void VulkanApp::drawFrame()
{

}
