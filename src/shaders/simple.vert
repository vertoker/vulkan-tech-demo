// GLSL vertex shader
#version 450

//#extension GL_KHR_vulkan_glsl : enable
// (-1 -> 1 ; -1 -> 1)

// Input
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

// Input constants (individual data per drawing)
// You must use only ONE constant per shader entry point
layout(push_constant) uniform Push { // this name doesn't matter, name as you want
	// Order of members is important
	vec2 offset;
	vec3 color;
} push; // name of it

// Output
// layout(location = 0) out vec3 fragColor; // use it if you need to test color blending

// Input location and Output location is different buffers

void main() {
	gl_Position = vec4(position + push.offset, 0.0, 1.0);
	//fragColor = color; // color is a special field for baricentric color blending
}