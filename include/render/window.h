#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

class VulkanWindow {
public:
	VulkanWindow(const int& w, const int& h, const std::string& name);
	~VulkanWindow();

	// Copy condition for only one GLFWwindow
	VulkanWindow(const VulkanWindow&) = delete;
	VulkanWindow& operator = (const VulkanWindow&) = delete;

	bool shouldClose() { return glfwWindowShouldClose(window); }
	bool wasWindowResized() { return framebufferResized; }
	void resetWindowResizedFlag() { framebufferResized = false; }
	GLFWwindow* getPtr() { return window; }

	VkExtent2D getExtent() 
	{
		return { 
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		}; 
	}

	void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

private:
	static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
	void initWindow();

	int width;
	int height;
	bool framebufferResized = false;

	std::string windowName;

	GLFWwindow* window;
};