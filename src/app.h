#pragma once

#include "render/window.h"

class VulkanApp {
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;
	static constexpr const char* NAME = "vulkan tech demo";

	void run() {
		init();
		while (!this->window.shouldClose()) {
			loop();
		}
		cleanup();
	}

private:
	VulkanWindow window{ WIDTH, HEIGHT, NAME };

	void init();
	void loop();
	void cleanup();
};