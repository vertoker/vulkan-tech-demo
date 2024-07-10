#include "app.h"

#include "iostream"

VulkanApp::VulkanApp(const VulkanAppSettings& settings) {
	window = new VulkanWindow(settings.screenWidth, settings.screenHeight, settings.name);
	device = new VulkanDevice(*window);
	swapChain = new VulkanSwapChain(*device, window->getExtent());
	pipeline = new VulkanPipeline(*device, settings.pipelineInfo, settings.vertShaderPath, settings.fragShaderPath);
}
VulkanApp::~VulkanApp() {
	delete device;
	delete window;
	delete pipeline;
	delete swapChain;
}

void VulkanApp::run() {
	while (!window->shouldClose()) {
		glfwPollEvents();
	}
}
