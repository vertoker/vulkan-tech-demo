#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

class VulkanWindow {
public:
	VulkanWindow(const int w, const int h, const char* name);
	~VulkanWindow();

	// Copy condition for only one GLFWwindow
	VulkanWindow(const VulkanWindow&) = delete;
	VulkanWindow& operator = (const VulkanWindow&) = delete;

	bool shouldClose() { return glfwWindowShouldClose(window); }

private:
	int width;
	int height;
	const char* windowName;

	GLFWwindow* window;
};