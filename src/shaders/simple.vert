// GLSL vertex shader
#version 450

//#extension GL_KHR_vulkan_glsl : enable
// (-1 -> 1 ; -1 -> 1)

// Input
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;

// Input constants (individual data per drawing)
layout(push_constant) uniform Push {
	mat4 transform;
	vec3 color;
} push;

// Output
// layout(location = 0) out vec3 fragColor; // use it if you need to test color blending

void main() {
	// Not B*A, use A*B for matrix multiplication
	//gl_Position = vec4(push.transform * position + push.offset, 1.0);
	//fragColor = color; // color is a special field for baricentric color blending

	gl_Position = push.transform * vec4(position, 1.0);
	fragColor = color;
}