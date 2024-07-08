#include "app.h"

VulkanApp::VulkanApp(const VulkanAppSettings& settings)
{
	window = new VulkanWindow(settings.Width, settings.Height, settings.Name);
	pipeline = new VulkanPipeline(settings.VertShaderPath, settings.FragShaderPath);
}

VulkanApp::~VulkanApp()
{
	delete window;
	delete pipeline;
}

void VulkanApp::run()
{
	while (!window->shouldClose()) {
		glfwPollEvents();
	}
}
