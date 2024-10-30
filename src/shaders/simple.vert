// GLSL vertex shader
#version 450

//#extension GL_KHR_vulkan_glsl : enable
// (-1 -> 1 ; -1 -> 1)

// Input
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;

layout(set = 0, binding = 0) uniform UniformBufferObject {
	mat4 projectionViewMatrix;
	//vec3 lightDirection;

	vec4 ambientLightColor; // w is for intensity
	vec3 lightPosition;
	vec4 lightColor; // w is for intensity
} ubo;

// Input constants (individual data per drawing)
layout(push_constant) uniform Push {
// only 128 bit = 2 4x4 matrices
	mat4 modelMatrix;
	mat4 normalMatrix;
} push;

void main() {
	// Not B*A, use A*B for matrix multiplication
	//gl_Position = vec4(push.modelMatrix * position + push.offset, 1.0);
	//fragColor = color; // color is a special field for baricentric color blending

	vec4 positionWorld = push.modelMatrix * vec4(position, 1.0);
	vec3 normalWorld = normalize(mat3(push.normalMatrix) * normal);
	gl_Position = ubo.projectionViewMatrix * positionWorld;

	// convert mat4 to mat3 remove translation, stayed only direction, which needed for
	// calculating object space normal to world space normal
	// works only if sx == sy == sz
	//vec3 normalWorld = normalize(mat3(push.modelMatrix) * normal);

	//mat3 normalMatrix = transpose(inverse(mat3(push.modelMatrix)));
	//vec3 normalWorld = normalize(normalMatrix * normal);

	// directional light realization
	//float lightIntensity = AMBIENT + max(dot(normalWorld, ubo.lightDirection), 0);
	//fragColor = lightIntensity * color;

	// point light realization
	vec3 distanceToLight = ubo.lightPosition - positionWorld.xyz;
	vec3 directionToLight = normalize(distanceToLight);
	
	vec3 ambientLightColor = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;

	// dot product of vec3 itself is easy squared distance
	float attenuation = 1.0 / dot(distanceToLight, distanceToLight);
	vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuation;
	vec3 diffuseLight = lightColor * max(dot(normalWorld, directionToLight), 0);

	fragColor = (diffuseLight + ambientLightColor) * color;
}