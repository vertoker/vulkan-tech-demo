#pragma once

// GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

class VulkanCamera {
public:
	// This is box view
	void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
	// This is frustum view
	void setPerspectiveProjection(float verticalFOV, float aspect, float near, float far);

	const glm::mat4 getProjection() const { return projectionMatrix; }

private:
	// By default is identity matrix
	glm::mat4 projectionMatrix{ 1.0f };
};