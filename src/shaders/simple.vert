// GLSL vertex shader
#version 450

//#extension GL_KHR_vulkan_glsl : enable
// (-1 -> 1 ; -1 -> 1)

// Input
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

// Input constants (individual data per drawing)
layout(push_constant) uniform Push {
	mat2 transform;
	vec2 offset;
	vec3 color;
} push;

// Output
// layout(location = 0) out vec3 fragColor; // use it if you need to test color blending

void main() {
	// Not B*A, use A*B for matrix multiplication
	gl_Position = vec4(push.transform * position + push.offset, 0.0, 1.0);
	//fragColor = color; // color is a special field for baricentric color blending
}