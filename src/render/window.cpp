#include "window.h"

VulkanWindow::VulkanWindow(const int w, const int h, const char* name)
{
	this->width = w;
	this->height = h;
	this->windowName = name;

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, windowName, nullptr, nullptr);
}

VulkanWindow::~VulkanWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}
