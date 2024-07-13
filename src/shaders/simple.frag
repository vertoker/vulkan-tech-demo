// GLSL fragment shader
#version 450

// Input
layout(location = 0) in vec3 fragColor;

// Output
layout(location = 0) out vec4 outColor;

// Input location and Output location is different buffers

void main() {
	outColor = vec4(fragColor, 1.0);
}