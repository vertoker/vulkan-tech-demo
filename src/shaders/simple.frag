// GLSL fragment shader
#version 450

// Input
// layout(location = 0) in vec3 fragColor; // use it if you need to test color blending

// Input constants (individual data per drawing)
// You must use only ONE constant per shader entry point
layout(push_constant) uniform Push { // this name doesn't matter, name as you want
	// Order of members is important
	mat2 transform;
	vec2 offset;
	vec3 color;
} push; // name of it

// Output
layout(location = 0) out vec4 outColor;

// Input location and Output location is different buffers

void main() {
	outColor = vec4(push.color, 1.0);
}