#include "render/window.hpp"

// std
#include <stdexcept>
#include <iostream>

VulkanWindow::VulkanWindow(const int& w, const int& h, const std::string& name)
{
	this->width = w;
	this->height = h;
	this->windowName = name;

	initWindow();
}
VulkanWindow::~VulkanWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void VulkanWindow::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}


void VulkanWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface");
	}
}

void VulkanWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto vulkanWindow = reinterpret_cast<VulkanWindow*>(glfwGetWindowUserPointer(window));
	vulkanWindow->framebufferResized = true;
	vulkanWindow->width = width;
	vulkanWindow->height = height;
}