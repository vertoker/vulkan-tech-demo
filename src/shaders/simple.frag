// GLSL fragment shader
#version 450

// Input
// use it if you need to test color blending
layout(location = 0) in vec3 fragColor;

// Input constants (individual data per drawing)
// You must use only ONE constant per shader entry point
layout(push_constant) uniform Push {
	mat4 transform;
	vec3 color;
} push;

// Output
layout(location = 0) out vec4 outColor;

// Input location and Output location is different buffers

void main() {
	outColor = vec4(fragColor, 1.0);
}