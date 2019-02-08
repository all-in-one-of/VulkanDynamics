#version 450
//#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UniformFragBufferObject {
	vec3 Ambient;
	vec3 LightColor;
    vec3 LightPosition; // location of the light, eye space
	float Shininess;
	float Strength;
	vec3 EyeDirection;
	float ConstantAttenuation; // attenuation coefficients
	float LinearAttenuation;
	float QuadraticAttenuation;
	mat4 viewMatrix ;
	mat4 eyeViewMatrix;
} ufbo;

layout (location = 0 ) in vec3 fragColor;
layout (location = 1 ) in vec3 Normal;
layout (location = 2 ) in vec4 Position;
layout (location = 3 ) in vec3 lightPos;
layout (location = 4 ) in vec3 NormalView;

layout(location = 0) out vec4 outColor;

void main() {

	vec3 lightPosView = vec3 (ufbo.eyeViewMatrix * vec4 (ufbo.LightPosition, 1.0));
	vec3 lightDirectionView = lightPosView - vec3(Position);
	float lightDistance = length(lightDirectionView);


    outColor = vec4 ( fragColor, 1.0f );
}