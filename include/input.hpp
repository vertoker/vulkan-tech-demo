#pragma once

#include "gameobject.hpp"
#include "render/window.hpp"

// std
#include <limits>

class InputKeyboardController {
public:
	struct KeyMappings {
		int moveForward = GLFW_KEY_W;
		int moveBackward = GLFW_KEY_S;
		int moveLeft = GLFW_KEY_A;
		int moveRight = GLFW_KEY_D;
		int moveUp = GLFW_KEY_E;
		int moveDown = GLFW_KEY_Q;

		int lookUp = GLFW_KEY_UP;
		int lookDown = GLFW_KEY_DOWN;
		int lookLeft = GLFW_KEY_LEFT;
		int lookRight = GLFW_KEY_RIGHT;
	};

	// This is simplification will be works only in one window
	void move(GLFWwindow* window, float dt, GameObject& controller);

	KeyMappings keys{};
	float moveSpeed{ 0.3f };
	float lookSpeed{ 0.8f };
};