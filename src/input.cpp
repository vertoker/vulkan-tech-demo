#include "input.h"

void InputKeyboardController::move(GLFWwindow* window, float dt, GameObject& controller)
{
	glm::vec3 rotate{ 0.0f };

	if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.0f;
	if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.0f;
	if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.0f;
	if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.0f;

	// Simple method to check rotation is zero: dot product by itself
	if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon)
		controller.transform.rotation += lookSpeed * dt * glm::normalize(rotate);

	const auto clampX = glm::radians<float>(85.0f);
	controller.transform.rotation.x += glm::clamp(controller.transform.rotation.x, clampX, clampX);
	// mod = repeat, but for both directions
	controller.transform.rotation.y = glm::mod(controller.transform.rotation.y, glm::two_pi<float>());

	float yaw = controller.transform.rotation.y;
	const glm::vec3 forwardDir{ sin(yaw), 0.0f, cos(yaw) };
	const glm::vec3 rightDir{ forwardDir.z, 0.0f, -forwardDir.x };
	const glm::vec3 upDir{ 0.0f, -1.0f, 0.0f };

	glm::vec3 moveDir{ 0.0f };

	if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
	if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
	if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
	if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
	if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
	if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

	// Simple method to check position is zero: dot product by itself
	if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon)
		controller.transform.position += moveDir * dt * glm::normalize(moveDir);
	// finish starts with window.cpp
}
