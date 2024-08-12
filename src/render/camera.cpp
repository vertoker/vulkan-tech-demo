#include "camera.h"

// std
#include <cassert>
#include <limits>

void VulkanCamera::setOrthographicProjection(float left, float right, float top, float bottom, float near, float far)
{
	projectionMatrix = glm::mat4{ 1.0f }; // identity matrix
	
	projectionMatrix[0][0] = 2.0f / (right - left);
	projectionMatrix[1][1] = 2.0f / (bottom - top);
	projectionMatrix[2][2] = 2.0f / (far - near);

	projectionMatrix[3][0] = -(right + left) / (right - left);
	projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
	projectionMatrix[3][2] = -near / (far - near);
}

void VulkanCamera::setPerspectiveProjection(float verticalFOV, float aspect, float near, float far)
{
	// aspect = width / height, if aspect near to 0, then this cause error
	assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
	// Constant for computation of matrix
	const float tanHalfVerticalFOV = tan(verticalFOV / 2.0f);
	projectionMatrix = glm::mat4{ 1.0f }; // identity matrix

	projectionMatrix[0][0] = 1.0f / (aspect * tanHalfVerticalFOV);
	projectionMatrix[1][1] = 1.0f / tanHalfVerticalFOV;
	projectionMatrix[2][2] = far / (far - near);
	projectionMatrix[2][3] = 1.0f;
	projectionMatrix[3][2] = -(far * near) / (far - near);
}
