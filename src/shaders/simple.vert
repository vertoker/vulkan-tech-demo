// GLSL vertex shader
#version 450

//#extension GL_KHR_vulkan_glsl : enable
// (-1 -> 1 ; -1 -> 1)

// Input
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

// Output
layout(location = 0) out vec3 fragColor;

// Input location and Output location is different buffers

void main() {
	gl_Position = vec4(position, 0.0, 1.0);
	fragColor = color;
}